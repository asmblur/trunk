/* packet-rapi.c
 * Routines for Windows CE Remote API dissection
 * Copyright 2004, David Eriksson <twogood@users.sourceforge.net>
 *
 * $Id$
 *
 * Ethereal - Network traffic analyzer
 * By Gerald Combs <gerald@ethereal.com>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#ifdef NEED_SNPRINTF_H
# include "snprintf.h"
#endif

#include <epan/packet.h>
/* #include "packet-RAPI.h" */

#define RAPI_TCP_PORT 990

static const char* const rapi_command_names[] = {
  "CeFindFirstFile",
  "CeFindNextFile",
  "CeFindClose",
  "CeGetFileAttributes",
  "CeSetFileAttributes",
  "CeCreateFile",
  "CeReadFile",
  "CeWriteFile",
  "CeCloseHandle",
  "CeFindAllFiles",
  "CeFindFirstDatabase",
  "CeFindNextDatabase",
  "CeOidGetInfo",
  "CeCreateDatabase",
  "CeOpenDatabase",
  "CeDeleteDatabase",
  "CeReadRecordPropsEx",
  "CeWriteRecordProps",
  "CeDeleteRecord",
  "CeSeekDatabase",
  "CeSetDatabaseInfo",
  "CeSetFilePointer",
  "CeSetEndOfFile",
  "CeCreateDirectory",
  "CeRemoveDirectory",
  "CeCreateProcess",
  "CeMoveFile",
  "CeCopyFile",
  "CeDeleteFile",
  "CeGetFileSize",
  "CeRegOpenKeyEx",
  "CeRegEnumKeyEx",
  "CeRegCreateKeyEx",
  "CeRegCloseKey",
  "CeRegDeleteKey",
  "CeRegEnumValue",
  "CeRegDeleteValue",
  "CeRegQueryInfoKey",
  "CeRegQueryValueEx",
  "CeRegSetValueEx",
  "GetSystemMemoryDivision",
  "CeGetStoreInformation",
  "CeGetSystemMetrics",
  "CeGetDesktopDeviceCaps",
  "CeFindAllDatabases",
  "RegCopyFile",
  "RegRestoreFile",
  "CeGetSystemInfo",
  "CeSHCreateShortcut",
  "CeSHGetShortcutTarget",
  "GetPasswordActive",
  "SetPasswordActive",
  "CeCheckPassword",
  "SetPassword",
  "CeEventHasOccured",
  "CeSyncTimeToPc",
  "CeStartReplication",
  "CeGetFileTime",
  "CeSetFileTime",
  "CeGetVersionEx",
  "CeGetWindow",
  "CeGetWindowLong",
  "CeGetWindowText",
  "CeGetClassName",
  "CeGlobalMemoryStatus",
  "CeGetSystemPowerStatusEx",
  "SetSystemMemoryDivision",
  "CeGetTempPath",
  "CeGetSpecialFolderPath",
  "CeRapiInvoke",
  "CeRestoreDatabase",
  "CeBackupDatabase",
  "CeBackupFile",
  "CeKillAllApps",
  "CeFindFirstDatabaseEx",
  "CeFindNextDatabaseEx",
  "CeCreateDatabaseEx",
  "CeSetDatabaseInfoEx",
  "CeOpenDatabaseEx",
  "CeDeleteDatabaseEx",
  "(unused)",
  "CeMountDBVol",
  "CeUnmountDBVol",
  "CeFlushDBVol",
  "CeEnumDBVolumes",
  "CeOidGetInfoEx",
  "CeProcessConfig"
};

/* Initialize the protocol and registered fields */
static int proto_RAPI = -1;
static int hf_RAPI_size = -1;
static int hf_RAPI_command = -1;
static int hf_RAPI_protocol_error_flag = -1;
static int hf_RAPI_protocol_error_code = -1;
static int hf_RAPI_data = -1;

/* Initialize the subtree pointers */
static gint ett_RAPI = -1;

