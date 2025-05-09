//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __NEWUSERDIALOG_H__
#define __NEWUSERDIALOG_H__
#ifdef _WIN32
#pragma once
#endif

#include "LawnDialog.h"
#include "SexyAppFramework/EditListener.h"

class NewUserDialog : public LawnDialog, public EditListener
{
public:
	LawnApp*			mApp;					
	EditWidget*			mNameEditWidget;		

public:
	NewUserDialog(LawnApp* theApp, bool isRename);
	virtual ~NewUserDialog();

	virtual int			GetPreferredHeight(int theWidth);
	virtual void		Resize(int theX, int theY, int theWidth, int theHeight);
	virtual void		AddedToManager(WidgetManager* theWidgetManager);
	virtual void		RemovedFromManager(WidgetManager* theWidgetManager);
	virtual void		Draw(Graphics* g);
	virtual void		EditWidgetText(int theId, const SexyString& theString);
	virtual bool		AllowChar(int, SexyChar theChar);
	SexyString			GetName();
	void				SetName(const SexyString& theName);
};

#endif
