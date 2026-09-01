/*-
 * Copyright (c) 2026 mingwrt contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* _isnanl(x) - external long-double NaN test for the CE libm.
 *
 * The CE math.h maps isnan(x) on a long double argument to _isnanl(x)
 * (the x86 path uses __isnanl, whose out-of-line definition lives in
 * mingwex/math/isnanl.c; the CE path only had a __CRT_INLINE body).
 * Clang's C99 inline semantics can emit an out-of-line call to _isnanl
 * instead of inlining it (at -O0, and in the dead branch of the
 * sizeof dispatch inside the isnan macro), so a real external
 * definition is required; mingwex/stdio/pformat.c and the round
 * helpers reference it.  Architecture independent: on ARM long double
 * is 64-bit (same layout as double), and on x86 the value is first
 * narrowed, which preserves NaN-ness.
 */

#include "math.h"
#include "fpmath.h"

int
_isnanl(long double e)
{
	union IEEEd2bits u;

	u.d = (double) e;
	return (u.bits.exp == 2047 && (u.bits.manl | u.bits.manh) != 0);
}
