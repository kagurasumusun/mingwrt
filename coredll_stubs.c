#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>

#ifdef __COREDLL__

#include <sys/stat.h>
#include <windows.h>

/* COREDLL has no _errno export, so the CRT must provide errno storage.
   Use a per-thread slot rather than a single static int: a shared
   errno would corrupt across threads in any multithreaded CE program.
   The slot comes from TlsAlloc (which kfuncs.h maps to the CE-SDK
   TlsCall(TLS_FUNCALLOC, 0) idiom, present on every CE generation) and
   the 4-byte cell is LocalAlloc'ed on first use per thread; like the
   other static buffers in this file it is never freed. */
int* _errno(void)
{
  static DWORD slot = (DWORD)-1;   /* TLS_OUT_OF_INDEXES */
  int *p;

  if (slot == (DWORD)-1)
    slot = TlsAlloc ();
  p = (int *) TlsGetValue (slot);
  if (p == NULL)
    {
      p = (int *) LocalAlloc (LPTR, sizeof (int));
      if (p == NULL)
        {
          static int oom;
          return &oom;
        }
      TlsSetValue (slot, p);
    }
  return p;
}

char* setlocale(int category, const char *locale)
{
  return 0;
}

long _get_osfhandle(int fd)
{
  return (long)fd;
}

/* Directory and CWD operations: COREDLL has no working-directory or
   path-resolution API, so _getcwd/_chdir report failure cleanly.
   (File *metadata* is real on CE: mingwex/wince/stat.c implements
   _stat/_fstat, and _stati64/_fstati64 wrap them below.)  The shims
   let gnulib-based consumers (libiconv srclib, Stage 5) compile, link
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

/* POSIX names: libc++ <filesystem> (posix_compat.h) does
   `using ::chdir; using ::getcwd;` and operations.cpp calls
   detail::chdir()/detail::getcwd().  COREDLL has no CWD API, so these
   fail cleanly exactly like the _-prefixed versions above. */
char *getcwd(char *buf, size_t size)
{
  (void)buf;
  (void)size;
  errno = ENOENT;
  return 0;
}

int chdir(const char *path)
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

int _getmaxstdio(void)
{
  return 512;
}

int _setmaxstdio(int newmax)
{
  (void)newmax;
  return 512;
}

/* On the CE CRT an fd *is* the Win32 handle (mingwex/wince/open.c
   returns the CreateFileW handle as the fd, and read/write/lseek use
   it directly as a HANDLE).  _get_osfhandle/_open_osfhandle are
   therefore the identity on the handle value, exactly the inverse of
   each other; flags cannot be applied retroactively and are ignored. */
intptr_t _open_osfhandle(intptr_t osfhandle, int flags)
{
  (void)flags;
  return osfhandle;
}

/* _stati64/_fstati64 mirror _stat/_fstat (mingwex/wince/stat.c) with a
   widened st_size; the other fields have identical layouts. */
int _stati64(const char *path, struct _stati64 *buf)
{
  struct _stat st;

  if (_stat(path, &st) == -1)
    return -1;
  buf->st_dev = st.st_dev;
  buf->st_ino = st.st_ino;
  buf->st_mode = st.st_mode;
  buf->st_nlink = st.st_nlink;
  buf->st_uid = st.st_uid;
  buf->st_gid = st.st_gid;
  buf->st_rdev = st.st_rdev;
  buf->st_size = (__int64) st.st_size;
  buf->st_atime = st.st_atime;
  buf->st_mtime = st.st_mtime;
  buf->st_ctime = st.st_ctime;
  return 0;
}

int _fstati64(int fd, struct _stati64 *buf)
{
  struct _stat st;

  if (_fstat(fd, &st) == -1)
    return -1;
  buf->st_dev = st.st_dev;
  buf->st_ino = st.st_ino;
  buf->st_mode = st.st_mode;
  buf->st_nlink = st.st_nlink;
  buf->st_uid = st.st_uid;
  buf->st_gid = st.st_gid;
  buf->st_rdev = st.st_rdev;
  buf->st_size = (__int64) st.st_size;
  buf->st_atime = st.st_atime;
  buf->st_mtime = st.st_mtime;
  buf->st_ctime = st.st_ctime;
  return 0;
}

/* The low-level fd surface (_open/_close/_read/_write/_lseek and their
   POSIX spellings) is implemented for real by
   mingwex/wince/{open,close,read,write,lseek}.c (Win32-handle based);
   EBADF shims here would shadow those implementations for the
   underscored names and duplicate their symbols when both spellings
   are referenced, so none are defined in this file. */

int _isatty(int fd)
{
  (void)fd;
  return 0;
}

/* fds are Win32 handles on the CE CRT, so dup/dup2 are handle
   duplication.  _dup returns the new fd (the newly allocated handle
   value); _dup2 follows the MSVCRT convention of returning 0 on
   success, which gnulib's ms_windows_dup2 relies on. */
int _dup(int fd)
{
  HANDLE h;

  if (!DuplicateHandle(GetCurrentProcess(), (HANDLE) fd,
                       GetCurrentProcess(), &h, 0, FALSE,
                       DUPLICATE_SAME_ACCESS))
    {
      errno = EBADF;
      return -1;
    }
  return (int) h;
}

int _dup2(int fd1, int fd2)
{
  HANDLE h;
  int i;

  if (fd1 == fd2)
    return 0;
  if (fd2 < 0)
    {
      errno = EBADF;
      return -1;
    }

  /* Free the target handle value so it can be allocated. */
  CloseHandle((HANDLE) fd2);

  /* Windows hands out the lowest free handle value; repeatedly
     duplicate fd1 and close the low-valued handles (skipping fd1
     itself) until the duplicate lands exactly on fd2. */
  for (i = 0; i <= fd2; i++)
    {
      if (!DuplicateHandle(GetCurrentProcess(), (HANDLE) fd1,
                           GetCurrentProcess(), &h, 0, FALSE,
                           DUPLICATE_SAME_ACCESS))
        {
          errno = EBADF;
          return -1;
        }
      if ((int) h == fd2)
        return 0;
      CloseHandle(h);
      if (i != fd1)
        CloseHandle((HANDLE) i);
    }
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

DWORD WINAPI GetModuleFileNameA(HINSTANCE hModule, LPSTR lpFilename,
                                DWORD nSize)
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
DWORD WINAPI GetFileType(HANDLE hFile)
{
  (void)hFile;
  return 0;
}

/* COREDLL exports LoadLibraryW only (every CE 4.x/5.x/6.x def and every real
   device SDK import lib agrees); the gnulib code that probes for
   optional desktop DLLs calls LoadLibraryA, so fail cleanly with a NULL
   module handle, which its callers check.  GetProcAddressW *is* a real
   COREDLL export (mapped from the plain GetProcAddress name by
   winbase.h, exactly like the Microsoft SDKs), so it deliberately has
   no shim here. */
HMODULE WINAPI LoadLibraryA(LPCSTR lpLibFileName)
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
