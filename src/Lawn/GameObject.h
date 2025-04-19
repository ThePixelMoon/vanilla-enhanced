//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#ifdef _WIN32
#pragma once
#endif

#include "ConstEnums.h"
#include "SexyAppFramework/Graphics.h"

using namespace Sexy;

class LawnApp;
class Board;

class GameObject
{
public:
	LawnApp*                        mApp;
	Board*                          mBoard;
    int                             mX;
    int                             mY;
    int                             mWidth;
    int                             mHeight;
    bool                            mVisible;
    int                             mRow;
    int                             mRenderOrder;

public:
    /*inline*/                      GameObject();
    /*inline*/ bool                 BeginDraw(Graphics* g);
    /*inline*/ void                 EndDraw(Graphics* g);
    /*inline*/ void                 MakeParentGraphicsFrame(Graphics* g);
};

#endif