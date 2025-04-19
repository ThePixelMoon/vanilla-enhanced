//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#pragma warning( disable : 4786 )

#include "CritSect.h"
#include <windows.h>

using namespace Sexy;

////////////////////////////////////////////////////////////////////////////////

CritSect::CritSect(void)
{
	InitializeCriticalSection(&mCriticalSection);
}

////////////////////////////////////////////////////////////////////////////////

CritSect::~CritSect(void)
{
	DeleteCriticalSection(&mCriticalSection);
}
