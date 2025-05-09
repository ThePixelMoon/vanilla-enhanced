//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __LAWN_COMMON_H__
#define __LAWN_COMMON_H__
#ifdef _WIN32
#pragma once
#endif

#include "ConstEnums.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/EditWidget.h"

using namespace Sexy;

class Board;
class LawnStoneButton;
class NewLawnButton;
namespace Sexy
{
	class Dialog;
	class Checkbox;
	class DialogButton;
	class CheckboxListener;
}

class LawnEditWidget : public EditWidget
{
public:
	Dialog*					mDialog;
	bool					mAutoCapFirstLetter;

public:
	LawnEditWidget(int theId, EditListener* theListener, Dialog* theDialog);
	~LawnEditWidget();

	virtual void			KeyDown(KeyCode theKey);
	virtual void			KeyChar(char theChar);
};

/*inline*/ bool				ModInRange(int theNumber, int theMod, int theRange = 0);
/*inline*/ bool				GridInRange(int x1, int y1, int x2, int y2, int theRangeX = 1, int theRangeY = 1);

/*inline*/ void				TileImageHorizontally(Graphics* g, Image* theImage, int theX, int theY, int theWidth);
/*inline*/ void				TileImageVertically(Graphics* g, Image* theImage, int theX, int theY, int theHeight);

Checkbox*					MakeNewCheckbox(int theId, CheckboxListener* theListener, bool theDefault);
LawnEditWidget*				CreateEditWidget(int theId, EditListener* theListener, Dialog* theDialog);
void						DrawEditBox(Graphics* g, EditWidget* theWidget);

std::string					GetSavedGameName(GameMode theGameMode, int theProfileId);
int							GetCurrentDaysSince2000();

#endif


