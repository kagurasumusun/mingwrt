/*
 * lseeki64.c: 64-bit seek for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * io.h declares _lseeki64() under __MSVCRT__; on COREDLL the fd *is*
 * the Win32 handle, so SetFilePointerEx (present in CE 4.x/5.x/6.x
 * COREDLL) gives the 64-bit seek directly.
 */

#include <windows.h>
#include <io.h>

__int64
_lseeki64 (int fd, __int64 offset, int whence)
{
  LARGE_INTEGER li;
  DWORD mode;

  switch (whence)
    {
    case SEEK_SET:
      mode = FILE_BEGIN;
      break;
    case SEEK_CUR:
      mode = FILE_CURRENT;
      break;
    case SEEK_END:
      mode = FILE_END;
      break;
    default:
      mode = (DWORD) -1;
    }

  li.QuadPart = offset;
  if (!SetFilePointerEx ((HANDLE) fd, li, &li, mode))
    return -1;
  return li.QuadPart;
}
