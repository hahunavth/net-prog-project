#include "../../lib/common/all.h"

#define MAX_LEN 10000
#define BUFF_SIZE 64

char *msg = "1-2-3-4-5-6-7-8-9-a-b-c-d-e-f-h-h-i-k-l-m-o-p-q-r-s-t-u-v-w-x-y-z";
char recv_msg[MAX_LEN];

int main()
{
  // encode
  msg_header_t *h = create_msg_header(CT_MSG, strlen(msg));
  char send_buff[BUFF_SIZE];

  // decode
  msg_header_t *recv_h = create_raw_msg_header();
  char recv_msg[8192];
  char recv_buff[BUFF_SIZE];

  for (h->offset = 0; h->offset < h->content_len; h->offset += h->body_size)
  {
    encode_msg_header_partial(h, send_buff, BUFF_SIZE, h->offset);
    encode_content_str(msg, send_buff, h);

    PRINT_STR(send_buff);
    strcpy(recv_buff, send_buff);

    decode_msg_header_partial(recv_h, recv_buff, BUFF_SIZE);
    decode_content_str(recv_msg, recv_buff, h);
  }
  PRINT_INT(strcmp(msg, recv_msg));
  return strcmp(msg, recv_msg);
}