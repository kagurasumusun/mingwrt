/*
 * This is just an RC_INVOKED guard for the real varargs.h
 * fixincluded in gcc system dir.  One day we will delete this file.
 */
#ifndef RC_INVOKED
#ifdef __COREDLL__
/* A sysroot assembled from this tree alone has no successor varargs.h:
   CeGCC's GCC does not ship one for the CE target, and clang's own
   resource-dir varargs.h is only an "#error: use <stdarg.h>" stub, so
   include_next would either pull in nothing usable or hard-fail.  Provide
   the classic interface directly on top of the compiler builtins instead;
   new code must use stdarg.h. */
#ifndef _VARARGS_H
#define _VARARGS_H
typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start((ap), (last))
#define va_end(ap)         __builtin_va_end((ap))
#define va_arg(ap, type)   __builtin_va_arg((ap), type)
#ifdef __cplusplus
# define va_copy(d, s)     __builtin_va_copy((d), (s))
#else
# define va_copy(d, s)     __builtin_va_copy((d), (s))
#endif
#define __va_copy(d, s)    __builtin_va_copy((d), (s))
#endif /* _VARARGS_H */
#else /* !__COREDLL__ */
/* A sysroot assembled from this tree has nothing after us that ships a
   varargs.h (clang's own resource varargs.h sits behind us in the search
   order and only errors anyway), so include the next header only when one
   exists.  New code must use stdarg.h.  Same pattern as direct.h. */
# if defined(__has_include_next) && __has_include_next(<varargs.h>)
#  include_next <varargs.h>
# endif
#endif /* __COREDLL__ */
#endif /* RC_INVOKED */
