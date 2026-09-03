/*
 * strndup.c: POSIX strndup for WinCE (never a COREDLL export).
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 */
#include <string.h>
#include <stdlib.h>

char *
strndup (const char *s, size_t n)
{
  size_t len;
  char *p;

  if (s == NULL)
    return NULL;
  len = strnlen (s, n);
  p = (char *) malloc (len + 1);
  if (p == NULL)
    return NULL;
  memcpy (p, s, len);
  p[len] = '\0';
  return p;
}
