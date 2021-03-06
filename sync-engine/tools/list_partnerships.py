#!/usr/bin/env python

import dbus
import sys

sys.path.insert(0, "..")
from SyncEngine.constants import *

try:
	engine = dbus.Interface(dbus.SessionBus().get_object(DBUS_SYNCENGINE_BUSNAME, DBUS_SYNCENGINE_OBJPATH), DBUS_SYNCENGINE_BUSNAME)
	item_types = engine.GetItemTypes()
except:
	print "\nerror: unable to connect to running sync-engine"
	print "\nPlease ensure sync-engine is running before executing this command\n"
	sys.exit(1)

try:
	partnerships = engine.GetPartnerships()

	print
	print "            AVAILABLE DEVICE PARTNERSHIPS"
	print "Index   Name    Device          Host         SyncItems"
	print "-----   ----    ------          ----         ---------"
	print

	idx = 0

	for pship in partnerships:
	
	
		id,guid,name,hostname,devicename,storetype,items = pship
	
		s="%d\t%s\t%s\t%s\t" % (idx,name,devicename,hostname)
		if storetype==PSHMGR_STORETYPE_EXCH:
			s+="(Server)"
		elif storetype==PSHMGR_STORETYPE_AS:
			s+="["
			for item in items:
				s += "%s " % str(item_types[item])
			s+="]"
		else:
			s+="(unknown storage type)"
		print s
		idx+=1
	
	print "\n"
		
except dbus.DBusException,e:
	
	if e._dbus_error_name=="org.freedesktop.DBus.Python.SyncEngine.errors.Disconnected":
		print "error: No device connected"
	elif e._dbus_error_name=="org.freedesktop.DBus.Python.Exception":
		print e
	else:
		print "error: %s" % e._dbus_error_name
	sys.exit(0)



