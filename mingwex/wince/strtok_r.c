/*
 * strtok_r.c: POSIX strtok_r for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 */
#include <string.h>

char *
strtok_r (char *s, const char *delim, char **save_ptr)
{
  char *end;

  if (save_ptr == NULL || delim == NULL)
    return NULL;
  if (s == NULL)
    s = *save_ptr;
  if (s == NULL)
    return NULL;

  s += strspn (s, delim);
  if (*s == '\0')
    {
      *save_ptr = s;
      return NULL;
    }
  end = s + strcspn (s, delim);
  if (*end == '\0')
    {
      *save_ptr = end;
      return s;
    }
  *end = '\0';
  *save_ptr = end + 1;
  return s;
}
