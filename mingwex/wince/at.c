/*
 * at.c: POSIX *at() family for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * libc++ <filesystem> remove_all() uses openat/fdopendir/unlinkat, and
 * canonicalize (gnulib) probes faccessat.  CE has no directory-fd
 * support at all, so:
 *   - openat: only AT_FDCWD is meaningful; any other dirfd fails with
 *     EBADF.  The mode argument is passed through for O_CREAT.
 *   - fdopendir: the fd is the Win32 handle; convert via the handle
 *     and opendir on the (unobtainable) path -- COREDLL cannot name a
 *     handle, so this fails honestly with ENOTSUP.  remove_all()
 *     handles that failure by falling back to the plain path path.
 *   - unlinkat: AT_FDCWD + flags 0 maps to unlink(); AT_REMOVEDIR maps
 *     to rmdir(); AT_SYMLINK_NOFOLLOW is a no-op (no symlinks).
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h> /* NULL */
#include <errno.h>
#include <dirent.h>

int
openat (int dirfd, const char *path, int flags, ...)
{
  mode_t mode = 0;
  va_list ap;

  if (dirfd != AT_FDCWD)
    {
      errno = EBADF;
      return -1;
    }

  if (flags & O_CREAT)
    {
      va_start (ap, flags);
      mode = va_arg (ap, mode_t);
      va_end (ap);
    }

  return open (path, flags, mode);
}

DIR *
fdopendir (int fd)
{
  /* COREDLL cannot convert a handle to a path, so a dir stream cannot
     be opened from an fd.  libc++ remove_all() treats this failure as
     "not a directory" and falls back to the path-based removal. */
  (void) fd;
  errno = ENOTSUP;
  return NULL;
}

int
fchmodat (int dirfd, const char *path, mode_t mode, int flags)
{
  if (dirfd != AT_FDCWD)
    {
      errno = EBADF;
      return -1;
    }
  /* AT_SYMLINK_NOFOLLOW is meaningless: CE has no symlinks, and
     chmod (path based) is the only mode changer available. */
  (void) flags;
  return chmod (path, mode);
}

int
unlinkat (int dirfd, const char *path, int flags)
{
  if (dirfd != AT_FDCWD)
    {
      errno = EBADF;
      return -1;
    }

  if (flags & AT_REMOVEDIR)
    return rmdir (path);

  return unlink (path);
}