/* Return the number of bytes eaten */
static int
add_optional_string(proto_tree *tree, tvbuff_t *tvb, gint start_offset, const char *label)
{
  gint offset = start_offset;
  guint32 has_string = tvb_get_letohl(tvb, offset);
  offset += 4;

  if (has_string)
  {
    guint32 size = tvb_get_letohl(tvb, offset);
    has_string = tvb_get_letohl(tvb, offset + 4);

    offset += 8;

    if (has_string)
    {
      char *str = tvb_fake_unicode(tvb, offset, size / 2, TRUE);
      proto_tree_add_text(tree, tvb, start_offset, size + 12, "%s: \"%s\"", label, str);
      g_free(str);

      offset += size;
    }
    else
      proto_tree_add_text(tree, tvb, start_offset, 12, "%s: (0x%x bytes return buffer)", label, size);
  }
  else
    proto_tree_add_text(tree, tvb, start_offset, 4, "%s: (NULL)", label);

  return offset - start_offset;
}

static int
add_string(proto_tree *tree, tvbuff_t *tvb, gint start_offset, const char *label)
{
  gint offset = start_offset;
  guint32 has_string;

  has_string = tvb_get_letohl(tvb, offset);
  offset += 4;

  if (has_string)
  {
    guint32 length = tvb_get_letohl(tvb, offset);
    char *str = tvb_fake_unicode(tvb, offset + 4, length, TRUE);
    proto_tree_add_text(tree, tvb, start_offset, 8 + length * 2, "%s: \"%s\"", label, str);
    g_free(str);

    offset += 4 + length * 2;
  }

  return offset - start_offset;
}

static int
add_uint16(proto_tree *tree, tvbuff_t *tvb, gint start_offset, const char *label)
{
  guint16 value = tvb_get_letohs(tvb, start_offset);
  proto_tree_add_text(tree, tvb, start_offset, 2, "%s: 0x%04x", label, value);
  return 2;
}

static int
add_uint32(proto_tree *tree, tvbuff_t *tvb, gint start_offset, const char *label)
{
  guint32 value = tvb_get_letohl(tvb, start_offset);
  proto_tree_add_text(tree, tvb, start_offset, 4, "%s: 0x%08x", label, value);
  return 4;
}


