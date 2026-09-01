/*
 * fdopen.c: fdopen and _fdopen implementations for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * Written by Pedro Alves <pedro_alves@portugalmail.pt> Feb 2007
 *
 * The original implementation delegated to _wfdopen().  That is only
 * correct on Windows Mobile 6 and later: the CE 4.x/5.x shared-source
 * coredll defs have no _wf* stream functions, so an import of
 * _wfdopen makes the whole executable fail to load on CE 5.0 devices.
 * The FILE structure is opaque on this CRT (stdio.h typedefs FILE as
 * void under __COREDLL__), so there is no portable way to wrap an fd
 * into a FILE* ourselves.  Fail honestly instead: libc++ <filesystem>
 * only reaches this via basic_filebuf::__open(int), which it never
 * calls (it opens files by path), so returning NULL costs nothing in
 * that path and keeps CE 5.0 executables loadable.
 */

#include <stdio.h>

FILE *
_fdopen (int fildes, const char *mode)
{
  (void) fildes;
  (void) mode;
  return NULL;
}

FILE *
fdopen (int fildes, const char *mode)
{
  return _fdopen (fildes, mode);
}
