/*
 * clock_gettime.c: POSIX clock_gettime / clock_getres for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * CLOCK_REALTIME  - SYSTEMTIME via FILETIME (100 ns ticks).
 * CLOCK_MONOTONIC - QueryPerformanceCounter.
 */
#include <time.h>
#include <errno.h>
#include <windows.h>

#define _W32_FT_OFFSET (116444736000000000ULL)

int
clock_gettime (clockid_t clk_id, struct timespec *tp)
{
  if (tp == NULL)
    {
      errno = EINVAL;
      return -1;
    }

  if (clk_id == CLOCK_REALTIME)
    {
      union {
        unsigned long long ns100;
        FILETIME ft;
      } now;
      SYSTEMTIME st;
      unsigned long long unix100;

      GetSystemTime (&st);
      SystemTimeToFileTime (&st, &now.ft);
      unix100 = now.ns100 - _W32_FT_OFFSET;
      tp->tv_sec = (time_t) (unix100 / 10000000ULL);
      tp->tv_nsec = (long) ((unix100 % 10000000ULL) * 100ULL);
      return 0;
    }

  if (clk_id == CLOCK_MONOTONIC)
    {
      LARGE_INTEGER freq, ctr;
      unsigned long long nsec;

      if (!QueryPerformanceFrequency (&freq) || freq.QuadPart == 0
          || !QueryPerformanceCounter (&ctr))
        {
          DWORD ms = GetTickCount ();
          tp->tv_sec = (time_t) (ms / 1000);
          tp->tv_nsec = (long) ((ms % 1000) * 1000000L);
          return 0;
        }
      nsec = (unsigned long long) ctr.QuadPart * 1000000000ULL
             / (unsigned long long) freq.QuadPart;
      tp->tv_sec = (time_t) (nsec / 1000000000ULL);
      tp->tv_nsec = (long) (nsec % 1000000000ULL);
      return 0;
    }

  errno = EINVAL;
  return -1;
}

int
clock_getres (clockid_t clk_id, struct timespec *res)
{
  if (res == NULL)
    {
      errno = EINVAL;
      return -1;
    }

  if (clk_id == CLOCK_REALTIME)
    {
      res->tv_sec = 0;
      res->tv_nsec = 1000000L; /* SYSTEMTIME millisecond */
      return 0;
    }

  if (clk_id == CLOCK_MONOTONIC)
    {
      LARGE_INTEGER freq;
      if (QueryPerformanceFrequency (&freq) && freq.QuadPart > 0)
        {
          res->tv_sec = 0;
          res->tv_nsec = (long) (1000000000ULL / (unsigned long long) freq.QuadPart);
          if (res->tv_nsec == 0)
            res->tv_nsec = 1;
          return 0;
        }
      res->tv_sec = 0;
      res->tv_nsec = 1000000L;
      return 0;
    }

  errno = EINVAL;
  return -1;
}