/* Code to actually dissect the packets */
static void
dissect_RAPI(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
  int bytes;

/* Set up structures needed to add the protocol subtree and manage it */
	proto_item *ti;
	proto_tree *RAPI_tree;

/* Make entries in Protocol column and Info column on summary display */
	if (check_col(pinfo->cinfo, COL_PROTOCOL)) 
		col_set_str(pinfo->cinfo, COL_PROTOCOL, "RAPI");
    
/* This field shows up as the "Info" column in the display; you should make
   it, if possible, summarize what's in the packet, so that a user looking
   at the list of packets can tell what type of packet it is. See section 1.5
   for more information.

   If you are setting it to a constant string, use "col_set_str()", as
   it's more efficient than the other "col_set_XXX()" calls.

   If you're setting it to a string you've constructed, or will be
   appending to the column later, use "col_add_str()".

   "col_add_fstr()" can be used instead of "col_add_str()"; it takes
   "printf()"-like arguments.  Don't use "col_add_fstr()" with a format
   string of "%s" - just use "col_add_str()" or "col_set_str()", as it's
   more efficient than "col_add_fstr()".

   If you will be fetching any data from the packet before filling in
   the Info column, clear that column first, in case the calls to fetch
   data from the packet throw an exception because they're fetching data
   past the end of the packet, so that the Info column doesn't have data
   left over from the previous dissector; do

	if (check_col(pinfo->cinfo, COL_INFO)) 
		col_clear(pinfo->cinfo, COL_INFO);

   */

  if (check_col(pinfo->cinfo, COL_INFO))  {
    if (pinfo->srcport == RAPI_TCP_PORT) {
      col_set_str(pinfo->cinfo, COL_INFO, "(reply)");
    }
    else {
      col_set_str(pinfo->cinfo, COL_INFO, "Windows CE Remote API call");
    }
  }

/* In the interest of speed, if "tree" is NULL, avoid building a
   protocol tree and adding stuff to it if possible.  Note,
   however, that you must call subdissectors regardless of whether
   "tree" is NULL or not. */
	/*if (tree)*/ {
    int offset = 0;

    while (tvb_length_remaining(tvb, offset) > 0) {

      /* Need at least 4 for size */
      if (tvb_length_remaining(tvb, offset) < 4) {
        pinfo->desegment_offset = offset;
        pinfo->desegment_len = 4;
        return;
      }

      proto_item *size_item;
      gint packet_size = tvb_get_letohl(tvb, offset);

#if 0
      if (packet_size > tvb_reported_length_remaining(tvb, offset)) {
        /* provide a hint to TCP where the next PDU starts */
        pinfo->want_pdu_tracking = 2;
        pinfo->bytes_until_next_pdu = packet_size - tvb_reported_length_remaining(tvb, offset);
      }
#endif

      if (tvb_length_remaining(tvb, offset) < packet_size) {
        pinfo->desegment_offset = offset;
        pinfo->desegment_len = 4 + packet_size;
        return;
      }

      /* NOTE: The offset and length values in the call to
         "proto_tree_add_item()" define what data bytes to highlight in the hex
         display window when the line in the protocol tree display
         corresponding to that item is selected.

         Supplying a length of -1 is the way to highlight all data from the
         offset to the end of the packet. */

      /* create display subtree for the protocol */
      ti = proto_tree_add_item(tree, proto_RAPI, tvb, offset, 4 + packet_size, FALSE);

      RAPI_tree = proto_item_add_subtree(ti, ett_RAPI);
      
      /* add an item to the subtree, see section 1.6 for more information */
      size_item = proto_tree_add_item(RAPI_tree,
          hf_RAPI_size, tvb, 0, 4, TRUE);
      offset += 4;

      if (packet_size < 4) {
        proto_item_append_text(size_item, " ERROR: Should be at least 4!");
        return;
      }

      if (pinfo->srcport == RAPI_TCP_PORT) {
        /* Reply package */
        guint32 protocol_error_flag = tvb_get_letohl(tvb, offset);
        proto_tree_add_item(RAPI_tree,
            hf_RAPI_protocol_error_flag, tvb, offset, 4, TRUE);
        offset += 4;
        packet_size -= 4;

        if (protocol_error_flag) {
          proto_tree_add_item(RAPI_tree,
              hf_RAPI_protocol_error_code, tvb, offset, 4, TRUE);
          offset += 4;
          packet_size -= 4;
        }
      }
      else {
        /* Command package */
        guint32 command = tvb_get_letohl(tvb, offset);
        proto_item *command_item = proto_tree_add_item(RAPI_tree,
            hf_RAPI_command, tvb, offset, 4, TRUE);
        
        if (command < array_length(rapi_command_names))
        {
          proto_item_append_text(command_item, " %s", rapi_command_names[command]);
          if (check_col(pinfo->cinfo, COL_INFO))  {
            col_add_str(pinfo->cinfo, COL_INFO, rapi_command_names[command]);
          }
        }
        else
          proto_item_append_text(command_item, " (Unknown or invalid command code)");
        
        offset += 4;
        packet_size -= 4;

        switch (command)
        {
          case 0x03: /* CeFileGetAttributes *//*{{{*/
            {
              int bytes = add_string(RAPI_tree, tvb, offset, "lpFileName");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/
            
          case 0x05: /* CeCreateFile *//*{{{*/
            {
              int bytes = add_uint32(RAPI_tree, tvb, offset, "dwDesiredAccess");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_uint32(RAPI_tree, tvb, offset, "dwShareMode");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_uint32(RAPI_tree, tvb, offset, "dwCreationDisposition");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_uint32(RAPI_tree, tvb, offset, "dwFlagsAndAttributes");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_uint32(RAPI_tree, tvb, offset, "hTemplateFile");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_string(RAPI_tree, tvb, offset, "lpFilename");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/
            
          case 0x08: /* CeCloseHandle*//*{{{*/
            {
              int bytes = add_uint32(RAPI_tree, tvb, offset, "hObject");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          case 0x09: /* CeFindAllFiles *//*{{{*/
            {
              int bytes = add_string(RAPI_tree, tvb, offset, "szPath");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_uint32(RAPI_tree, tvb, offset, "dwFlags");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          case 0x0d: /* CeCreateDatabase *//*{{{*/
            {
              unsigned i;
              int bytes = add_uint32(RAPI_tree, tvb, offset, "dwDbaseType");
              offset += bytes;
              packet_size -= bytes;

              guint16 sort_order_count = tvb_get_letohs(tvb, offset);
              bytes = add_uint16(RAPI_tree, tvb, offset, "wNumSortOrder");
              offset += bytes;
              packet_size -= bytes;

              for (i = 0; i < sort_order_count; i++) {
                bytes = add_uint32(RAPI_tree, tvb, offset, "rgSortSpecs[].propid");
                offset += bytes;
                packet_size -= bytes;
                bytes = add_uint32(RAPI_tree, tvb, offset, "rgSortSpecs[].dwFlags");
                offset += bytes;
                packet_size -= bytes;
              }

              bytes = add_string(RAPI_tree, tvb, offset, "lpszName");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          case 0x0f: /* CeDeleteDatabase *//*{{{*/
            {
              int bytes = add_uint32(RAPI_tree, tvb, offset, "oidDbase");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          case 0x14: /* CeSetDatabaseInfo *//*{{{*/
            {
              int bytes = add_uint32(RAPI_tree, tvb, offset, "oidDbase");
              offset += bytes;
              packet_size -= bytes;

              guint32 flags = tvb_get_letohl(tvb, offset);
              bytes = add_uint32(RAPI_tree, tvb, offset, "DbInfo.dwFlags");
              offset += bytes;
              packet_size -= bytes;

              if (flags & 1 /*CEDB_VALIDNAME*/)
                add_string(RAPI_tree, tvb, offset, "DbInfo.szDbaseName");
              bytes = 64; /* CEDB_MAXDBASENAMELEN * 2 */
              offset += bytes;
              packet_size -= bytes;

              if (flags & 2 /*CEDB_VALIDTYPE*/)
                add_uint32(RAPI_tree, tvb, offset, "DbInfo.dwDbaseType");
              bytes = 4;
              offset += bytes;
              packet_size -= bytes;

              /* wNumRecords not used */
              bytes = 2;
              offset += bytes;
              packet_size -= bytes;

              if (flags & 4 /*CEDB_VALIDSORTSPEC*/)
                add_uint16(RAPI_tree, tvb, offset, "DbInfo.wNumSortOrder");
              bytes = 2;
              offset += bytes;
              packet_size -= bytes;

              if (flags & 8 /*CEDB_VALIDMODTIME*/) {
                add_uint32(RAPI_tree, tvb, offset, "DbInfo.ftLastModified.dwLowDateTime");
                add_uint32(RAPI_tree, tvb, offset+4, "DbInfo.ftLastModified.dwHighDateTime");
              }
              bytes = 8;
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          case 0x1b: /* CeCopyFile *//*{{{*/
            {
              int bytes = add_string(RAPI_tree, tvb, offset, "lpExistingFileName");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_string(RAPI_tree, tvb, offset, "lpNewFileName");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_uint32(RAPI_tree, tvb, offset, "bFailIfExists");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          case 0x1e: /* CeRegOpenKeyEx *//*{{{*/
            {
              int bytes = add_uint32(RAPI_tree, tvb, offset, "hKey");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_string(RAPI_tree, tvb, offset, "lpszSubKey");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          case 0x20: /* CeRegCreateKeyEx *//*{{{*/
            {
              int bytes = add_uint32(RAPI_tree, tvb, offset, "hKey");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_string(RAPI_tree, tvb, offset, "lpszSubKey");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_string(RAPI_tree, tvb, offset, "lpszClass");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          case 0x21: /* CeRegCloseKey *//*{{{*/
            {
              int bytes = add_uint32(RAPI_tree, tvb, offset, "hKey");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          case 0x26: /* CeRegQueryValueEx *//*{{{*/
            {
              int bytes = add_uint32(RAPI_tree, tvb, offset, "hKey");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_optional_string(RAPI_tree, tvb, offset, "lpValueName");
              offset += bytes;
              packet_size -= bytes;

              /* TODO: remaining parameters when optional_uint32 is supported */
            }
            break;/*}}}*/
                     
          case 0x27: /* CeRegSetValueEx *//*{{{*/
            {
              int bytes = add_uint32(RAPI_tree, tvb, offset, "hKey");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_optional_string(RAPI_tree, tvb, offset, "lpValueName");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_uint32(RAPI_tree, tvb, offset, "dwType");
              offset += bytes;
              packet_size -= bytes;

              /* TODO: remaining parameters */
            }
            break;/*}}}*/

          case 0x1c: /* CeDeleteFile */
          case 0x2d: /* RegCopyFile *//*{{{*/
            {
              int bytes = add_optional_string(RAPI_tree, tvb, offset, "Filename");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

           case 0x56: /* CeProcessConfig *//*{{{*/
            {
              int bytes = add_optional_string(RAPI_tree, tvb, offset, "Config");
              offset += bytes;
              packet_size -= bytes;

              bytes = add_uint32(RAPI_tree, tvb, offset, "Flags");
              offset += bytes;
              packet_size -= bytes;
            }
            break;/*}}}*/

          default:
            break;
        }        
       }
    
      if (packet_size > 0) {
        const guint8 *data;

        bytes = tvb_length_remaining(tvb, offset);
        if (bytes > packet_size)
          bytes = packet_size;

        data = tvb_get_ptr(tvb, offset, bytes);
        proto_tree_add_bytes_format(RAPI_tree, hf_RAPI_data, tvb,
            offset,
            bytes,
            data,
            "Data (%i bytes)", bytes);

        offset += bytes;
        packet_size -= bytes;
      }
      
    } /* while (...) */

  } /* if (tree) */

  /* If this protocol has a sub-dissector call it here, see section 1.8 */
}


/* Register the protocol with Ethereal */

/* this format is require because a script is used to build the C function
   that calls all the protocol registration.
*/

void
proto_register_RAPI(void)
{                 

/* Setup list of header fields  See Section 1.6.1 for details*/
	static hf_register_info hf[] = {
		{ &hf_RAPI_size,
			{ "Size", "rapi.size", FT_UINT32, BASE_HEX, NULL, 0, "Size of remaining packet", HFILL }
    },
		{ &hf_RAPI_command,
			{ "Command", "rapi.command", FT_UINT32, BASE_HEX, NULL, 0, "Command code", HFILL }
		},
		{ &hf_RAPI_protocol_error_flag,
			{ "Protocol error flag", "rapi.protocol_error_flag", FT_UINT32, BASE_HEX, NULL, 0, "Protocol error flag", HFILL }
		},
		{ &hf_RAPI_protocol_error_code,
			{ "Protocol error code", "rapi.protocol_error_code", FT_UINT32, BASE_HEX, NULL, 0, "Protocol error code (HRESULT)", HFILL }
		},
		{ &hf_RAPI_data,
			{ "Data", "rapi.data", FT_BYTES, BASE_HEX, NULL, 0, "Data", HFILL }
		}
	};

/* Setup protocol subtree array */
	static gint *ett[] = {
		&ett_RAPI,
	};

/* Register the protocol name and description */
	proto_RAPI = proto_register_protocol("Windows CE Remote API",
	    "RAPI", "RAPI");

/* Required function calls to register the header fields and subtrees used */
	proto_register_field_array(proto_RAPI, hf, array_length(hf));
	proto_register_subtree_array(ett, array_length(ett));
}


/* If this dissector uses sub-dissector registration add a registration routine.
   This format is required because a script is used to find these routines and
   create the code that calls these routines.
*/
void
proto_reg_handoff_RAPI(void)
{
	dissector_handle_t RAPI_handle;

	RAPI_handle = create_dissector_handle(dissect_RAPI,
	    proto_RAPI);
	dissector_add("tcp.port", RAPI_TCP_PORT, RAPI_handle);
}


