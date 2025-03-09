#include "appcrashreport.h"
#include <signal.h>

#include <windows.h>
#include <imagehlp.h>

#include <stdlib.h>
#include <stdio.h>

#include <QDebug>


QString globalAppName;

//COPYPASTE shitty place

/* Resolve symbol name and source location given the path to the executable
   and an address */
int addr2line(char const * const program_name, void const * const addr)
{
  char addr2line_cmd[512] = {0};

  /* have addr2line map the address to the relent line in the code */
   sprintf(addr2line_cmd,"addr2line -f -p -e %.256s %p", program_name, addr);

  /* This will print a nicely formatted string specifying the
     function and source line of the address */
  return system(addr2line_cmd);
}

void windows_print_stacktrace(const char *appName, CONTEXT* context)
{
  SymInitialize(GetCurrentProcess(), 0, true);

  STACKFRAME frame = { 0 };

  /* setup initial stack frame */
  frame.AddrPC.Offset         = context->Eip;
  frame.AddrPC.Mode           = AddrModeFlat;
  frame.AddrStack.Offset      = context->Esp;
  frame.AddrStack.Mode        = AddrModeFlat;
  frame.AddrFrame.Offset      = context->Ebp;
  frame.AddrFrame.Mode        = AddrModeFlat;

  while (StackWalk(IMAGE_FILE_MACHINE_I386 ,
                   GetCurrentProcess(),
                   GetCurrentThread(),
                   &frame,
                   context,
                   0,
                   SymFunctionTableAccess,
                   SymGetModuleBase,
                   0 ) )
  {
    addr2line(appName, (void*)frame.AddrPC.Offset); //
  }

  SymCleanup( GetCurrentProcess() );
}


AppCrashReport::AppCrashReport(QString appName):appName(appName)
{
    addAwaitingSignal(SIGSEGV);
    addAwaitingSignal(SIGTERM);
    addAwaitingSignal(SIGINT);
    addAwaitingSignal(SIGILL);
    addAwaitingSignal(SIGFPE);
    addAwaitingSignal(SIGABRT);

    setSignalHandlers();

    globalAppName = appName;
}

void localSignalHandler(int signal) //yet hard coded, but bettet make as a pointer to a function, to make possible set another, or even chain
{
    //In any case first just try to collect call stack

   qDebug() << "Signal catched " <<signal;
    // windows_print_stacktrace();
}

 void AppCrashReport::setSignalHandlers()
 {
    for (int i = 0; i < waitingSignals.size(); ++i)
    {
        signal(waitingSignals[i],localSignalHandler);

        qDebug() << "Setting handler for "<< waitingSignals[i];
    }
 }

 void AppCrashReport::addAwaitingSignal(int signal)
 {
    waitingSignals << signal;
 }
