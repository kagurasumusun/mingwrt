/*
 * gmtime_r.c: POSIX gmtime_r for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 */
#include "timeutil.h"

struct tm *
gmtime_r (const time_t *t, struct tm *result)
{
  FILETIME f;
  SYSTEMTIME s;

  if (t == NULL || result == NULL)
    return NULL;

  __time_t_to_FILETIME (*t, &f);
  FileTimeToSystemTime (&f, &s);
  __SYSTEMTIME_to_tm (&s, result);
  return result;
}
