#include <stdio.h>
#include "./string.h"
#include <stdlib.h>

int trim(char str[])
{
  int i = 0;
  int j = 0;
  do
  {
    while (str[j] == '\r' || str[j] == '\n')
    {
      j++;
    }
    str[i++] = str[j++];
  } while (str[j - 1] != '\0');
}

char *trailing_l(const char *str)
{
  int len = strlen(str);
  char *new_str;
  if (str[len - 1] == '\n')
  {
    new_str = (char *)calloc((len + 1), sizeof(char));
    strcpy(new_str, str);
  }
  else
  {
    new_str = (char *)calloc((len + 2), sizeof(char));
    strcpy(new_str, str);
    new_str[len] = '\n';
  }
  return new_str;
}