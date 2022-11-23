#include "../../common/all.h"
#include "./str.h"

int encode_content_str(
    char *large_msg,
    char *body,
    msg_header_t *h)
{
  strncpy(body, large_msg + h->offset, h->body_size);
  body[h->body_size] = '\0';
  return 0;
}

int decode_content_str(
    char *large_msg,
    char *body,
    msg_header_t *h)
{
  char i = h->offset;
  strncpy(large_msg + i, body, h->body_size);
  large_msg[h->offset + h->body_size] = '\0';
  return 0;
}
