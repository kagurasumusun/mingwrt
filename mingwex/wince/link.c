/*
 * link.c: POSIX link for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> create_hard_link uses ::link on the POSIX path.
 * COREDLL exports neither CreateHardLinkW nor any hard-link API on
 * CE 4.x/5.x/6.x, so fail honestly with ENOSYS (the feature does not
 * exist); libc++ maps the error to std::errc::not_supported.
 */

#include <unistd.h>
#include <errno.h>

int
link (const char *oldpath, const char *newpath)
{
  (void) oldpath;
  (void) newpath;
  errno = ENOSYS;
  return -1;
}
