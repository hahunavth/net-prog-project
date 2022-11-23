#ifndef __MSG_CONTENT_H__
#define __MSG_CONTENT_H__

#include "./body/str.h"
#include "./body/file.h"

typedef union
{
  char *buffer;
  msg_file_content_t *file;
} msg_content_t;

#endif