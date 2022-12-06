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
enum ContentType
{
  CT_FILE,
  CT_MSG,
  CT_END,
  CT_REGISTER,
  CT_EXIT
};

typedef struct
{
  enum ContentType content_type;
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