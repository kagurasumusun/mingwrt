/*
 * readlink.c: POSIX readlink for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> read_symlink uses ::readlink on the POSIX path.
 * COREDLL has no symlink support on CE 4.x/5.x/6.x, so fail honestly
 * with EINVAL (not a symlink); libc++ maps that to
 * std::errc::invalid_argument.
 */

#include <unistd.h>
#include <errno.h>

int
readlink (const char *path, char *buf, size_t bufsiz)
{
  (void) path;
  (void) buf;
  (void) bufsiz;
  errno = EINVAL;
  return -1;
}
