//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __CURSORWIDGET_H__
#define __CURSORWIDGET_H__
#ifdef _WIN32
#pragma once
#endif

#include "Widget.h"
#include "Point.h"
#include "ConstEnums.h"

namespace Sexy
{

class Image;

class CursorWidget : public Widget
{
public:
	Image*					mImage;
	CursorMode				mCursorMode;

public:
	CursorWidget();

	virtual void			Draw(Graphics* g);
	void					SetImage(Image* theImage);
	void					Update();
	Point					GetHotspot();
	
};

}

#endif //__CURSORWIDGET_H__