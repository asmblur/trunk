/* $Id$ */
#include "liborange_internal.h"
#include <synce_log.h>
#include <assert.h>
#include <stdlib.h>

#define VERBOSE 0

/*
   .apk files used by TomTom products
 */

#define BUFFER_SIZE 0x8000


static uint8_t orange_read_byte(FILE* input_file)
{
  uint8_t byte;
  if (sizeof(byte) != fread(&byte, 1, sizeof(byte), input_file))
    byte = 0;
#if VERBOSE
  fprintf(stderr, "%02x ", byte);
#endif
  return byte;
}

static bool orange_write_byte(FILE* output_file, uint8_t byte)
{
  return sizeof(byte) == fwrite(&byte, 1, sizeof(byte), output_file);
}

static void ugly_copy(FILE* output_file, size_t offset, size_t size)
{
  uint8_t* buffer = malloc(size);
  size_t bytes_copied;

#if VERBOSE
  fprintf(stderr, "Copy %08x bytes from offset %08x to offset %08lx\n",
      size, offset, ftell(output_file));
#endif

/*  fflush(output_file);*/
  fseek(output_file, offset, SEEK_SET);
  
  bytes_copied = fread(buffer, 1, size, output_file);
  
  fseek(output_file, 0, SEEK_END);

  if (size != bytes_copied)
  {
    fprintf(stderr, "Copy %08x bytes from offset %08x to offset %08lx failed\n",
        size, offset, ftell(output_file));
    abort(); 
  }
  
  
  bytes_copied = fwrite(buffer, 1, size, output_file);
  assert(size == bytes_copied);
}

bool orange_extract_arpk(
    const char* input_filename,
    const char* output_directory)
{
  bool success = false;
  FILE* input_file = fopen(input_filename, "r");
  FILE* output_file = fopen("/tmp/arpk.bin", "w+");
  size_t uncompressed_size;
  uint8_t block_start_stop;
  uint8_t current_byte;
  uint8_t* dictionary = malloc(BUFFER_SIZE);
  size_t bytes_written = 0;

  if (!dictionary)
    goto exit;
  
  if (!input_file)
    goto exit;

  if (!output_file)
    goto exit;

  if (orange_read_byte(input_file) != 'A' ||
      orange_read_byte(input_file) != 'R' ||
      orange_read_byte(input_file) != 'P' ||
      orange_read_byte(input_file) != 'K')
    goto exit;

  uncompressed_size = 
    orange_read_byte(input_file) |
    (orange_read_byte(input_file) << 8) |
    (orange_read_byte(input_file) << 16) |
    (orange_read_byte(input_file) << 24);

  synce_trace("ARPK signature found");

  synce_trace("uncompressed size: %08x (%i)", uncompressed_size, uncompressed_size);

  block_start_stop = orange_read_byte(input_file);

#if VERBOSE
  fprintf(stderr, "Block start\n");
#endif

  while (bytes_written < uncompressed_size /*0x50000*/)
  {
    unsigned count;

    current_byte = orange_read_byte(input_file);
  
    if (block_start_stop == current_byte)
    {
      unsigned offset;

#if VERBOSE
      fprintf(stderr, "Block stop (offset %08lx)\n", ftell(output_file));
#endif

      current_byte = orange_read_byte(input_file);

      if (block_start_stop == current_byte)
      {
        count = 1;
      }
      else if (current_byte <= 9)
      {
        size_t offset_bytes = current_byte % 5;
        size_t size_bytes   = current_byte / 5;

        offset = orange_read_byte(input_file);

        if (offset_bytes > 1)
          offset |= orange_read_byte(input_file) << 8;
        if (offset_bytes > 2)
          offset |= orange_read_byte(input_file) << 16;
        if (offset_bytes > 3)
          offset |= orange_read_byte(input_file) << 24;
         
        count = orange_read_byte(input_file);
        
        if (size_bytes > 0)
          count |= orange_read_byte(input_file) << 8;
        if (size_bytes > 1)
          abort();
        
        ugly_copy(output_file, offset, count);
        bytes_written += count;
        count = 0;
      }
      else
      {
        count = current_byte - 5;
        current_byte = orange_read_byte(input_file);
#if VERBOSE
        fprintf(stderr, "Byte %02x repeated %02x times at offset %08lx\n", current_byte, count, ftell(output_file));
#endif
      }
    }
    else
    {
      count = 1;
    }

    while (count--)
    {
      orange_write_byte(output_file, current_byte);
      bytes_written++;
    }
  } /* for() */

exit:
  FCLOSE(input_file);
  FCLOSE(output_file);
  return success;
}

