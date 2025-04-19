//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __DIALOGLISTENER_H__
#define __DIALOGLISTENER_H__
#ifdef _WIN32
#pragma once
#endif

namespace Sexy
{

class DialogListener
{
public:
	virtual void			DialogButtonPress(int theDialogId, int theButtonId) {}
	virtual void			DialogButtonDepress(int theDialogId, int theButtonId) {}
};

}

#endif // __DIALOGLISTENER_H__