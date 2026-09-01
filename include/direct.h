/*
 * direct.h
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * Functions for manipulating paths and directories (included from io.h)
 * plus functions for setting the current drive.
 *
 */

#ifdef __COREDLL__
/* CeGCC pairs this header with the COREDLL runtime's own direct.h further
   down the include path.  A sysroot assembled from this tree alone has
   nothing after us, so include the next header only when one exists and
   fall through to the (deliberately empty on COREDLL) declarations below.
   Same pattern as float.h (CI 33353690290 / 33355350730); this fixed the
   libiconv srclib build (cellvm-build Stage 5). */
# if defined(__has_include_next) && __has_include_next(<direct.h>)
#  include_next <direct.h>
# endif
#else /* __COREDLL__ */

#ifndef	_DIRECT_H_
#define	_DIRECT_H_

/* All the headers include this file. */
#include <_mingw.h>

#define __need_wchar_t
#ifndef RC_INVOKED
#include <stddef.h>
#endif	/* Not RC_INVOKED */

#include <io.h>

#ifndef RC_INVOKED

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __COREDLL__

#ifndef _DISKFREE_T_DEFINED
/* needed by _getdiskfree (also in dos.h) */
struct _diskfree_t {
	unsigned total_clusters;
	unsigned avail_clusters;
	unsigned sectors_per_cluster;
	unsigned bytes_per_sector;
};
#define _DISKFREE_T_DEFINED
#endif  

/*
 * You really shouldn't be using these. Use the Win32 API functions instead.
 * However, it does make it easier to port older code.
 */
_CRTIMP int __cdecl __MINGW_NOTHROW _getdrive (void);
_CRTIMP unsigned long __cdecl __MINGW_NOTHROW _getdrives(void);
_CRTIMP int __cdecl __MINGW_NOTHROW _chdrive (int);
_CRTIMP char* __cdecl __MINGW_NOTHROW _getdcwd (int, char*, int);
_CRTIMP unsigned __cdecl __MINGW_NOTHROW _getdiskfree (unsigned, struct _diskfree_t *);

#ifndef	_NO_OLDNAMES
# define diskfree_t _diskfree_t
#endif

#ifndef _WDIRECT_DEFINED
/* wide character versions. Also in wchar.h */
#ifdef __MSVCRT__ 
_CRTIMP int __cdecl __MINGW_NOTHROW _wchdir(const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wgetcwd(wchar_t*, int);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wgetdcwd(int, wchar_t*, int);
_CRTIMP int __cdecl __MINGW_NOTHROW _wmkdir(const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW _wrmdir(const wchar_t*);
#endif	/* __MSVCRT__ */
#define _WDIRECT_DEFINED
#endif

#endif /* !__COREDLL__ */

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _DIRECT_H_ */

#endif /* Not __COREDLL__ */
