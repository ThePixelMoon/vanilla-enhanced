//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __SEXY_DIRECTXERRORSTRING_H__
#define __SEXY_DIRECTXERRORSTRING_H__
#ifdef _WIN32
#pragma once
#endif

#include <string>
#include <ddraw.h>

namespace Sexy
{
	std::string GetDirectXErrorString(HRESULT theResult);
} // namespace 

#endif