//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __H_CRITSECT_H__
#define __H_CRITSECT_H__
#ifdef _WIN32
#pragma once
#endif

#include "Common.h"

class CritSync;

namespace Sexy
{

class CritSect 
{
private:
	CRITICAL_SECTION mCriticalSection;
	friend class AutoCrit;

public:
	CritSect(void);
	~CritSect(void);
};

}

#endif // _H_CritSect
