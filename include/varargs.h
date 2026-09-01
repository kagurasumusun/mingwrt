/*
 * This is just an RC_INVOKED guard for the real varargs.h
 * fixincluded in gcc system dir.  One day we will delete this file.
 */
#ifndef RC_INVOKED
/* A sysroot assembled from this tree has nothing after us that ships a
   varargs.h (clang's own resource varargs.h sits behind us in the search
   order and only errors anyway), so include the next header only when one
   exists.  New code must use stdarg.h.  Same pattern as direct.h. */
# if defined(__has_include_next) && __has_include_next(<varargs.h>)
#  include_next <varargs.h>
# endif
#endif
