/*
 * fchmod.c: POSIX fchmod for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> copy_file uses ::fchmod on the POSIX path.  CE
 * has no fd->path conversion (GetFinalPathNameByHandle is Vista+ and
 * absent from every CE 4.x/5.x/6.x COREDLL) and no per-handle
 * attribute API (SetFileInformationByHandle is Vista+ too), so a
 * fd-based chmod cannot be implemented.  CE file systems have no
 * permissions beyond the read-only attribute, which chmod(2) (path
 * based, mingwex/wince/chmod.c) already covers; fchmod therefore
 * succeeds as a no-op, matching the "no permissions on this fs"
 * behaviour POSIX permits (e.g. FAT implementations).
 */

#include <unistd.h>

int
fchmod (int fd, mode_t mode)
{
  (void) fd;
  (void) mode;
  return 0;
}
