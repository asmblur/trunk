# -*- coding: utf-8 -*-
############################################################################
#    Copyright (C) 2006  Ole André Vadla Ravnås <oleavr@gmail.com>       #
#                                                                          #
#    This program is free software; you can redistribute it and#or modify  #
#    it under the terms of the GNU General Public License as published by  #
#    the Free Software Foundation; either version 2 of the License, or     #
#    (at your option) any later version.                                   #
#                                                                          #
#    This program is distributed in the hope that it will be useful,       #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#    GNU General Public License for more details.                          #
#                                                                          #
#    You should have received a copy of the GNU General Public License     #
#    along with this program; if not, write to the                         #
#    Free Software Foundation, Inc.,                                       #
#    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
############################################################################

"""
    Unhandled fields:
        Body (subnodes/attrs: Body{Size,Truncated}), Children (subnodes: Child),
        Yomi{CompanyName,FirstName,LastName}, {Customer,Government}Id,
        AccountName, MMS and Rtf
"""

import parser
import libxml2
from SyncEngine.constants import *
from SyncEngine.xmlutil import *

def from_airsync(as_node):
    dst_doc = parser.parser.convert(libxml2.parseDoc(as_node.toxml(encoding="utf-8")), SYNC_ITEM_CONTACTS, parser.FMT_FROM_AIRSYNC)
    return minidom.parseString(str(dst_doc))

def to_airsync(os_doc):
    dst_doc = parser.parser.convert(libxml2.parseDoc(os_doc.toxml(encoding="utf-8")), SYNC_ITEM_CONTACTS, parser.FMT_TO_AIRSYNC)
    dst_doc = minidom.parseString(str(dst_doc))
    return dst_doc