/*
 * localtime_r.c: POSIX localtime_r for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * COREDLL has no thread-safe _r conversion; reuse the CE localtime
 * FILETIME offset and write into the caller buffer.
 */
#include "timeutil.h"

struct tm *
localtime_r (const time_t *timer, struct tm *result)
{
  SYSTEMTIME ss, ls, s;
  FILETIME sf, lf, f;
  long long t, diff;

  if (timer == NULL || result == NULL)
    return NULL;

  GetSystemTime (&ss);
  GetLocalTime (&ls);

  SystemTimeToFileTime (&ss, &sf);
  SystemTimeToFileTime (&ls, &lf);

  diff = __FILETIME_to_ll (&sf) - __FILETIME_to_ll (&lf);

  __time_t_to_FILETIME (*timer, &f);
  t = __FILETIME_to_ll (&f) - diff;
  __ll_to_FILETIME (t, &f);
  FileTimeToSystemTime (&f, &s);
  __SYSTEMTIME_to_tm (&s, result);

  return result;
}
