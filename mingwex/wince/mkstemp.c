/*
 * mkstemp.c: POSIX mkstemp for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 */
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <windows.h>

int
mkstemp (char *tmpl)
{
  static const char digits[] = "abcdefghijklmnopqrstuvwxyz0123456789";
  char *xxxx;
  size_t len;
  DWORD seed;
  int i, fd, attempt;

  if (tmpl == NULL)
    {
      errno = EINVAL;
      return -1;
    }
  len = strlen (tmpl);
  if (len < 6)
    {
      errno = EINVAL;
      return -1;
    }
  xxxx = tmpl + len - 6;
  if (memcmp (xxxx, "XXXXXX", 6) != 0)
    {
      errno = EINVAL;
      return -1;
    }

  seed = GetTickCount () ^ GetCurrentProcessId ();
  for (attempt = 0; attempt < 256; attempt++)
    {
      DWORD x = seed + (DWORD) attempt * 1103515245u + 12345u;
      for (i = 0; i < 6; i++)
        {
          xxxx[i] = digits[x % 36];
          x /= 36;
        }
      fd = open (tmpl, O_RDWR | O_CREAT | O_EXCL, 0600);
      if (fd >= 0)
        return fd;
    }
  errno = EEXIST;
  return -1;
}
