//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __CONTINUEDIALOG_H__
#define __CONTINUEDIALOG_H__
#ifdef _WIN32
#pragma once
#endif

#include "LawnDialog.h"

class ContinueDialog : public LawnDialog
{
public:
	enum
	{
		ContinueDialog_Continue,
		ContinueDialog_NewGame
	};

public:
	DialogButton*		mContinueButton;	
	DialogButton*		mNewGameButton;		

public:
	ContinueDialog(LawnApp* theApp);
	virtual ~ContinueDialog();

	virtual int			GetPreferredHeight(int theWidth);
	virtual void		Resize(int theX, int theY, int theWidth, int theHeight);
	virtual void		AddedToManager(WidgetManager* theWidgetManager);
	virtual void		RemovedFromManager(WidgetManager* theWidgetManager);
	virtual void		ButtonDepress(int theId);
	void				RestartLoopingSounds();
};

#endif