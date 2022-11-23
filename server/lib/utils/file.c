#include "./string.h"
#include "file.h"

// get file size
long f_get_size(FILE *f)
{
  fseek(f, 0, SEEK_END);
  long f_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  return f_size;
}

long f_find_str(FILE *f, const char *match_str)
{
  char s1[1024];
  char s2[1024];
  strcpy(s1, match_str);

  while (!feof(f))
  {
    long pos = ftell(f);
    fgets(s2, 1024, f);
    trim(s1);
    trim(s2);
    if (strcmp(s1, s2) == 0)
      return pos; // matched line start
  }
  return -1;
}

int f_move_text(FILE *f, int start, int offset, int f_size)
{
  if (offset > 0)
  {
    // ftruncate(fileno(f), f_size);
    long i = f_size - 1;
    while (i >= start)
    {
      fseek(f, i, SEEK_SET);
      char c = fgetc(f);
      // fseek(f, -1, SEEK_CUR);
      // fputc('?', f);
      fseek(f, i + offset, SEEK_SET);
      fputc(c, f);
      // printf("%ld -> %ld: %c\n", i, i + offset, c);
      i--;
    }
    return 0;
  }
  else if (offset < 0)
  {
    long i = start;
    while (i - offset <= f_size - 1)
    {
      fseek(f, i - offset, SEEK_SET);
      char c = fgetc(f);
      fseek(f, i, SEEK_SET);
      fputc(c, f);
      // printf("%ld -> %ld: %c\n", i - offset, i, c);
      i++;
    }
    ftruncate(fileno(f), f_size + offset);
    return 0;
  }
  return -1;
}

int f_replace_(FILE *f, int pos, int prev_len, const char *replace)
{
  long f_size = f_get_size(f);
  int next_len = strlen(replace);

  int diff = (next_len - prev_len);
  if (diff == 0)
    return -1;
  //
  if (f_move_text(f, pos, diff, f_size) == 0)
  {
    fseek(f, pos, SEEK_SET);
    fputs(replace, f);
  }
  // fseek(f, pos, SEEK_SET);
  // fputs("???", f);

  return 0;
}

int f_replace(FILE *f, const char *match, const char *replace)
{
  char *_match = trailing_l(match);
  char *_replace = trailing_l(replace);
  trim(_match);
  trim(_replace);
  long pos = f_find_str(f, _match);
  if (pos == -1)
  {
    free(_match);
    free(_replace);
    return -1;
  }
  return f_replace_(f, pos, strlen(_match), _replace);
}

int f_delete(FILE *f, const char *match)
{
  long pos = f_find_str(f, match);
  long next_line = ftell(f);
  if (fgetc(f) == EOF && pos != 0)
  {
    return f_move_text(f, pos - 1, -(next_line - pos) - 1, f_get_size(f));
  }
  return f_move_text(f, pos, -(next_line - pos), f_get_size(f));
}

int f_add(FILE *f, const char *item)
{
  char *s = trailing_l(item);
  trim(s);
  fseek(f, 0, SEEK_END);

  long pos = ftell(f);
  if (pos != 0)
    fputc('\n', f);
  fputs(s, f);
  return 0;
}