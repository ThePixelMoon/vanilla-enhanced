//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#include "Lawn/Board.h"
#include "Lawn/Zombie.h"
#include "GameButton.h"
#include "LawnApp.h"
#include "ContinueDialog.h"
#include "Resources.h"
#include "Sexy.TodLib/TodFoley.h"
#include "Sexy.TodLib/TodStringFile.h"

ContinueDialog::ContinueDialog(LawnApp* theApp) : LawnDialog(
	theApp, 
	Dialogs::DIALOG_CONTINUE, 
	true, 
	_S("CONTINUE GAME?"/*[CONTINUE_GAME_HEADER]*/),
	_S(""), 
	_S("CANCEL"/*[DIALOG_BUTTON_CANCEL]*/),
	Dialog::BUTTONS_FOOTER)
{
    if (theApp->IsAdventureMode())
    {
        mDialogLines = _S("Do you want to continue your current game or restart the level?"/*[CONTINUE_GAME_OR_RESTART]*/);
        mContinueButton = MakeButton(ContinueDialog::ContinueDialog_Continue, this, _S("Continue"/*[CONTINUE_BUTTON]*/));
        mNewGameButton = MakeButton(ContinueDialog::ContinueDialog_NewGame, this, _S("Restart Level"/*[RESTART_BUTTON]*/));
    }
    else
    {
        mDialogLines = _S("Do you want to continue your current game or start a new game?"/*[CONTINUE_GAME]*/);
        mContinueButton = MakeButton(ContinueDialog::ContinueDialog_Continue, this, _S("Continue"/*[CONTINUE_BUTTON]*/));
        mNewGameButton = MakeButton(ContinueDialog::ContinueDialog_NewGame, this, _S("New Game"/*[NEW_GAME_BUTTON]*/));
    }

    mTallBottom = true;
    CalcSize(10, 60);
}

ContinueDialog::~ContinueDialog()
{
    delete mContinueButton;
    delete mNewGameButton;
}

int ContinueDialog::GetPreferredHeight(int theWidth)
{
    return LawnDialog::GetPreferredHeight(theWidth) + 40;
}

void ContinueDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
    LawnDialog::Resize(theX, theY, theWidth, theHeight);

    int aBtnWidth = IMAGE_BUTTON_LEFT->mWidth + IMAGE_BUTTON_MIDDLE->mWidth * 3 + IMAGE_BUTTON_RIGHT->mWidth;
    int aBtnHeight = mLawnYesButton->mHeight;

    /*
    int aContinueX = mLawnYesButton->mX - 20;
    int aNewGameX = mLawnYesButton->mX + mLawnYesButton->mWidth - aBtnWidth + 24;
    while (aContinueX + aBtnWidth > aNewGameX)
    {
        aContinueX -= 20;
        aNewGameX += 20;
    }

    mContinueButton->Resize(aContinueX, mLawnYesButton->mY - aBtnHeight - 2, aBtnWidth, aBtnHeight);
    mNewGameButton->Resize(aNewGameX, mContinueButton->mY, aBtnWidth, aBtnHeight);
    mLawnYesButton->Resize(theWidth / 2 - aBtnWidth / 2, mLawnYesButton->mY, aBtnWidth, aBtnHeight);
    */

    mContinueButton->Resize(mLawnYesButton->mX - 20, mLawnYesButton->mY - aBtnHeight, aBtnWidth, aBtnHeight);
    mNewGameButton->Resize(mLawnYesButton->mX + mLawnYesButton->mWidth - aBtnWidth + 20, mContinueButton->mY, aBtnWidth, aBtnHeight);
}

void ContinueDialog::AddedToManager(WidgetManager* theWidgetManager)
{
    LawnDialog::AddedToManager(theWidgetManager);
    AddWidget(mContinueButton);
    AddWidget(mNewGameButton);
}

void ContinueDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
    LawnDialog::RemovedFromManager(theWidgetManager);
    RemoveWidget(mContinueButton);
    RemoveWidget(mNewGameButton);
}

void ContinueDialog::RestartLoopingSounds()
{
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS || mApp->IsStormyNightLevel())
    {
        mApp->PlayFoley(FoleyType::FOLEY_RAIN);
    }

    Zombie* aZombie = nullptr;
    while (mApp->mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mPlayingSong)
        {
            aZombie->StartZombieSound();
        }
    }
}

void ContinueDialog::ButtonDepress(int theId)
{
    if (theId == ContinueDialog::ContinueDialog_Continue)
    {
        if (mApp->mBoard->mNextSurvivalStageCounter != 1)
        {
            std::string aFileName = GetSavedGameName(mApp->mGameMode, mApp->mPlayerInfo->mId);
            mApp->EraseFile(aFileName);
        }

        RestartLoopingSounds();
        mApp->KillDialog(mId);
    }
    else if (theId == ContinueDialog::ContinueDialog_NewGame)
    {
        if (mApp->IsAdventureMode())
        {
            LawnDialog* aDialog = (LawnDialog*)mApp->DoDialog(
                Dialogs::DIALOG_RESTARTCONFIRM, 
                true, 
                _S("Restart Level?"/*[RESTART_LEVEL_HEADER]*/),
                _S("Are you sure that you want to restart the level?"/*[RESTART_LEVEL]*/),
                _S(""), 
                Dialog::BUTTONS_OK_CANCEL
            );
            aDialog->mLawnYesButton->mLabel = TodStringTranslate(_S("[RESTART_BUTTON]"));
            //aDialog->CalcSize(0, 0);
        }
        else
        {
            LawnDialog* aDialog = (LawnDialog*)mApp->DoDialog(
                Dialogs::DIALOG_RESTARTCONFIRM, 
                true, 
                _S("New Game?"/*[NEW_GAME_HEADER]*/),
                _S("Are you sure that you want to start a new game?"/*[NEW_GAME]*/),
                _S(""), 
                Dialog::BUTTONS_OK_CANCEL
            );
            aDialog->mLawnYesButton->mLabel = TodStringTranslate(_S("[NEW_GAME_BUTTON]"));
            //aDialog->CalcSize(0, 0);
        }
    }
    else
    {
        mApp->KillDialog(mId);
        if (mApp->IsAdventureMode())
        {
            mApp->ShowGameSelector();
        }
        else if (mApp->IsSurvivalMode())
        {
            mApp->KillBoard();
            mApp->ShowChallengeScreen(ChallengePage::CHALLENGE_PAGE_SURVIVAL);
        }
        else if (mApp->IsPuzzleMode())
        {
            mApp->KillBoard();
            mApp->ShowChallengeScreen(ChallengePage::CHALLENGE_PAGE_PUZZLE);
        }
        else
        {
            mApp->KillBoard();
            mApp->ShowChallengeScreen(ChallengePage::CHALLENGE_PAGE_CHALLENGE);
        }
    }
}
