#ifndef __ERROR_H__
#define __ERROR_H__

#include "./debug.h"
#include <errno.h>

#define PRINT_ERROR \
  if (errno)        \
    perror("Error\t");

#define CATCH_STDERR  \
  if (errno)          \
  {                   \
    perror(" Error"); \
    PRINT_LOCATION;   \
    return -1;        \
  }

#define CATCH_NULL_PTR(ptr)                           \
  if (!(ptr))                                         \
  {                                                   \
    PRINT_ERROR;                                      \
    printf("E\t: Unexpected NULL value: %s\n", #ptr); \
    PRINT_LOCATION;                                   \
  }

#endif