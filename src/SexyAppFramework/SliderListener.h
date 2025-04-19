//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __SLIDERLISTENER_H__
#define __SLIDERLISTENER_H__
#ifdef _WIN32
#pragma once
#endif

namespace Sexy
{

class SliderListener
{
public:
	virtual void			SliderVal(int theId, double theVal) {};
};

}

#endif //__SLIDERLISTENER_H__
