/*
 * lseeki64.c: 64-bit seek for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * io.h declares _lseeki64() under __COREDLL__.  COREDLL exports
 * SetFilePointer but NOT SetFilePointerEx (checked against the CE
 * 4.x/5.x/6.x export lists), so the 64-bit move is done through
 * SetFilePointer's lpDistanceToMoveHigh out-parameter.  The fd *is*
 * the Win32 handle on this CRT (mingwex/wince/open.c).
 */

#include <windows.h>
#include <io.h>

__int64
_lseeki64 (int fd, __int64 offset, int whence)
{
  DWORD mode;
  LONG hi = (LONG) (offset >> 32);
  DWORD lo;

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

  lo = SetFilePointer ((HANDLE) fd, (LONG) (offset & 0xFFFFFFFF), &hi, mode);
  if (lo == INVALID_SET_FILE_POINTER && GetLastError () != NO_ERROR)
    return -1;
  return ((__int64) hi << 32) | lo;
}
