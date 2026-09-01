/*
 * realpath.c: POSIX realpath for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> canonical uses ::realpath on the POSIX path.
 * COREDLL exports no path-resolution API (GetFullPathName is absent
 * from every CE 4.x/5.x/6.x def), so a path cannot be canonicalised;
 * fail honestly with ENOENT.  libc++ weakens realpath to its
 * absolute_path fallback, so std::filesystem::canonical still works.
 */

#include <stdlib.h>
#include <errno.h>

char *
realpath (const char *path, char *resolved_path)
{
  (void) path;
  (void) resolved_path;
  errno = ENOENT;
  return NULL;
}
