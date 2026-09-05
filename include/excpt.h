#ifndef EXCPT_H
#define EXCPT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _EXCEPTION_DISPOSITION {
    ExceptionContinueExecution,
    ExceptionContinueSearch,
    ExceptionNestedException,
    ExceptionCollidedUnwind,
    ExceptionExecuteHandler
} EXCEPTION_DISPOSITION;

struct _CONTEXT;
typedef struct _CONTEXT CONTEXT;
struct _EXCEPTION_RECORD;
struct _EXCEPTION_POINTERS;
typedef struct _CONTEXT *PCONTEXT;
typedef struct _EXCEPTION_RECORD *PEXCEPTION_RECORD;
typedef struct _EXCEPTION_POINTERS *PEXCEPTION_POINTERS;
typedef PCONTEXT LPCONTEXT;
typedef PEXCEPTION_RECORD LPEXCEPTION_RECORD;
typedef PEXCEPTION_POINTERS LPEXCEPTION_POINTERS;
// The kernel's per-function unwind info (nkarm.h _RUNTIME_FUNCTION). The CE
// SDK only forward-declares it; define it here so the PDATA_EH handler can read
// FunctionEntry->HandlerData (the per-function table, e.g. FuncInfoB). Layout
// verified against CE 6.0 nkarm.h (HandlerData at offset 12).
typedef struct _RUNTIME_FUNCTION {
    unsigned long BeginAddress;        // function start (absolute VA)
    unsigned long EndAddress;          // function end
    void *ExceptionHandler;            // the EXCEPTION_ROUTINE (or PDATA_EH pHandler)
    void *HandlerData;                 // handler data (PDATA_EH pHandlerData, e.g. FuncInfoB)
    unsigned long PrologEndAddress;    // end of the prologue
} RUNTIME_FUNCTION, *PRUNTIME_FUNCTION;

typedef struct _DISPATCHER_CONTEXT {
    unsigned long ControlPc;
    struct _RUNTIME_FUNCTION *FunctionEntry;
    unsigned long EstablisherFrame;
    CONTEXT* ContextRecord;
} DISPATCHER_CONTEXT, *PDISPATCHER_CONTEXT;

#define EXCEPTION_EXECUTE_HANDLER        1
#define EXCEPTION_CONTINUE_SEARCH        0
#define EXCEPTION_CONTINUE_EXECUTION    -1

/* SEH filter-expression intrinsics.  These are compiler builtins under
   -fms-extensions (clang Builtins.td: ExceptionCode/ExceptionInfo/
   AbnormalTermination); prototypes only, never defined - the compiler
   implements them.  On non-x86 the outlined __except filter receives the
   EXCEPTION_POINTERS as its first argument, which _exception_info()
   returns; _exception_code() reads the code out of it.  The
   GetException* macros are the SDK spellings used inside __except()
   filter expressions - same contract as the MSVC and mingw-w64 excpt.h
   (mingwrt's top-level __try/__except crash logger depends on this). */
unsigned long _exception_code (void);
void *_exception_info (void);
int _abnormal_termination (void);

#define GetExceptionCode _exception_code
#define GetExceptionInformation() ((struct _EXCEPTION_POINTERS *) _exception_info ())
#define AbnormalTermination _abnormal_termination

#ifdef __cplusplus
}
#endif

#endif
