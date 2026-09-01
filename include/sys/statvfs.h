/*
 * statvfs.h: POSIX statvfs for WinCE (libc++ <filesystem> space()).
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * COREDLL exports GetDiskFreeSpaceExW (ce 4.x/5.x/6.x all), so a real
 * statvfs is implementable; mingwex/wince/statvfs.c fills the struct
 * from it.  Only the members libc++ <filesystem> reads are given
 * defined values; the rest are zeroed.
 */
#ifndef _SYS_STATVFS_H_
#define _SYS_STATVFS_H_

#ifndef RC_INVOKED

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct statvfs
{
	unsigned long f_bsize;	/* Fundamental file system block size.  */
	unsigned long f_frsize;	/* Fragment size (block size for transfers).  */
	unsigned long long f_blocks; /* Total size of file system in f_frsize units.  */
	unsigned long long f_bfree;	/* Free blocks in f_frsize units.  */
	unsigned long long f_bavail;	/* Free blocks for unprivileged users.  */
	unsigned long f_files;	/* Total number of inodes.  */
	unsigned long f_ffree;	/* Free inodes.  */
	unsigned long f_favail;	/* Free inodes for unprivileged users.  */
	unsigned long f_fsid;	/* File system ID.  */
	unsigned long f_flag;	/* Mount flags.  */
	unsigned long f_namemax; /* Maximum length of filenames.  */
};

int __cdecl __MINGW_NOTHROW statvfs (const char *__restrict__,
				     struct statvfs *__restrict__);

#ifdef __cplusplus
}
#endif

#endif /* Not RC_INVOKED */

#endif /* _SYS_STATVFS_H_ */
