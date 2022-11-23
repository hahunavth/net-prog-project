#ifndef __MSG_BODY_STR_H__
#define __MSG_BODY_STR_H__

#include "../msg-header.h"

extern int encode_content_str(
    char *large_msg,
    char *body,
    msg_header_t *h);

extern int decode_content_str(
    char *large_msg,
    char *body,
    msg_header_t *h);

#endif
