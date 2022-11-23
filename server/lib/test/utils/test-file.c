#include "../../utils/file.h"
#include "../base/utils.h"

#define FILE_NAME "./a.txt"
//
#define INIT                        \
  FILE *f = fopen(FILE_NAME, "w+"); \
  if (f == NULL)                    \
  {                                 \
    perror(FILE_NAME);              \
    return 0;                       \
  }

#define FINALIZE \
  fclose(f);

#define TEST(I, FN) \
  printf("Test %s: ", #I, #FN);
//

int test1();
int test2();
int test3();

int main()
{
  printf("Test 1: %d\n", test1());
  printf("Test 2: %d\n", test2());
  printf("Test 2: %d\n", test3());
  // TEST(1, test1);

  // CHECK_ECHO(NULL);

  return 0;
}

int test1()
{
  INIT;

  char *msg = "Hello 1";
  f_add(f, msg);
  fseek(f, 0, SEEK_SET);
  char s[1024];
  fgets(s, 1024, f);

  FINALIZE;
  return strcmp(msg, s);
}

int test2()
{
  INIT;

  f_add(f, "Hello 1");
  fseek(f, 0, SEEK_SET);
  f_delete(f, "Hello 1");

  fseek(f, 0, SEEK_END);
  long f_sz = ftell(f);

  FINALIZE;
  return f_sz;
}

int test3()
{
  INIT;

  f_add(f, "Hello 1");
  fseek(f, 0, SEEK_SET);
  f_replace(f, "Hello 1", "ABC");

  fseek(f, 0, SEEK_END);
  long f_sz = ftell(f);

  FINALIZE;
  return !(f_sz == 3);
}