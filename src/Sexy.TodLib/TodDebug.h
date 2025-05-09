//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __TODDEBUG_H__
#define __TODDEBUG_H__
#ifdef _WIN32
#pragma once

#include <Windows.h>
#endif

class TodHesitationBracket
{
public:
	char			mMessage[256];
	int				mBracketStartTime;

public:
	TodHesitationBracket(const char* theFormat, ...) { ; }
	~TodHesitationBracket() { ; }

	inline void		EndBracket() { ; }
};

void				TodLog(const char* theFormat, ...);
void				TodLogString(const char* theMsg);
void				TodTrace(const char* theFormat, ...);
void				TodTraceMemory();
void				TodTraceAndLog(const char* theFormat, ...);
void				TodTraceWithoutSpamming(const char* theFormat, ...);
void				TodHesitationTrace(...);
void				TodReportError(LPEXCEPTION_POINTERS exceptioninfo, const char* theMessage);
void				TodAssertFailed(const char* theCondition, const char* theFile, int theLine, const char* theMsg = "", ...);
/*inline*/ void		TodErrorMessageBox(const char* theMessage, const char* theTitle);
long __stdcall		TodUnhandledExceptionFilter(LPEXCEPTION_POINTERS exceptioninfo);

/*inline*/ void*	TodMalloc(int theSize);
/*inline*/ void		TodFree(void* theBlock);
void				TodAssertInitForApp();

extern void (*gBetaSubmitFunc)();

// Moon: i hate those warnings
#pragma warning(disable : 4003)

#ifdef _DEBUG
#define TOD_ASSERT(condition, ...) { \
    if (!bool(condition)) { \
        TodAssertFailed(""#condition, __FILE__, __LINE__, ##__VA_ARGS__); \
        if (IsDebuggerPresent()) { __debugbreak(); } \
        TodTraceMemory(); \
    } \
}
#else
#define TOD_ASSERT(condition, ...)
#endif

#endif
