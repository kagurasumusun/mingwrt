/*
 * truncate.c: POSIX truncate for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> resize_file uses ::truncate on the POSIX path.
 * COREDLL has no SetEndOfFile-by-path API; open with
 * GENERIC_WRITE and call SetEndOfFile (both exported by CE 4.x/5.x/6.x
 * COREDLL).
 */

#include <windows.h>
#include <unistd.h>
#include <wchar.h>
#include <errno.h>

int
truncate (const char *path, off_t length)
{
  wchar_t pathw[MAX_PATH + 1];
  HANDLE h;
  LARGE_INTEGER li;

  if (length < 0)
    {
      errno = EINVAL;
      return -1;
    }

  mbstowcs (pathw, path, MAX_PATH);

  h = CreateFileW (pathw, GENERIC_WRITE,
		   FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (h == INVALID_HANDLE_VALUE)
    {
      errno = ENOENT;
      return -1;
    }

  li.QuadPart = length;
  if (!SetFilePointerEx (h, li, NULL, FILE_BEGIN)
      || !SetEndOfFile (h))
    {
      CloseHandle (h);
      errno = EACCES;
      return -1;
    }

  CloseHandle (h);
  return 0;
}
