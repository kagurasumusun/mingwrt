/*
 * symlink.c: POSIX symlink for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> create_symlink uses ::symlink on the POSIX
 * path.  COREDLL exports no symlink API on CE 4.x/5.x/6.x, so fail
 * honestly with ENOSYS; libc++ maps the error to
 * std::errc::not_supported.
 */

#include <unistd.h>
#include <errno.h>

int
symlink (const char *target, const char *linkpath)
{
  (void) target;
  (void) linkpath;
  errno = ENOSYS;
  return -1;
}
