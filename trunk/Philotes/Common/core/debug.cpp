///PHILOTES Source Code.  (C)2012 PhiloLabs

#include "core/types.h"
#include "util/string.h"

static void Error(const char* error)
{
    #ifdef _DEBUG
    OutputDebugString(error);
    #endif
    ::MessageBox(NULL, error, "PHILOTES ERROR  Orz !!", MB_OK|MB_APPLMODAL|MB_SETFOREGROUND|MB_TOPMOST|MB_ICONERROR);
    abort();
}


//------------------------------------------------------------------------------
/**
    This function is called by ph_assert() when the assertion fails.
*/
void 
n_barf(const char* exp, const char* file, int line)
{
	Util::String msg;
	msg.Format("*** PHILOTES ASSERTION ***\nexpression: %s\nfile: %s\nline: %d\n", exp, file, line);
    Error(msg.AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    This function is called by ph_assert2() when the assertion fails.
*/
void
n_barf2(const char* exp, const char* msg, const char* file, int line)
{
	Util::String m;
	m.Format("*** PHILOTES ASSERTION ***\nexpression: %s\nfile: %s\nline: %d\n", exp, file, line);
    Error(m.AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    This function is called when a serious situation is encountered which
    requires abortion of the application.
*/
void __cdecl
n_error(const char* msg, ...)
{
    va_list argList;
    va_start(argList, msg);
    
    Util::String str;
    str.FormatArgList(msg, argList);
    Error(str.AsCharPtr());

	va_end(argList);
}

//------------------------------------------------------------------------------
/**
    This function is called when a warning should be issued which doesn't
    require abortion of the application.
*/
void __cdecl
n_warning(const char* msg, ...)
{
    va_list argList;
    va_start(argList, msg);
    
    Util::String str;
    str.FormatArgList(msg, argList);
    MessageBox(NULL,str.AsCharPtr(),"Philotes",MB_OK);

    va_end(argList);
}        

//------------------------------------------------------------------------------
/**
    This function is called when a message should be displayed to the
    user which requires a confirmation (usually displayed as a MessageBox).
*/
void __cdecl
n_confirm(const char* msg, ...)
{
	va_list argList;
	va_start(argList, msg);

	Util::String str;
	str.FormatArgList(msg, argList);
	MessageBox(NULL,str.AsCharPtr(),"Philotes",MB_OK);

	va_end(argList);
}

//------------------------------------------------------------------------------
/**
    Nebula's printf replacement. Will redirect the text to the console
    and/or logfile.

     - 27-Nov-98   floh    created
*/
void __cdecl
n_printf(const char *msg, ...)
{
    va_list argList;
    va_start(argList, msg);
    
	Util::String str;
	str.FormatArgList(msg, argList);

	printf(str.AsCharPtr());

    va_end(argList);
}

//------------------------------------------------------------------------------
/**
    Put the message to the debug window.

    - 26-Mar-05    kims    created
*/
void __cdecl
n_dbgout(const char *msg, ...)
{
    va_list argList;
    va_start(argList,msg);

	Util::String str;
	str.FormatArgList(msg, argList);

	OutputDebugString(str.AsCharPtr());

    va_end(argList);
}

//------------------------------------------------------------------------------
/**
    Put process to sleep.

     - 21-Dec-98   floh    created
*/
void 
n_sleep(double sec)
{
	int milliSecs = int(sec * 1000.0);
	::Sleep(milliSecs);
}
