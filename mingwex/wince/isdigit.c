/* isdigit.c: isdigit implementation for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * The 2007 CeGCC wince set provides out-of-line copies of the other
 * ten ANSI ctype classifiers (isalnum.c ... isxdigit.c); isdigit.c was
 * omitted, so any non-inlined reference to isdigit (address taken, or
 * a compiler that does not inline the ctype.h fast path) had no
 * definition on CE.  Provide it here, routing through _isctype exactly
 * like its siblings.
 *
 */

#include <ctype.h>

int
isdigit (int c)
{
  return __ISCTYPE (c, _DIGIT);
}
