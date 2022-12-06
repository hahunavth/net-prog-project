#include "../../lib/common/all.h"
#include "../../lib/msg/msg-header.h"

int main()
{
  int BUFF_SIZE = 48;
  char buff[BUFF_SIZE + 1];

  char *msg = "1-2-3-4-5-6-7-8-9-a-b-c-d-e-f-h-h-i-k-l-m-o-p-q-r-s-t-u-v-w-x-y-z";
  int msg_len = strlen(msg);
  msg_header_t *h = create_msg_header(CT_MSG, msg_len);
  //
  char recv_msg[10000];
  msg_header_t *recv_h = create_raw_msg_header();

  char *body = buff + sizeof(msg_header_t);
  int BODY_SIZE = BUFF_SIZE - sizeof(msg_header_t);
  for (h->offset = 0; h->offset < h->content_len; h->offset += h->body_size)
  {
    encode_msg_header_partial(h, buff, BUFF_SIZE, h->offset);
    // strncpy(body, msg + h->offset, h->body_size);
    // body[h->body_size] = '\0';
    encode_content_str(msg, body, h);

    printf("%ld:%ld: %s\n", h->offset, h->body_size, body);
    // PRINT_STR(buff + 8);

    // decode
    recv_h = decode_msg_header_partial(recv_h, buff, BUFF_SIZE);
    // PRINT_LONG(recv_h->body_size);
    decode_content_str(recv_msg, body, h);
    // if (rmi + recv_h->body_size > 100000)
    //   break;
    // strncpy(recv_msg + rmi, body, recv_h->body_size);
    // rmi += recv_h->body_size;
    // recv_msg[rmi] = '\0';
  }

  PRINT_STR(recv_msg);

  return 0;
}