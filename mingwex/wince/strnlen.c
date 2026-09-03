/*
 * strnlen.c: POSIX strnlen for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * coredll 6.0 exports strnlen; the local definition is used on CE 4/5
 * and is a valid replacement on CE 6 if the object is pulled first.
 */
#include <string.h>

size_t
strnlen (const char *s, size_t n)
{
  size_t i;

  if (s == NULL)
    return 0;
  for (i = 0; i < n && s[i] != '\0'; i++)
    ;
  return i;
}
