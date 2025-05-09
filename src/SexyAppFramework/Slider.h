//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __SLIDER_H__
#define __SLIDER_H__
#ifdef _WIN32
#pragma once
#endif

#include "Widget.h"

namespace Sexy
{

class SliderListener;

class Slider : public Widget
{
public:		
	SliderListener*			mListener;
	double					mVal;
	int						mId;
	Image*					mTrackImage;
	Image*					mThumbImage;

	bool					mDragging;
	int						mRelX;
	int						mRelY;

	bool					mHorizontal;
	int						mThumbOffsetX;
	bool					mNoDraw;

public:
	Slider(Image* theTrackImage, Image* theThumbImage, int theId, SliderListener* theListener);

	virtual void			SetValue(double theValue);

	virtual bool			HasTransparencies();
	virtual void			Draw(Graphics* g);	
	void					SliderDraw(Graphics* g);

	virtual void			MouseMove(int x, int y);
	virtual void			MouseDown(int x, int y, int theClickCount);
	virtual void			MouseDrag(int x, int y);
	virtual void			MouseUp(int x, int y);
	virtual void			MouseLeave();
};

}

#endif //__SLIDER_H__