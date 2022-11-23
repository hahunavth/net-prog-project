#include "./msg-header.h"
#include <sys/param.h>

#define NOT_ENOUGH_BUFFER_SIZE_FOR_HEADER \
  {                                       \
    perror("Buffer size too small!");     \
    exit(EXIT_FAILURE);                   \
  }

msg_header_t *create_raw_msg_header()
{
  msg_header_t *h = (msg_header_t *)calloc(1, sizeof(msg_header_t));
  return h;
}

msg_header_t *create_msg_header(int content_type, long content_len)
{
  msg_header_t *h = (msg_header_t *)calloc(1, sizeof(msg_header_t));

  h->content_type = content_type;
  h->content_len = content_len;
  h->offset = 0;
  h->body_size = 0;

  return h;
}

int encode_msg_header_partial(msg_header_t *h, char *buffer, const int buff_size, long p_offset)
{
  int header_size = sizeof(msg_header_t);
  int body_size = buff_size - header_size;

  if (body_size <= 0)
    NOT_ENOUGH_BUFFER_SIZE_FOR_HEADER;

  h->offset = p_offset;
  h->body_size = MIN(body_size, h->content_len - p_offset);
  // long new_offset = p_offset + h->body_size;

  memcpy(buffer, h, header_size);

  return h->body_size;
}

msg_header_t *decode_msg_header_partial(msg_header_t *h, char *buffer, int buff_size)
{
  if (buff_size <= sizeof(msg_header_t))
    NOT_ENOUGH_BUFFER_SIZE_FOR_HEADER;

  memcpy(h, buffer, sizeof(msg_header_t));

  return h;
}