//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __NATIVEDISPLAY_H__
#define __NATIVEDISPLAY_H__
#ifdef _WIN32
#pragma once
#endif

#include "Common.h"

namespace Sexy
{

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class NativeDisplay
{
public:
	int						mRGBBits;
	ulong					mRedMask;
	ulong					mGreenMask;
	ulong					mBlueMask;
	int						mRedBits;
	int						mGreenBits;
	int						mBlueBits;
	int						mRedShift;
	int						mGreenShift;
	int						mBlueShift;

public:
	NativeDisplay();
	virtual ~NativeDisplay();
};

};


#endif