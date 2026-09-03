/*
 * sysconf.c: POSIX sysconf for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 */
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <windows.h>

long
sysconf (int name)
{
  SYSTEM_INFO si;

  switch (name)
    {
    case _SC_PAGESIZE:
      GetSystemInfo (&si);
      return (long) si.dwPageSize;
    case _SC_NPROCESSORS_ONLN:
      GetSystemInfo (&si);
      return (long) si.dwNumberOfProcessors;
    case _SC_CLK_TCK:
      return (long) CLOCKS_PER_SEC;
    default:
      errno = EINVAL;
      return -1;
    }
}
