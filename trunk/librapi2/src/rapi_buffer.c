/* $Id$ */
#include "rapi_internal.h"
#include "rapi_buffer.h"
#include "rapi_endian.h"
#include "rapi_wstr.h"
#include "rapi_log.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define RAPI_BUFFER_DEBUG 0

#if RAPI_BUFFER_DEBUG
#define rapi_buffer_trace(args...)    rapi_trace(args)
#define rapi_buffer_warning(args...)  rapi_warning(args)
#define rapi_buffer_error(args...)    rapi_error(args)
#else
#define rapi_buffer_trace(args...)
#define rapi_buffer_warning(args...)
#define rapi_buffer_error(args...)
#endif

#if HAVE_DMALLOC_H
#include "dmalloc.h"
#endif

#define RAPI_BUFFER_INITIAL_SIZE 16

struct _RapiBuffer
{
	unsigned char* data;
	size_t max_size;
	unsigned bytes_used;
	unsigned read_index;
};

/**
 * Enlarge buffer to at least a specified size
 */
static bool rapi_buffer_enlarge(RapiBuffer* buffer, size_t bytes_needed)
{
	size_t new_size = buffer->max_size;
	unsigned char* new_data = NULL;
	bool success = false;

	if (new_size == 0)
		new_size = RAPI_BUFFER_INITIAL_SIZE;
	
	while (new_size < bytes_needed)
		new_size <<= 1;
	
	rapi_buffer_trace("trying to realloc %i bytes, buffer->data=%p", new_size, buffer->data);
	
	new_data = realloc(buffer->data, new_size);
	if (new_data)
	{
		buffer->data = new_data;
		buffer->max_size = new_size;
		success = true;
	}
	else
	{
		rapi_buffer_error("realloc %i bytes failed", new_size);
	}
	
	return success;
}

/**
 * See if the buffer is large enough, try to enlarge if it is too small
 */
static bool rapi_buffer_assure_size(RapiBuffer* buffer, size_t extra_size)
{
	bool success = false;
	size_t bytes_needed = buffer->bytes_used + extra_size;

	if (bytes_needed > buffer->max_size)
	{
		success = rapi_buffer_enlarge(buffer, bytes_needed);
		if (!success)
		{
			rapi_buffer_error("failed to enlarge buffer, bytes_needed=%i\n", bytes_needed);
		}
	}
	else
	{
		success = true;
	}

	return success;
}


RapiBuffer* rapi_buffer_new()
{
	RapiBuffer* buffer = calloc(1, sizeof(RapiBuffer));
	
	return buffer;
}

void rapi_buffer_free_data(RapiBuffer* buffer)
{
	if (buffer && buffer->data)
	{
		free(buffer->data);
		memset(buffer, 0, sizeof(RapiBuffer));
	}
}

void rapi_buffer_free(RapiBuffer* buffer)
{
	if (buffer)
	{
		rapi_buffer_free_data(buffer);
		free(buffer);
	}
}

bool rapi_buffer_reset(RapiBuffer* buffer, unsigned char* data, size_t size)
{
	if (!buffer)
	{
		rapi_buffer_error("buffer is NULL");
		return false;
	}
	
	rapi_buffer_free_data(buffer);
	
	buffer->data = data;
	buffer->max_size = buffer->bytes_used = size;
	buffer->read_index = 0;

	return true;
}

size_t rapi_buffer_get_size(RapiBuffer* buffer)
{
	return buffer->bytes_used;
}

unsigned char* rapi_buffer_get_raw(RapiBuffer* buffer)
{
	return buffer->data;
}

bool rapi_buffer_write_data(RapiBuffer* buffer, const void* data, size_t size)
{
	if (!rapi_buffer_assure_size(buffer, size))
	{
		rapi_buffer_error("rapi_buffer_assure_size failed, size=%i\n", size);
		return false;
	}

	memcpy(buffer->data + buffer->bytes_used, data, size);
	buffer->bytes_used += size;

	return true;
}

bool rapi_buffer_write_uint16(RapiBuffer* buffer, u_int16_t value)
{
	u_int16_t little_endian_value = htole16(value);
	return rapi_buffer_write_data(buffer, &little_endian_value, sizeof(u_int16_t));
}

bool rapi_buffer_write_uint32(RapiBuffer* buffer, u_int32_t value)
{
	u_int32_t little_endian_value = htole32(value);
	return rapi_buffer_write_data(buffer, &little_endian_value, sizeof(u_int32_t));
}

bool rapi_buffer_write_string(RapiBuffer* buffer, LPCWSTR unicode)
{
	/*
	 * This function writes a sequence like this:
	 *
	 * Offset  Value
	 *     00  0x00000001
	 *     04  String length in number of unicode chars + 1
	 *     08  String data 
	 */
	size_t length = rapi_wstr_strlen(unicode) + 1;
	
	rapi_buffer_trace("Writing string of length %i",length);
	
	return 
		rapi_buffer_write_uint32(buffer, 1) &&
		rapi_buffer_write_uint32(buffer, length) &&
		rapi_buffer_write_data(buffer, (void*)unicode, length * sizeof(WCHAR));
}

