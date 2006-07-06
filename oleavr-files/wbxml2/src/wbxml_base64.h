/*
 * libwbxml, the WBXML Library.
 * Copyright (C) 2002-2004 Aymerick J�hanne <aymerick@jehanne.org>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 * GPL v2: http://www.gnu.org/licenses/gpl.txt
 * 
 * Contact: libwbxml@aymerick.com
 * Home: http://libwbxml.aymerick.com
 */
 
/**
 * @file wbxml_base64.h 
 * @ingroup wbxml_base64
 *
 * @author Aymerick J�hanne <libwbxml@aymerick.com>
 * @date 01/11/03
 *
 * @brief Base64 encoding/decoding functions
 *
 * @note Code adapted from APR library (http://apr.apache.org/) 
 */

#ifndef WBXML_BASE64_H
#define WBXML_BASE64_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @addtogroup wbxml_base64 
 *  @{ 
 */

/**
 * @brief Encode a buffer to Base64
 * @param buffer The buffer to encode
 * @param len    Buffer length
 * @return The new base64 encoded Buffer (must be freed by caller), or NULL if not enought memory
 */
WBXML_DECLARE(WB_UTINY *) wbxml_base64_encode(const WB_UTINY *buffer, WB_LONG len);

/**
 * @brief Decode a Base64 encoded buffer
 * @param buffer The buffer to decode
 * @param result Resulting decoded buffer
 * @return Length of resulting decoded buffer ('0' if no decoded)
 * @note Be aware that if return value is '0', then 'result' param will be NULL, else 'result' param
 *       has to be freed by caller.
 */
WBXML_DECLARE(WB_LONG) wbxml_base64_decode(const WB_UTINY *buffer, WB_UTINY **result);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WBXML_BASE64_H */
