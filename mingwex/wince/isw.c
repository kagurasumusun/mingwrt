/* isw.c: wide-character classification functions for WinCE.
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * COREDLL exports only iswctype, towlower and towupper of the wide
 * classification surface (all three generations agree, and the real
 * CE 4.x/6.x import libraries match); the individual isw* functions
 * therefore have to be built here on top of iswctype with the
 * ctype.h masks, which is exactly the routing the MS CRT uses.
 *
 */

#include <ctype.h>

int
iswalnum (wint_t c)
{
  return iswctype (c, _ALPHA | _DIGIT);
}

int
iswalpha (wint_t c)
{
  return iswctype (c, _ALPHA);
}

int
iswascii (wint_t c)
{
  return (c & ~0x7F) == 0;
}

int
iswcntrl (wint_t c)
{
  return iswctype (c, _CONTROL);
}

int
iswdigit (wint_t c)
{
  return iswctype (c, _DIGIT);
}

int
iswgraph (wint_t c)
{
  return iswctype (c, _PUNCT | _ALPHA | _DIGIT);
}

int
iswlower (wint_t c)
{
  return iswctype (c, _LOWER);
}

int
iswprint (wint_t c)
{
  return iswctype (c, _BLANK | _PUNCT | _ALPHA | _DIGIT);
}

int
iswpunct (wint_t c)
{
  return iswctype (c, _PUNCT);
}

int
iswspace (wint_t c)
{
  return iswctype (c, _SPACE);
}

int
iswupper (wint_t c)
{
  return iswctype (c, _UPPER);
}

int
iswxdigit (wint_t c)
{
  return iswctype (c, _HEX);
}
