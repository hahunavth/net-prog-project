#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <string.h>

#define PRINT_LOCATION \
  printf("\tat( %s:%d )\n", __FILE__, __LINE__);

#define PRINT_INT(n) \
  printf("> Int: %s=%d at( %s:%d )\n", #n, n, __FILE__, __LINE__)

#define PRINT_LONG(n) \
  printf("> Long: %s=%ld at( %s:%d )\n", #n, n, __FILE__, __LINE__)

#define PRINT_STR(s) \
  CATCH_NULL_PTR(s); \
  printf("> Str: %s[%ld]=\"%s\" at( %s:%d )\n", #s, strlen(s), s, __FILE__, __LINE__)

#endif