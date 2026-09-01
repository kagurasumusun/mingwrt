/*
 * conio.h
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * Low level console I/O functions. Pretty please try to use the ANSI
 * standard ones if you are writing new code.
 *
 */

#ifndef	_CONIO_H_
#define	_CONIO_H_

/* All the headers include this file. */
#include <_mingw.h>

#ifndef RC_INVOKED

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef __COREDLL__
/* CeGCC pairs this header with the COREDLL runtime's own conio.h further
   down the include path.  A sysroot assembled from this tree alone has
   nothing after us, so include the next header only when one exists and
   fall through to the (deliberately empty on COREDLL) declarations below.
   Same pattern as direct.h / float.h. */
# if defined(__has_include_next) && __has_include_next(<conio.h>)
#  include_next <conio.h>
# endif
#else

_CRTIMP char* __cdecl __MINGW_NOTHROW	_cgets (char*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_cprintf (const char*, ...);
_CRTIMP int __cdecl __MINGW_NOTHROW	_cputs (const char*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_cscanf (char*, ...);

_CRTIMP int __cdecl __MINGW_NOTHROW	_getch (void);
_CRTIMP int __cdecl __MINGW_NOTHROW	_getche (void);
_CRTIMP int __cdecl __MINGW_NOTHROW	_kbhit (void);
_CRTIMP int __cdecl __MINGW_NOTHROW	_putch (int);
_CRTIMP int __cdecl __MINGW_NOTHROW	_ungetch (int);

#ifndef	_NO_OLDNAMES

_CRTIMP int __cdecl __MINGW_NOTHROW	getch (void);
_CRTIMP int __cdecl __MINGW_NOTHROW	getche (void);
_CRTIMP int __cdecl __MINGW_NOTHROW	kbhit (void);
_CRTIMP int __cdecl __MINGW_NOTHROW	putch (int);
_CRTIMP int __cdecl __MINGW_NOTHROW	ungetch (int);

#endif	/* Not _NO_OLDNAMES */

#endif /* !__COREDLL__ */

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _CONIO_H_ */
