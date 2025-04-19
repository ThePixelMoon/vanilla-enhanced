//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __D3D8HELPER_H__
#define __D3D8HELPER_H__
#ifdef _WIN32
#pragma once

#include <Windows.h>
#endif

#include <string>

namespace Sexy
{
	bool GetD3D8AdapterInfo(GUID &theGUID, std::string &theDriver, std::string &theDescription);
}

#endif