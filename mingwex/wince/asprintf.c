/*
 * asprintf.c: GNU asprintf / vasprintf for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int
vasprintf (char **strp, const char *fmt, va_list ap)
{
  va_list aq;
  int n;
  char *p;

  if (strp == NULL || fmt == NULL)
    return -1;

  va_copy (aq, ap);
  n = vsnprintf (NULL, 0, fmt, aq);
  va_end (aq);
  if (n < 0)
    return -1;

  p = (char *) malloc ((size_t) n + 1);
  if (p == NULL)
    return -1;
  n = vsnprintf (p, (size_t) n + 1, fmt, ap);
  if (n < 0)
    {
      free (p);
      return -1;
    }
  *strp = p;
  return n;
}

int
asprintf (char **strp, const char *fmt, ...)
{
  va_list ap;
  int n;

  va_start (ap, fmt);
  n = vasprintf (strp, fmt, ap);
  va_end (ap);
  return n;
}
