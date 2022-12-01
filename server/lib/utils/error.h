#ifndef __ERROR_H__
#define __ERROR_H__

#include "./debug.h"
#include "./color.h"
#include <errno.h>

#define PRINT_ERROR                     \
  if (errno)                            \
  {                                     \
    perror("\033[0;41mError\033[0m\t"); \
  }

#define CATCH_STDERR                   \
  if (errno)                           \
  {                                    \
    perror(" \033[0;41mError\033[0m"); \
    PRINT_LOCATION;                    \
    return -1;                         \
  }

#define CATCH_NULL_PTR(ptr)                       \
  if (!(ptr))                                     \
  {                                               \
    PRINT_ERROR;                                  \
    printf("%sE:%s", C_ERROR, C_COLOR_RESET);     \
    printf(" \tUnexpected NULL value: %s", #ptr); \
    printf("\n");                                 \
    PRINT_LOCATION;                               \
  }

#endif