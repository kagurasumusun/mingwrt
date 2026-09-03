/*
 * nanosleep.c: POSIX nanosleep for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * CE Sleep is not signal-interruptible; rem is zeroed on success.
 */
#include <time.h>
#include <errno.h>
#include <windows.h>

int
nanosleep (const struct timespec *req, struct timespec *rem)
{
  unsigned long long ms;

  if (req == NULL || req->tv_sec < 0
      || req->tv_nsec < 0 || req->tv_nsec >= 1000000000L)
    {
      errno = EINVAL;
      return -1;
    }

  ms = (unsigned long long) req->tv_sec * 1000ULL
       + ((unsigned long long) req->tv_nsec + 999999ULL) / 1000000ULL;
  if (ms > 0xffffffffULL)
    ms = 0xffffffffULL;

  Sleep ((DWORD) ms);

  if (rem)
    {
      rem->tv_sec = 0;
      rem->tv_nsec = 0;
    }
  return 0;
}
