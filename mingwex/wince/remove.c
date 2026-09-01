/*
 * remove.c: remove implementation for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * stdio.h declares remove() on every target; COREDLL exports neither
 * remove nor any file API under that name, so the CRT supplies it as
 * the unlink wrapper POSIX requires (libc++ <filesystem> calls
 * ::remove).
 */

#include <stdio.h>

int
remove (const char *path)
{
  extern int unlink (const char *);
  return unlink (path);
}
