/*
 * statvfs.c: POSIX statvfs for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> space() uses ::statvfs on the POSIX path.
 * COREDLL exports GetDiskFreeSpaceExW on CE 4.x/5.x/6.x, so a real
 * implementation is possible.  Only the members libc++ reads
 * (f_bsize/f_frsize, f_blocks/f_bfree/f_bavail) are defined; the
 * remainder are zeroed.
 */

#include <windows.h>
#include <sys/statvfs.h>
#include <wchar.h>
#include <errno.h>

int
statvfs (const char *path, struct statvfs *buf)
{
  wchar_t pathw[MAX_PATH + 1];
  ULARGE_INTEGER total, freeavail, freecaller;

  mbstowcs (pathw, path, MAX_PATH);

  /* GetDiskFreeSpaceExW accepts a file name or directory; a trailing
     backslash root works on CE.  Fail cleanly if the volume is not
     reachable. */
  if (!GetDiskFreeSpaceExW (pathw, &freecaller, &total, &freeavail))
    {
      errno = ENOENT;
      return -1;
    }

  memset (buf, 0, sizeof (*buf));
  buf->f_bsize = 1;			/* libc++ space() uses bytes */
  buf->f_frsize = 1;
  buf->f_blocks = (unsigned long long) (total.QuadPart);
  buf->f_bfree = (unsigned long long) (freeavail.QuadPart);
  buf->f_bavail = (unsigned long long) (freecaller.QuadPart);
  buf->f_namemax = MAX_PATH;

  return 0;
}
