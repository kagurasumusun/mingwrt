#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>

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

int _setmaxstdio(int newmax)
{
  (void)newmax;
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

/* COREDLL has no strerror, signal machinery, or process/module queries
   used by gnulib's error/sigprocmask/raise/progreloc/dup2/fcntl (libiconv
   srclib, Stage 5).  The shims below keep those references resolvable and
   degrade the way the platform actually behaves: signals do not exist
   and the program path is unavailable.  */

char *strerror(int errnum)
{
  static char buf[32];
  sprintf(buf, "Unknown error %d", errnum);
  return buf;
}

__p_sig_fn_t __cdecl signal(int sig, __p_sig_fn_t handler)
{
  (void)sig;
  (void)handler;
  errno = EINVAL;
  return SIG_ERR;
}

int __cdecl raise(int sig)
{
  (void)sig;
  errno = EINVAL;
  return -1;
}

unsigned long __cdecl GetModuleFileNameA(void *hModule, char *lpFilename,
                                         unsigned long nSize)
{
  (void)hModule;
  if (nSize > 0 && lpFilename)
    lpFilename[0] = 0;
  return 0;
}

/* gnulib's stat-w32 (libiconv srclib) calls GetFileType on every stat to
   route between handle-based and path-based probing; COREDLL does not
   export it.  FILE_TYPE_UNKNOWN (0) sends callers down their path-based
   fallback, which is what the shims above serve. */
unsigned long __cdecl GetFileType(void *hFile)
{
  (void)hFile;
  return 0;
}

/* COREDLL exports LoadLibraryW only (every CE 3.0-8 def and every real
   device SDK import lib agrees); the gnulib code that probes for
   optional desktop DLLs calls LoadLibraryA, so fail cleanly with a NULL
   module handle, which its callers check.  GetProcAddressW *is* a real
   COREDLL export (mapped from the plain GetProcAddress name by
   winbase.h, exactly like the Microsoft SDKs), so it deliberately has
   no shim here. */
void * __cdecl LoadLibraryA(const char *lpLibFileName)
{
  (void)lpLibFileName;
  return 0;
}

/* libc++'s <fstream> uses the POSIX spellings; CE has 32-bit offsets. */

void setbuf(FILE *f, char *buf)
{
  setvbuf(f, buf, buf != 0 ? _IOFBF : _IONBF, BUFSIZ);
}

int fseeko(FILE *f, off_t offset, int whence)
{
  return fseek(f, (long)offset, whence);
}

off_t ftello(FILE *f)
{
  return (off_t)ftell(f);
}

#endif
