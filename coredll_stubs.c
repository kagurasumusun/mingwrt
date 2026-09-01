#include <errno.h>
#include <stddef.h>

#ifdef __COREDLL__

#include <sys/stat.h>

int* _errno()
{
  static int e = 0;
  return &e;
}

char* setlocale(int category, const char *locale)
{
  return 0;
}

long _get_osfhandle(int fd)
{
  return (long)fd;
}

/* Directory and file-metadata operations: COREDLL has no backing API for
   any of these (no CWD, no stat).  The shims report failure cleanly so
   that gnulib-based consumers (libiconv srclib, Stage 5) compile, link
   against libmingw32.a and degrade gracefully at run time instead of
   failing with implicit-declaration or unresolved-symbol errors. */

char *_getcwd(char *buf, size_t size)
{
  (void)buf;
  (void)size;
  errno = ENOENT;
  return 0;
}

int _chdir(const char *path)
{
  (void)path;
  errno = ENOENT;
  return -1;
}

/* gnulib's canonicalize-lgpl probes path components through faccessat
   (its __faccessat maps there on _WIN32). */
int faccessat(int dirfd, const char *path, int mode, int flags)
{
  (void)dirfd;
  (void)path;
  (void)mode;
  (void)flags;
  errno = ENOENT;
  return -1;
}

int _stati64(const char *path, struct _stati64 *buf)
{
  (void)path;
  (void)buf;
  errno = ENOENT;
  return -1;
}

int _fstati64(int fd, struct _stati64 *buf)
{
  (void)fd;
  (void)buf;
  errno = EBADF;
  return -1;
}

int _getmaxstdio(void)
{
  return 512;
}

intptr_t _open_osfhandle(intptr_t osfhandle, int flags)
{
  (void)osfhandle;
  (void)flags;
  errno = EMFILE;
  return -1;
}

/* Low-level fd I/O: COREDLL only offers stdio and the Win32 handle API,
   so an fd table does not exist.  The shims fail with EBADF/ENOENT. */

int _open(const char *path, int flags, ...)
{
  (void)path;
  (void)flags;
  errno = ENOENT;
  return -1;
}

int _close(int fd)
{
  (void)fd;
  errno = EBADF;
  return -1;
}

int _read(int fd, void *buf, unsigned int count)
{
  (void)fd;
  (void)buf;
  (void)count;
  errno = EBADF;
  return -1;
}

int _write(int fd, const void *buf, unsigned int count)
{
  (void)fd;
  (void)buf;
  (void)count;
  errno = EBADF;
  return -1;
}

long _lseek(int fd, long offset, int whence)
{
  (void)fd;
  (void)offset;
  (void)whence;
  errno = EBADF;
  return -1;
}

int _isatty(int fd)
{
  (void)fd;
  return 0;
}

int _dup(int fd)
{
  (void)fd;
  errno = EBADF;
  return -1;
}

int _dup2(int fd1, int fd2)
{
  (void)fd1;
  (void)fd2;
  errno = EBADF;
  return -1;
}

#endif
