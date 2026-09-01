/*
 * pathconf.c: POSIX pathconf for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> current_path() calls ::pathconf(".", _PC_PATH_MAX)
 * to size the getcwd buffer.  CE has no per-filesystem path limits, so
 * return fixed values: PATH_MAX for _PC_PATH_MAX, and -1 with no
 * errno (meaning "no limit") for the rest, per POSIX.
 */

#include <unistd.h>
#include <limits.h>
#include <errno.h>

long
pathconf (const char *path, int name)
{
  (void) path;
  switch (name)
    {
    case _PC_PATH_MAX:
      return (long) PATH_MAX;
    case _PC_NAME_MAX:
      return 255;
    default:
      /* No limit; errno stays untouched (POSIX: -1 without errno
	 means "no limit"). */
      return -1;
    }
}
