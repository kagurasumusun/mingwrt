#ifndef _UNISTD_H
/*
 * This file is part of the Mingw32 package.
 *
 * unistd.h maps (roughly) to io.h
 * Other headers included by unistd.h may be selectively processed;
 * __UNISTD_H_SOURCED__ enables such selective processing.
 */
#define _UNISTD_H
#define __UNISTD_H_SOURCED__ 1

#include <io.h>
#include <process.h>
#include <getopt.h>

/* These are also defined in stdio.h. */
#ifndef	SEEK_SET
#define SEEK_SET 0
#endif

#ifndef	SEEK_CUR
#define SEEK_CUR 1
#endif

#ifndef SEEK_END
#define SEEK_END 2
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined __NO_ISOCEXT
#include <sys/types.h> /* For useconds_t. */

int __cdecl __MINGW_NOTHROW usleep(useconds_t useconds);
#endif  /* Not __NO_ISOCEXT */

/* This is defined as a real library function to allow autoconf
   to verify its existence. */
int ftruncate(int, off_t);
#ifndef __NO_INLINE__
__CRT_INLINE int ftruncate(int __fd, off_t __length)
{
  return _chsize (__fd, __length);
}
#endif

#ifndef _NO_OLDNAMES
/* POSIX file layer (libc++ <filesystem> compiles these on every
   non-Win32API target, WinCE included).  COREDLL has no symlink
   support at all, so link/symlink/readlink live in mingwex/wince and
   fail honestly with ENOSYS/ENOENT; truncate/fchmod wrap chsize/chmod. */
int link(const char*, const char*);
int symlink(const char*, const char*);
int readlink(const char*, char*, size_t);
int truncate(const char*, off_t);
int fchmod(int, mode_t);
/* Path configuration and *at() family (POSIX.1-2008); libc++ <filesystem>
   operations.cpp uses these.  mingwex/wince implements them with the
   plain functions (CE has no dir-fd or symlink support). */
long pathconf(const char*, int);
int openat(int, const char*, int, ...);
int fchmodat(int, const char*, mode_t, int);
int unlinkat(int, const char*, int);
#endif /* _NO_OLDNAMES */

#ifdef __COREDLL__
/* pathconf name constants (POSIX.1-2008). */
#define _PC_LINK_MAX 0
#define _PC_PATH_MAX 1
#define _PC_NAME_MAX 2
#endif

#ifdef __cplusplus
}
#endif

#undef __UNISTD_H_SOURCED__
#endif /* _UNISTD_H */
