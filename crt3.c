/*
 * crt1_ce.c
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * Source code for the startup proceedures used by all programs on a
 * wince system. This code is compiled to make crt1.o, which should be
 * located in the library path.
 *
 */

/* Hide the declaration of _fmode with dllimport attribute in stdlib.h to
   avoid problems with older GCC. */
#define __IN_MINGW_RUNTIME 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <float.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <excpt.h>
#include <wince_cxx_eh.h>

extern void __gccmain ();
extern void _pei386_runtime_relocator (void);

/* No atexit on coredll, we must initialize our private version.  */
BOOL __atexit_init(void);

void __cdecl _cexit(void);

/*
 * WinCE-specific: top-level crash diagnostic (added cellvm-build /
 * kagurasumusun 2026-09-03; see llvm-project's
 * utils/wince/WINEH-ABI-FACTS.md 4h and WINCE-HANDOFF.md 19).
 *
 * Background: on this target, the CE kernel only dispatches a hardware
 * fault (data/prefetch abort, illegal instruction) or a software
 * exception to a handler if the faulting function's .pdata entry has the
 * ExceptionFlag bit set, which only happens for functions that use an
 * explicit __try/__except/__finally (see WINEH-ABI-FACTS.md 4g). Ordinary
 * application code (essentially all of it, in practice - none of
 * EasyRPG/SDL/liblcf/pixman/libpng/zlib use MS __try) has no such entry,
 * so a hardware fault anywhere in that code has, until now, had nowhere
 * to go: the process simply disappears with no diagnostic, which matches
 * the "runs briefly, then dies" symptom being investigated. This does
 * NOT change whether the fault is recoverable (it still isn't - the
 * handler below always terminates the process) but it DOES turn a silent
 * death into a one-line log with the exception code and faulting
 * address, which is the minimum needed to tell a hardware fault (e.g. an
 * ARMv5TE unaligned-access data abort) apart from a C++ exception with no
 * unwind coverage, or an ordinary abort()/ExitProcess() call.
 *
 * This wraps the *entire* WinMain call (i.e. the entire application,
 * whether it defines its own WinMain or only main() via winmain_ce.o) in
 * a single top-level __try/__except, exactly the "unhandled exception
 * filter" idiom SEH is designed for. It cannot itself be broken by the
 * open EHABI-vs-CE-native-.pdata questions in WINEH-ABI-FACTS.md, because
 * __try/__except takes the WinCFI/.pdata path (WINEH-ABI-FACTS.md 4d),
 * not the .ARM.exidx path - it is the one EH mechanism on this target
 * that the CE kernel's own fault dispatcher (exdsptch.c) natively
 * understands.
 *
 * Written source-level against the implemented __try/__except support
 * (clang/test/CodeGen/wince-seh.c); verified at the first real
 * build+device run, like the rest of this toolchain's SEH support.
 */
static void
__wince_crashlog_write (EXCEPTION_POINTERS *ep)
{
  wchar_t modpathW[MAX_PATH];
  char path[MAX_PATH];
  char *dot;
  FILE *f;
  DWORD code = 0;
  DWORD nparams = 0;
  void *addr = NULL;
  unsigned i;

  if (ep != NULL && ep->ExceptionRecord != NULL)
    {
      code = ep->ExceptionRecord->ExceptionCode;
      addr = ep->ExceptionRecord->ExceptionAddress;
      nparams = ep->ExceptionRecord->NumberParameters;
    }

  /* Log next to the executable: <exe-dir>\<exe-name-without-ext>.crash.log.
     Root of the object store / the exe's own directory is writable on
     every CE configuration this toolchain targets (unlike \Windows). */
  modpathW[0] = 0;
  if (!GetModuleFileNameW (NULL, modpathW,
			    sizeof (modpathW) / sizeof (modpathW[0])))
    return;
  path[0] = 0;
  wcstombs (path, modpathW, sizeof (path) - 1);
  path[sizeof (path) - 1] = '\0';
  dot = strrchr (path, '.');
  if (dot != NULL && strlen (dot) + strlen (".crash.log") < 16)
    strcpy (dot, ".crash.log");
  else
    strncat (path, ".crash.log", sizeof (path) - strlen (path) - 1);

  f = fopen (path, "w");
  if (f == NULL)
    return;

  fprintf (f,
	   "unhandled exception - process terminating\r\n"
	   "  ExceptionCode    = 0x%08lX\r\n"
	   "  ExceptionAddress = 0x%08lX\r\n"
	   "  NumberParameters = %lu\r\n",
	   (unsigned long) code, (unsigned long) (DWORD_PTR) addr,
	   (unsigned long) nparams);
  if (ep != NULL && ep->ExceptionRecord != NULL)
    for (i = 0; i < nparams && i < EXCEPTION_MAXIMUM_PARAMETERS; ++i)
      fprintf (f, "  ExceptionInformation[%u] = 0x%08lX\r\n", i,
	       (unsigned long) ep->ExceptionRecord->ExceptionInformation[i]);
  fclose (f);
}

