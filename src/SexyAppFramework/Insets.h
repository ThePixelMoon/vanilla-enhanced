//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __INSETS_H__
#define __INSETS_H__
#ifdef _WIN32
#pragma once
#endif

namespace Sexy
{

class Insets
{
public:
	int						mLeft;
	int						mTop;
	int						mRight;
	int						mBottom;

public:
	Insets();
	Insets(int theLeft, int theTop, int theRight, int theBottom);
	Insets(const Insets& theInsets);			
};

}

#endif //__INSETS_H__