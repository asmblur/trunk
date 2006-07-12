#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright (C) 2006  Ole André Vadla Ravnås <oleavr@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

import gobject
import dbus
import dbus.service
import dbus.glib

from twisted.internet import glib2reactor
glib2reactor.install()
from twisted.internet import reactor
from twisted.internet.protocol import Protocol, Factory
from twisted.web2 import server, channel

from pyrapi2 import *
from partnership import *
from airsync import *
from util import *
from interfaces import *
from errors import *
import socket

import os

from xml.dom import minidom

BUS_NAME = "org.synce.SyncEngine"
OBJECT_PATH = "/org/synce/SyncEngine"

class SyncEngine(dbus.service.Object):
    """
    A D-Bus service which provides an interface for synchronizing
    with Windows CE-based devices.
    """

    def __init__(self, bus_name, object_path):
        dbus.service.Object.__init__(self, bus_name, object_path)

        self.session = RAPISession(SYNCE_LOG_LEVEL_DEFAULT)
        self.cur_partnership = None
        self._get_partnerships()

    def _get_partnerships(self):
        self.partnerships = {}
        self.slots = [ None, None ]

        reg_entries = {}
        sync_entries = []

        # Inspect registry entries
        hklm = self.session.HKEY_LOCAL_MACHINE
        partners_key = hklm.create_sub_key(
                r"Software\Microsoft\Windows CE Services\Partners")
        for pos in xrange(1, 3):
            key = partners_key.create_sub_key("P%d" % pos)
            if key.disposition == REG_OPENED_EXISTING_KEY:
                try:
                    id = key.query_value("PId")
                    hostname = key.query_value("PName")

                    if id != 0 and len(hostname) > 0:
                        reg_entries[hostname] = (pos, id)
                except RAPIError:
                    pass
            key.close()
        partners_key.close()

        # Look up the synchronization data on each
        for ctic in config_query_get(self.session, "Sync", "Sources").children.values():
            sub_ctic = config_query_get(self.session, "Sync.Sources", ctic.type, recursive=True)

            guid = sub_ctic.type
            hostname = sub_ctic["Server"]
            description = sub_ctic["Name"]

            if hostname in reg_entries:
                pos, id = reg_entries[hostname]
                del reg_entries[hostname]

                pship = Partnership(pos, id, guid, hostname, description)
                if pship.is_our():
                    self.cur_partnership = pship

                self.partnerships[id] = pship
                self.slots[pos-1] = pship

                engine = sub_ctic.children["Engines"].children[GUID_WM5_ENGINE]
                for provider in engine.children["Providers"].children.values():
                    if int(provider["Enabled"]) != 0:
                        pship.sync_items.append(SYNC_ITEM_ID_FROM_GUID[provider.type])
            else:
                sync_entries.append((guid, hostname, description))

        for entry in reg_entries.values():
            print "deleting dangling registry entry:", entry
            hklm.delete_sub_key(
                r"Software\Microsoft\Windows CE Services\Partners\P%d" % entry[0])

        for entry in sync_entries:
            print "deleting dangling sync source:", entry
            config_query_remove(self.session, "Sync.Sources", entry[0])

        for pship in self.slots:
            print pship

    @dbus.service.method(SYNC_ENGINE_INTERFACE, in_signature='', out_signature='a{us}')
    def GetItemTypes(self):
        """
        Get a list of synchronizable item types.

        Returns:
        a dictionary mapping item identifiers to names.
        """

        types = {}
        for id, val in SYNC_ITEMS.items():
            name, readonly = val
            types[id] = name

        return types

    @dbus.service.method(SYNC_ENGINE_INTERFACE, in_signature='', out_signature='a(ussau)')
    def GetPartnerships(self):
        """
        Get a list of partnerships on the device.

        Returns:
        an array of structs containing:
          an integer identifier
          a string with the partnership name
          a string with the hostname associated with it
          an array of sync item ids

        Possible Errors:
        Disconnected
        """

        ret = []
        for p in self.partnerships.values():
            ret.append((p.id, p.name, p.hostname, p.sync_items))
        return ret

    @dbus.service.method(SYNC_ENGINE_INTERFACE, in_signature='sau', out_signature='u')
    def CreatePartnership(self, name, sync_items):
        """
        Create a new partnership on the device.

        Parameters:
        name - a user-specified name of the partnership, for example "Linux desktop"
        sync_items - an array of sync item ids

        Returns:
        an integer identifying the newly created partnership

        Possible Errors:
        Disconnected, InvalidArgument, NoFreeSlots
        """

        if len(name) > 20:
            raise InvalidArgument("name too long (20 chars max)")

        for item in sync_items:
            if not item in SYNC_ITEMS:
                raise InvalidArgument("sync item identifier %d is invalid" % item)

        if len(self.partnerships) == 2:
            raise NoFreeSlots("all slots are currently full")

        slot = 1
        for partnership in self.slots:
            if partnership == None:
                break
            slot += 1

        pship = Partnership(slot, generate_id(), generate_guid(),
                            socket.gethostname(), name)

        #
        # Create the synchronization config data source
        #
        source = Characteristic(pship.guid)
        source["Name"] = pship.name
        source["Server"] = pship.hostname
        source["StoreType"] = "2"

        engines = Characteristic("Engines")
        source.add_child(engines)

        engine = Characteristic(GUID_WM5_ENGINE)
        engines.add_child(engine)

        settings = Characteristic("Settings")
        settings["User"] = "DEFAULT"
        settings["Domain"] = "DEFAULT"
        settings["Password"] = "DEFAULT"
        settings["SavePassword"] = "1"
        settings["UseSSL"] = "0"
        settings["ConflictResolution"] = "1"
        settings["URI"] = "Microsoft-Server-ActiveSync"
        engine.add_child(settings)

        providers = Characteristic("Providers")
        engine.add_child(providers)

        for item_id, item_rec in SYNC_ITEMS.items():
            item_str, item_readonly = item_rec
            item_guid = SYNC_ITEM_ID_TO_GUID[item_id]
            item_enabled = (item_id in sync_items)

            provider = Characteristic(item_guid)
            provider["Enabled"] = str(int(item_enabled))
            provider["ReadOnly"] = str(int(item_readonly))
            provider["Name"] = item_str
            providers.add_child(provider)

        config_query_set(self.session, "Sync.Sources", source)

        #
        # Update the registry
        #
        hklm = self.session.HKEY_LOCAL_MACHINE

        partners_key = hklm.create_sub_key(
                r"Software\Microsoft\Windows CE Services\Partners")
        partners_key.set_value("PCur", pship.slot)

        key = partners_key.create_sub_key("P%d" % slot)
        key.set_value("PId", pship.id)
        key.set_value("DataSourceID", pship.guid)
        key.set_value("PName", pship.hostname)
        key.close()

        partners_key.close()

        #
        # Store it
        #
        self.partnerships[pship.id] = pship
        self.slots[pship.slot-1] = pship

        pship.create_sync_state()
        self.cur_partnership = pship

        return pship.id

    @dbus.service.method(SYNC_ENGINE_INTERFACE, in_signature='u', out_signature='')
    def DeletePartnership(self, id):
        """
        Delete a partnership on the device.

        Parameters:
        id - integer identifier of the partnership to be deleted

        Possible Errors:
        Disconnected, InvalidArgument
        """

        if not id in self.partnerships:
            raise InvalidArgument("invalid id %d" % id)

        pship = self.partnerships[id]

        config_query_remove(self.session, "Sync.Sources", pship.guid)

        if pship.is_our():
            pship.delete_sync_state()
            self.cur_partnership = None

        del self.partnerships[id]
        self.slots[pship.slot-1] = None

    @dbus.service.method(SYNC_ENGINE_INTERFACE, in_signature='', out_signature='')
    def StartSync(self):
        pship = self.cur_partnership
        if pship is None:
            raise NotAvailable("No partnership found")

        print "Starting synchronization"

        entries = config_query_get(self.session, None, "CM_ProxyEntries").children.values()
        transport = None
        for entry in entries:
            if entry.type.startswith("HTTP-"):
                transport = entry
                break

        if transport is None:
            raise ProtocolError("HTTP transport not found")

        transport = config_query_get(self.session, "CM_ProxyEntries", transport.type)

        dtpt = Characteristic("CurrentDTPTNetwork")
        dtpt["DestId"] = transport["SrcId"]
        config_query_set(self.session, "CM_NetEntries", dtpt)
        print "CurrentDTPTNetwork set"

        self._contacts_added = []

        res = ASResource(pship)
        res.connect("contact-added", self._contact_added_cb)
        res.connect("end-of-changeset", self._end_of_changeset_cb)

        site = server.Site(res)
        factory = channel.HTTPFactory(site)
        reactor.listenTCP(26675, factory)

        print "Starting synchronization"
        doc = minidom.Document()
        doc_node = doc.createElement("sync")
        doc_node.setAttribute("xmlns", "http://schemas.microsoft.com/as/2004/core")
        doc_node.setAttribute("type", "Interactive")
        doc.appendChild(doc_node)

        node = doc.createElement("partner")
        node.setAttribute("id", pship.guid)
        doc_node.appendChild(node)

        print "Calling sync_start()"
        #print doc_node.toprettyxml()

        self.session.sync_start(doc_node.toxml())
        print "Succeeded"

        # set the current partnership on the device
        #
        # this is probably only needed for legacy items,
        # i.e. non-PIM ones using RRAC...
        #
        print "Setting PCur...",
        partners = self.session.HKEY_LOCAL_MACHINE.create_sub_key(
                r"Software\Microsoft\Windows CE Services\Partners")
        partners.set_value("PCur", 1, REG_DWORD)
        print "success"

        #print "Calling start_replication...",
        #self.session.start_replication()
        #print "success"

        print "Calling sync_resume...",
        self.session.sync_resume()
        print "success"

    def _contact_added_cb(self, res, sid, contact):
        print "queuing remote contact add with sid %s [%s]" % (sid, contact["FileAs"])
        self._contacts_added.append((sid, contact))

    def _end_of_changeset_cb(self, res):
        print "end of changeset, pushing %d contact adds" % len(self._contacts_added)

        if self._contacts_added:
            self.ContactsAdded(self._contacts_added)
            self._contacts_added = []

        self.Synchronized()

    @dbus.service.signal(SYNC_ENGINE_INTERFACE, signature="a(sa{ss})")
    def ContactsAdded(self, contacts):
        print "Emitting ContactsAdded with %d contacts" % len(contacts)

    @dbus.service.signal(SYNC_ENGINE_INTERFACE, signature="")
    def Synchronized(self):
        print "Emitting Synchronized"


if __name__ == "__main__":
    session_bus = dbus.SessionBus()
    bus_name = dbus.service.BusName(BUS_NAME, bus=session_bus)
    obj = SyncEngine(bus_name, OBJECT_PATH)

    reactor.run()
