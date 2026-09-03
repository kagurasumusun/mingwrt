/*
 * fsync.c: POSIX fsync for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * On COREDLL an fd is the Win32 HANDLE itself.
 */
#include <unistd.h>
#include <errno.h>
#include <windows.h>

int
fsync (int fd)
{
  if (fd == (int) INVALID_HANDLE_VALUE)
    {
      errno = EBADF;
      return -1;
    }
  if (!FlushFileBuffers ((HANDLE) fd))
    {
      errno = EIO;
      return -1;
    }
  return 0;
}
