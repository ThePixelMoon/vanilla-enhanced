//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __ACHIVEMENTS_H__
#define __ACHIVEMENTS_H__
#ifdef _WIN32
#pragma once
#endif

#include "ConstEnums.h"
#include "SexyAppFramework/Graphics.h"

using namespace Sexy;

class LawnApp;
class Achievements
{

public:
    Achievements(LawnApp* theApp);

    LawnApp*                mApp;
    void                    GiveAchievement(AchievementType theAchivementType);
    void                    InitAchievement();
    SexyString              ReturnAchievementName(int theAchivementIndex);
    bool                    ReturnShowInAwards(int theAchivementIndex);
};

class AchievementDefinition
{
public:
    AchievementType             mAchievementType; // achievement identifier
    int                         mAchievementIconIndex; // icon index in the spritesheet
    const SexyChar*             mAchievementName; // the name used for description choosing etc
    bool                        mShowInAwards; // if it should appear in the awards menu after it was given
};

extern AchievementDefinition gAchievementDefs[NUM_ACHIEVEMENTS];

#endif