bool rapi_buffer_write_optional_string(RapiBuffer* buffer, LPCWSTR unicode)
{
	/*
	 * This function writes a sequence like this:
	 *
	 * Offset  Value
	 *     00  0x00000001
	 *     04  String length in number of bytes + 2
	 *     08  boolean specifying if string data is sent or not
	 *     0c  String data 
	 */
	size_t size;
	
	if (unicode)
	  size = (rapi_wstr_strlen(unicode) + 1) * sizeof(WCHAR);
	else
		size = 0;
	
	return rapi_buffer_write_optional(buffer, (void*)unicode, size, true);
}

bool rapi_buffer_write_optional(RapiBuffer* buffer, void* data, size_t size, bool send_data)
{
	/* See http://sourceforge.net/mailarchive/message.php?msg_id=64440 */
	if (data)
	{
		return 
			rapi_buffer_write_uint32(buffer, 1) &&
			rapi_buffer_write_uint32(buffer, size) &&
			rapi_buffer_write_uint32(buffer, send_data) &&
			(send_data ? rapi_buffer_write_data(buffer, data, size) : true);
	}
	else
	{
		return rapi_buffer_write_uint32(buffer, 0);
	}
}

bool rapi_buffer_write_optional_in(RapiBuffer* buffer, const void* data, size_t size)
{
	if (data)
	{
		return
			rapi_buffer_write_uint32(buffer, 1) &&
			rapi_buffer_write_uint32(buffer, size) &&
			rapi_buffer_write_data(buffer, data, size);
	}
	else
	{
		return rapi_buffer_write_uint32(buffer, 0);
	}
}

bool rapi_buffer_write_optional_out(RapiBuffer* buffer, void* data, size_t size)
{
	/* See http://sourceforge.net/mailarchive/message.php?msg_id=64440 */
	if (data)
	{
		return
			rapi_buffer_write_uint32(buffer, 1) &&
			rapi_buffer_write_uint32(buffer, size) &&
			rapi_buffer_write_uint32(buffer, 0);
	}
	else
	{
		return rapi_buffer_write_uint32(buffer, 0);
	}
}

bool rapi_buffer_write_optional_inout(RapiBuffer* buffer, void* data, size_t size)
{
	/* See http://sourceforge.net/mailarchive/message.php?msg_id=64440 */
	if (data)
	{
		return 
			rapi_buffer_write_uint32(buffer, 1) &&
			rapi_buffer_write_uint32(buffer, size) &&
			rapi_buffer_write_uint32(buffer, 1) &&
			rapi_buffer_write_data(buffer, data, size);
	}
	else
	{
		return rapi_buffer_write_uint32(buffer, 0);
	}
}

bool rapi_buffer_read_data(RapiBuffer* buffer, void* data, size_t size)
{
	if (!data)
	{
		rapi_buffer_error("data is NULL");
		return false;
	}

	if (!buffer)
	{
		rapi_buffer_error("buffer is NULL");
		return false;
	}

	if ( (buffer->read_index + size) > buffer->bytes_used )
	{
		rapi_buffer_error("unable to read %i bytes. read_index=%i, bytes_used=%i", size,
				buffer->read_index, buffer->bytes_used);

		return false;
	}

	memcpy(data, buffer->data + buffer->read_index, size);
	buffer->read_index += size;

	return true;
}

bool rapi_buffer_read_uint16(RapiBuffer* buffer, u_int16_t* value)
{
	if ( !rapi_buffer_read_data(buffer, value, sizeof(u_int16_t)) )
		return false;

	*value = letoh16(*value);

	return true;
}

bool rapi_buffer_read_uint32(RapiBuffer* buffer, u_int32_t* value)
{
	if ( !rapi_buffer_read_data(buffer, value, sizeof(u_int32_t)) )
		return false;

	*value = letoh32(*value);

	return true;
}

bool rapi_buffer_read_string(RapiBuffer* buffer, LPWSTR unicode, size_t* size)
{
	u_int32_t exact_size = 0;
	
	if (!buffer || !unicode || !size)
	{
		rapi_buffer_error("bad parameter");
		return false;
	}

	if ( !rapi_buffer_read_uint32(buffer, &exact_size) )
		return false;
	rapi_buffer_trace("exact_size = %i = 0x%x", exact_size, exact_size);

	if ( exact_size > *size )
	{
		rapi_buffer_error("buffer too small (have %i bytes, need %i bytes)", *size, exact_size);
		return false;
	}

	*size = exact_size;

	if ( !rapi_buffer_read_data(buffer, unicode, (exact_size+1) * sizeof(WCHAR)) )
	{
		rapi_buffer_error("failed to read buffer");
		return false;
	}
	
	return true;
}



