/*
 * utimes.c: POSIX utimes for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> __last_write_time uses ::utimes on the POSIX
 * path.  COREDLL has no SetFileTime-by-path API, so open the file
 * with FILE_WRITE_ATTRIBUTES and call SetFileTime (exported by CE
 * 4.x/5.x/6.x COREDLL).  A NULL times argument means "set both to the
 * current time" per POSIX; CE cannot read the current time to
 * sub-second precision, so it is approximated with GetSystemTime.
 */

#include <windows.h>
#include <sys/time.h>
#include <wchar.h>
#include <errno.h>

int
utimes (const char *path, const struct timeval times[2])
{
  wchar_t pathw[MAX_PATH + 1];
  HANDLE h;
  FILETIME ft_access, ft_modify;
  SYSTEMTIME st;

  mbstowcs (pathw, path, MAX_PATH);

  h = CreateFileW (pathw, FILE_WRITE_ATTRIBUTES,
		   FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (h == INVALID_HANDLE_VALUE)
    {
      errno = ENOENT;
      return -1;
    }

  if (times == NULL)
    {
      GetSystemTime (&st);
      SystemTimeToFileTime (&st, &ft_access);
      ft_modify = ft_access;
    }
  else
    {
      /* timeval has no sub-second granularity problem here: convert
	 seconds since epoch to FILETIME (100ns since 1601). */
      LONGLONG ll;

      ll = ((LONGLONG) times[0].tv_sec + 11644473600LL) * 10000000LL;
      ft_access.dwLowDateTime = (DWORD) ll;
      ft_access.dwHighDateTime = (DWORD) (ll >> 32);

      ll = ((LONGLONG) times[1].tv_sec + 11644473600LL) * 10000000LL;
      ft_modify.dwLowDateTime = (DWORD) ll;
      ft_modify.dwHighDateTime = (DWORD) (ll >> 32);
    }

  if (!SetFileTime (h, NULL, &ft_access, &ft_modify))
    {
      CloseHandle (h);
      errno = EACCES;
      return -1;
    }

  CloseHandle (h);
  return 0;
}
