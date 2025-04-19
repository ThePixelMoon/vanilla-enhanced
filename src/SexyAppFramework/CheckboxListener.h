//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __CHECKBOX_LISTENER__
#define __CHECKBOX_LISTENER__
#ifdef _WIN32
#pragma once
#endif

namespace Sexy
{

class CheckboxListener
{
public:
	virtual void			CheckboxChecked(int theId, bool checked) {}
};

}

#endif //__CHECKBOX_LISTENER__