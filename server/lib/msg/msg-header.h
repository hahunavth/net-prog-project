#ifndef __MSG_HEADER_H__
#define __MSG_HEADER_H__

#include "../common/std.h"

/*
 * @brief ${brief}
 *
 * Created on Wed Nov 16 2022
 * Copyright (c) 2022 Vu Thanh Ha
 */
// CONTENT TYPE
#define CT_FILE 0x00
#define CT_MSG 0x01
#define CT_END 0x02
#define CT_REGISTER 0x05
#define CT_EXIT 0x06

typedef struct
{
  int content_type;
  //
  long content_len;
  //
  long offset;
  long body_size;
} msg_header_t;

extern msg_header_t *create_raw_msg_header();
extern msg_header_t *create_msg_header(int content_type, long content_len);
extern int encode_msg_header_partial(msg_header_t *h, char *buffer, const int buff_size, long p_offset);
extern msg_header_t *decode_msg_header_partial(msg_header_t *h, char *buffer, int buff_size);

#endif