/*
 * CE6: a top-level VEH (vectored exception handler), registered once at
 * startup (see WinMainCRTStartup) so that a hardware fault raised on ANY
 * thread of the process is logged - not just faults in the WinMain call
 * chain.  A VEH runs before the kernel's frame-based (.pdata) dispatch,
 * so it sees the fault even when the faulting function carries no .pdata
 * ExceptionFlag (the common case: ordinary application code with no
 * __try/__except).  It is process-global (covers every thread) and
 * therefore strictly broader than the __try wrapping below.
 *
 * It deliberately skips C++ exceptions (WINCE_CXX_EH_NUMBER, see
 * <wince_cxx_eh.h>): those are controlled exceptions that the C++ frame
 * handler (Unit 3) or a user catch(...) is meant to consume, so logging
 * every throw/catch would drown the log in noise.  A C++ exception that
 * is genuinely *unhandled* still terminates the process via the __try
 * backstop on the throwing thread (the one place that path is guaranteed
 * to be present in this toolchain).
 *
 * The handler always returns EXCEPTION_CONTINUE_SEARCH: it only *logs*,
 * it never resumes or swallows the fault.  Termination is left to the
 * normal dispatch (frame handler / __try backstop), so a C++ frame that
 * *does* catch the fault is not disturbed.
 *
 * __wince_crashlog_logged de-duplicates: if both the VEH (any thread) and
 * the __try backstop (main thread) would fire for the same fault, only
 * the first one writes the log.  A plain volatile flag is used on
 * purpose - at this point (a fatal fault) a benign race is acceptable and
 * pulling in Interlocked* from a VEH is the one thing the platform docs
 * warn against.
 */
static volatile LONG __wince_crashlog_logged = 0;

#if (_WIN32_WCE >= 0x0600)
static LONG WINAPI
__wince_crashlog_veh (PEXCEPTION_POINTERS ep)
{
  if (ep == NULL || ep->ExceptionRecord == NULL)
    return EXCEPTION_CONTINUE_SEARCH;
  if (ep->ExceptionRecord->ExceptionCode == WINCE_CXX_EH_NUMBER)
    return EXCEPTION_CONTINUE_SEARCH; /* C++ exception: leave it to the frame handler */
  if (!__wince_crashlog_logged)
    {
      __wince_crashlog_logged = 1;
      __wince_crashlog_write (ep);
    }
  return EXCEPTION_CONTINUE_SEARCH; /* let the frame dispatch / __try backstop terminate */
}
#endif /* _WIN32_WCE >= 0x0600 */

/* Filter: logs (unless the VEH already did) and always executes the
   handler (there is nothing safe to resume into after an unexpected
   exception this high up the call stack).  Kept as a separate helper -
   rather than inlining the fprintf calls directly in the __except()
   filter expression - only to keep the outlined filter clang generates
   small and easy to read in a disassembly if this itself needs
   debugging later.  On CE4/5 (no VEH API) this __except is the ONLY
   top-level crash log; on CE6 the VEH is primary and this is the
   backstop (e.g. for an unhandled C++ exception, which the VEH skips). */
static int
__wince_crashlog_filter (EXCEPTION_POINTERS *ep)
{
  if (!__wince_crashlog_logged)
    __wince_crashlog_write (ep);
  return EXCEPTION_EXECUTE_HANDLER;
}

/*
 * This function is called from the entry point for all programs.
 */
void
WinMainCRTStartup (HINSTANCE hInst, HINSTANCE hPrevInst,
                   LPWSTR lpCmdLine, int nCmdShow)
{
  int nRet;

#if (_WIN32_WCE >= 0x0600)
  /*
   * CE6: install the top-level VEH first, before anything else runs, so
   * that a hardware fault on any thread (and even one during CRT start-up
   * itself) is logged.  First=1 puts it at the head of the VEH list.  The
   * handle is intentionally not retained: the VEH lives for the process
   * lifetime, which is all a crash logger needs.  CE4/5 have no VEH API
   * (AddVectoredExceptionHandler is CE6-only, see coredll6.def): there the
   * __try/__except wrapping below is the only top-level crash log.
   */
  AddVectoredExceptionHandler (1, __wince_crashlog_veh);
#endif /* _WIN32_WCE >= 0x0600 */

  /*
   * Initialize floating point unit.
   */
  _fpreset ();			/* Supplied by the runtime library. */

   /* Adust references to dllimported data that have non-zero offsets.  */
  _pei386_runtime_relocator ();

  /*
   * Initialize the atexit table.
   */
  __atexit_init();

  /* From libgcc.a, __main calls global class constructors,
     __do_global_ctors, which registers __do_global_dtors as the first
     entry of the private atexit table we have just initialised  */
  __gccmain();

  /*
   * Call the main function. If the user does not supply one the one
   * in the 'libmingw32.a' library will be linked in, and that one
   * calls main. See winmain_ce.c in the 'lib' dir for more details.
   *
   * Wrapped in __try/__except (see __wince_crashlog_write above): this
   * is the single choke point for every WinCE program built by this
   * toolchain, so it is the one place a top-level crash log can be
   * added that covers both WinMain- and main()-style applications
   * without touching every application's own source.
   */
  __try
    {
      nRet = WinMain (hInst, hPrevInst, lpCmdLine, nCmdShow);
    }
  __except (__wince_crashlog_filter (GetExceptionInformation ()))
    {
      nRet = -1;
    }

  /*
   * Perform exit processing for the C library. This means
   * flushing output and calling 'atexit' registered functions.
   */
  _cexit ();

  ExitProcess (nRet);
}
