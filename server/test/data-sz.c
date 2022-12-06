#include "../lib/common/all.h"

int main()
{
  PRINT_LONG(sizeof(enum ContentType));
  PRINT_LONG(sizeof(int));
  PRINT_LONG(sizeof(unsigned int));
  PRINT_LONG(sizeof(long));
  PRINT_LONG(sizeof(short));

  PRINT_LONG(sizeof(msg_header_t));
  PRINT_LONG(sizeof(msg_content_t));
  return 0;
}