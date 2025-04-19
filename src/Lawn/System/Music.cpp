//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#include "Music.h"
#include "Board.h"
#include "PlayerInfo.h"
#include "LawnApp.h"
#include "PakLib/PakInterface.h"
#include "Sexy.TodLib/TodDebug.h"
#include "Sexy.TodLib/TodCommon.h"
#include "SexyAppFramework/BassLoader.h"
#include "SexyAppFramework/BassMusicInterface.h"

#include <codecvt>
#include <vector>
#include <fstream>

using namespace Sexy;

Music::Music()
{
	mApp = (LawnApp*)gSexyAppBase;
	mMusicInterface = gSexyAppBase->mMusicInterface;
	mCurMusicTune = MusicTune::MUSIC_TUNE_NONE;
	mCurMusicFileMain = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileDrums = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileHihats = MusicFile::MUSIC_FILE_NONE;
	mBurstOverride = -1;
	mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF;
	mQueuedDrumTrackPackedOrder = -1;
	mBaseBPM = 155;
	mBaseModSpeed = 3;
	mMusicBurstState = MusicBurstState::MUSIC_BURST_OFF;
	mPauseOffset = 0;
	mPauseOffsetDrums = 0;
	mPaused = false;
	mMusicDisabled = false;
	mFadeOutCounter = 0;
	mFadeOutDuration = 0;
}

MusicFileData gMusicFileData[MusicFile::NUM_MUSIC_FILES];  

bool Music::TodLoadMusic(MusicFile theMusicFile, const std::wstring& theFileName)
{
	HMUSIC aHMusic = NULL;
    HSTREAM aStream = NULL;
    BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
    
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::string utf8FileName = conv.to_bytes(theFileName);
	std::string anExt;

	auto dot = utf8FileName.find_last_of('.');
	if (dot != std::string::npos)
		anExt = utf8FileName.substr(dot + 1);

	TodTraceAndLog( "file name: %s", utf8FileName.c_str() );

    if (anExt != "wav" && anExt != "ogg" && anExt != "mp3")
    {
		std::ifstream file( utf8FileName, std::ios::binary | std::ios::ate );
		if ( !file )
			return false;

		int aSize = static_cast<int>(file.tellg());
		file.seekg(0, std::ios::beg);

		std::vector<char> aData(aSize);
		file.read(aData.data(), aSize);

		aHMusic = BASS_MusicLoad(TRUE, aData.data(), 0, aSize, BASS_MUSIC_RAMP, 0);

        if (!aHMusic)
        {
            TodTraceAndLog("error code: %d", BASS_ErrorGetCode());
            return false;
        }

        TOD_ASSERT(gMusicFileData[theMusicFile].mFileData == nullptr);
		gMusicFileData[theMusicFile].mFileData = reinterpret_cast<unsigned int *>(aData.data());
	}
    else
    {
		std::ifstream file( utf8FileName, std::ios::binary | std::ios::ate );
		if ( !file )
			return false;

		int aSize = static_cast<int>(file.tellg());
		file.seekg(0, std::ios::beg);

		std::vector<char> aData(aSize);
		file.read(aData.data(), aSize);

		char* dataCopy = new char[aSize];
		std::memcpy(dataCopy, aData.data(), aSize);

		aStream = BASS_StreamCreateFile(TRUE, dataCopy, 0, aSize, 0);
        if (!aStream)
        {
			TodTraceAndLog("error code: %d", BASS_ErrorGetCode());
			delete[] dataCopy;
            return false;
        }

        TOD_ASSERT(gMusicFileData[theMusicFile].mFileData == nullptr);
		gMusicFileData[theMusicFile].mFileData = reinterpret_cast<unsigned int*>(dataCopy);
	}

    BassMusicInfo aMusicInfo;
    aMusicInfo.mHStream = aStream;
    aMusicInfo.mHMusic = aHMusic;
    aBass->mMusicMap.insert(BassMusicMap::value_type(theMusicFile, aMusicInfo));
    return true;
}

void Music::SetupMusicFileForTune(MusicFile theMusicFile, MusicTune theMusicTune)
{
	int aTrackCount = 0;
	int aTrackStart1 = -1, aTrackEnd1 = -1, aTrackStart2 = -1, aTrackEnd2 = -1;

	switch (theMusicTune)
	{
	case MusicTune::MUSIC_TUNE_DAY_GRASSWALK:
		switch (theMusicFile) {
		case MusicFile::MUSIC_FILE_MAIN_MUSIC:		aTrackCount = 30;	aTrackStart1 = 0;	aTrackEnd1 = 23;											break;
		case MusicFile::MUSIC_FILE_HIHATS:			aTrackCount = 30;	aTrackStart1 = 27;	aTrackEnd1 = 27;											break;
		case MusicFile::MUSIC_FILE_DRUMS:			aTrackCount = 30;	aTrackStart1 = 24;	aTrackEnd1 = 26;											break;
		} break;
	case MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES:
		switch (theMusicFile) {
		case MusicFile::MUSIC_FILE_MAIN_MUSIC:		aTrackCount = 30;	aTrackStart1 = 0;	aTrackEnd1 = 17;											break;
		case MusicFile::MUSIC_FILE_HIHATS:			aTrackCount = 30;	aTrackStart1 = 18;	aTrackEnd1 = 24;	aTrackStart2 = 29;	aTrackEnd2 = 29;	break;
		case MusicFile::MUSIC_FILE_DRUMS:			aTrackCount = 30;	aTrackStart1 = 25;	aTrackEnd1 = 28;											break;
		} break;
	case MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST:
		switch (theMusicFile) {
		case MusicFile::MUSIC_FILE_MAIN_MUSIC:		aTrackCount = 30;	aTrackStart1 = 0;	aTrackEnd1 = 15;											break;
		case MusicFile::MUSIC_FILE_HIHATS:			aTrackCount = 30;	aTrackStart1 = 23;	aTrackEnd1 = 23;											break;
		case MusicFile::MUSIC_FILE_DRUMS:			aTrackCount = 30;	aTrackStart1 = 16;	aTrackEnd1 = 22;											break;
		} break;
	case MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF:
		switch (theMusicFile) {
		case MusicFile::MUSIC_FILE_MAIN_MUSIC:		aTrackCount = 30;	aTrackStart1 = 0;	aTrackEnd1 = 17;											break;
		case MusicFile::MUSIC_FILE_HIHATS:			aTrackCount = 30;	aTrackStart1 = 21;	aTrackEnd1 = 21;											break;
		case MusicFile::MUSIC_FILE_DRUMS:			aTrackCount = 30;	aTrackStart1 = 18;	aTrackEnd1 = 20;											break;
		} break;
	default:
		if (theMusicFile == MusicFile::MUSIC_FILE_MAIN_MUSIC || theMusicFile == MusicFile::MUSIC_FILE_DRUMS)
		{
			aTrackCount = 30;
			aTrackStart1 = 0;
			aTrackEnd1 = 30;
		}
		break;
	}

	HMUSIC aHMusic = GetBassMusicHandle(theMusicFile);
	for (int aTrack = 0; aTrack < aTrackCount; aTrack++)
	{
		int aVolume;
		if (aTrack >= aTrackStart1 && aTrack <= aTrackEnd1)
			aVolume = 150;
		else if (aTrack >= aTrackStart2 && aTrack <= aTrackEnd2)
			aVolume = 150;
		else
			aVolume = 0;

		BASS_ChannelSetAttribute(aHMusic, BASS_ATTRIB_VOL, aVolume/100);
	}
}

void Music::LoadSong(MusicFile theMusicFile, const std::wstring& theFileName)
{
	TodTrace("preloadsong");

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::string utf8FileName = conv.to_bytes(theFileName);

	if (!TodLoadMusic(theMusicFile, theFileName))
	{
		TodTrace("music failed to load\n");
		mMusicDisabled = true;
	}
	else
	{
		BASS_ChannelSetAttribute(GetBassMusicHandle(theMusicFile), BASS_ATTRIB_MUSIC_PSCALER, 4);
		TodTrace("song '%s'", utf8FileName.c_str());
	}
}

void Music::MusicTitleScreenInit()
{
	LoadSong(MusicFile::MUSIC_FILE_MAIN_MUSIC, L"sounds\\mainmusic.mo3");
	MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_TITLE_CRAZY_DAVE_MAIN_THEME);
}

void Music::MusicInit()
{
#ifdef _DEBUG
	int aNumLoadingTasks = mApp->mCompletedLoadingThreadTasks + GetNumLoadingTasks();
#endif

	LoadSong(MusicFile::MUSIC_FILE_DRUMS, L"sounds\\mainmusic.mo3");
	mApp->mCompletedLoadingThreadTasks += 3500;
	LoadSong(MusicFile::MUSIC_FILE_HIHATS, L"sounds\\mainmusic_hihats.mo3");
	mApp->mCompletedLoadingThreadTasks += 3500;
}

void Music::MusicCreditScreenInit()
{
	BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
	if (aBass->mMusicMap.find((int)MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN) == aBass->mMusicMap.end())
		LoadSong(MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN, L"sounds\\ZombiesOnYourLawn.ogg");
}

void Music::StopAllMusic()
{
	if (mMusicInterface != nullptr)
	{
		if (mCurMusicFileMain != MusicFile::MUSIC_FILE_NONE)
			mMusicInterface->StopMusic(mCurMusicFileMain);
		if (mCurMusicFileDrums != MusicFile::MUSIC_FILE_NONE)
			mMusicInterface->StopMusic(mCurMusicFileDrums);
		if (mCurMusicFileHihats != MusicFile::MUSIC_FILE_NONE)
			mMusicInterface->StopMusic(mCurMusicFileHihats);
	}

	mCurMusicTune = MusicTune::MUSIC_TUNE_NONE;
	mCurMusicFileMain = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileDrums = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileHihats = MusicFile::MUSIC_FILE_NONE;
	mQueuedDrumTrackPackedOrder = -1;
	mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF;
	mMusicBurstState = MusicBurstState::MUSIC_BURST_OFF;
	mPauseOffset = 0;
	mPauseOffsetDrums = 0;
	mPaused = false;
	mFadeOutCounter = 0;
}

HMUSIC Music::GetBassMusicHandle(MusicFile theMusicFile)
{
	BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
	auto anItr = aBass->mMusicMap.find((int)theMusicFile);
	TOD_ASSERT(anItr != aBass->mMusicMap.end());
	return anItr->second.mHMusic;
}

void Music::PlayFromOffset(MusicFile theMusicFile, int theOffset, double theVolume)
{
	if (!mApp) mApp = (LawnApp*)gSexyApp;
	BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
	auto anItr = aBass->mMusicMap.find((int)theMusicFile);
	TOD_ASSERT(anItr != aBass->mMusicMap.end());
	BassMusicInfo* aMusicInfo = &anItr->second;

	if (aMusicInfo->mHStream)
	{
		bool aNoLoop = theMusicFile == MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN;
		mMusicInterface->PlayMusic(theMusicFile, theOffset, aNoLoop);
	}
	else
	{
		BASS_ChannelStop(aMusicInfo->mHMusic);
		SetupMusicFileForTune(theMusicFile, mCurMusicTune);
		aMusicInfo->mStopOnFade = false;
		aMusicInfo->mVolume = aMusicInfo->mVolumeCap * theVolume;
		aMusicInfo->mVolumeAdd = 0.0;
		//gBass->BASS_ChannelSetAttribute(aMusicInfo->mHMusic, -1, aMusicInfo->mVolume * 100.0, -101);
		BASS_ChannelSetAttribute(aMusicInfo->mHMusic, BASS_ATTRIB_VOL, aMusicInfo->mVolume);
		BASS_ChannelFlags(aMusicInfo->mHMusic, BASS_MUSIC_POSRESET | BASS_MUSIC_RAMP | BASS_MUSIC_LOOP, -1);
		BASS_ChannelSetPosition(aMusicInfo->mHMusic, MAKELONG(theOffset, 0) /**/, BASS_POS_MUSIC_ORDER);
		BASS_ChannelPlay(aMusicInfo->mHMusic, false);
	}
}

void Music::PlayMusic(MusicTune theMusicTune, int theOffset, int theDrumsOffset)
{
	if (mMusicDisabled)
		return;

	mCurMusicTune = theMusicTune;
	mCurMusicFileMain = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileDrums = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileHihats = MusicFile::MUSIC_FILE_NONE;
	bool aRestartingSong = theOffset != -1;

	switch (theMusicTune)
	{
	case MusicTune::MUSIC_TUNE_DAY_GRASSWALK:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		if (theOffset == -1)
		{
			theOffset = 0x80000030;
			theDrumsOffset = 0x8000005C;
		}
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theDrumsOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
		if (theOffset == -1)
			theOffset = 0x8000005E;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
		if (theOffset == -1)
			theOffset = 0x8000007D;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
		if (theOffset == -1)
			theOffset = 0x800000B8;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_CHOOSE_YOUR_SEEDS:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0x8000007A;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_TITLE_CRAZY_DAVE_MAIN_THEME:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0x80000098;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_ZEN_GARDEN:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0xDD;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_PUZZLE_CEREBRAWL:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0xB1;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_MINIGAME_LOONBOON:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0xA6;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_CONVEYER:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0xD4;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_FINAL_BOSS_BRAINIAC_MANIAC:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0x9E;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_CREDITS_ZOMBIES_ON_YOUR_LAWN:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	default:
		TOD_ASSERT(false);
		break;
	}

	if (aRestartingSong)
	{
		if (mCurMusicFileMain != MusicFile::MUSIC_FILE_NONE)
		{
			HMUSIC aHMusic = GetBassMusicHandle(mCurMusicFileMain);
			BASS_ChannelSetAttribute(aHMusic, BASS_ATTRIB_MUSIC_BPM, mBaseBPM);
			BASS_ChannelSetAttribute(aHMusic, BASS_ATTRIB_MUSIC_SPEED, mBaseModSpeed);
		}
		if (mCurMusicFileDrums != -1)
		{
			HMUSIC aHMusic = GetBassMusicHandle(mCurMusicFileDrums);
			BASS_ChannelSetAttribute(aHMusic, BASS_ATTRIB_MUSIC_BPM, mBaseBPM);
			BASS_ChannelSetAttribute(aHMusic, BASS_ATTRIB_MUSIC_SPEED, mBaseModSpeed);
		}
		if (mCurMusicFileHihats != -1)
		{
			HMUSIC aHMusic = GetBassMusicHandle(mCurMusicFileHihats);
			BASS_ChannelSetAttribute(aHMusic, BASS_ATTRIB_MUSIC_BPM, mBaseBPM);
			BASS_ChannelSetAttribute(aHMusic, BASS_ATTRIB_MUSIC_SPEED, mBaseModSpeed);
		}
	}
	else
	{
		HMUSIC aHMusic = GetBassMusicHandle(mCurMusicFileMain);
		mBaseBPM = BASS_ChannelSetAttribute(aHMusic, BASS_ATTRIB_MUSIC_BPM, -1);
		mBaseModSpeed = BASS_ChannelSetAttribute(aHMusic, BASS_ATTRIB_MUSIC_SPEED, -1);
	}
}

unsigned long Music::GetMusicOrder(MusicFile theMusicFile)
{
	TOD_ASSERT(theMusicFile != MusicFile::MUSIC_FILE_NONE);
    HCHANNEL hChannel = GetBassMusicHandle(theMusicFile);
    QWORD packedPos = BASS_ChannelGetPosition(hChannel, BASS_POS_MUSIC_ORDER);
    return packedPos;
}

void Music::MusicResyncChannel(MusicFile theMusicFileToMatch, MusicFile theMusicFileToSync)
{
	QWORD posMatch = GetMusicOrder(theMusicFileToMatch);
    QWORD posSync = GetMusicOrder(theMusicFileToSync);
    int orderMatch = static_cast<int>(posMatch >> 16);
    int orderSync  = static_cast<int>(posSync  >> 16);
    int diff = orderSync - orderMatch;
    if (std::abs(diff) <= 128)
    {
        HCHANNEL hSync = GetBassMusicHandle(theMusicFileToSync);
        int adjustedBPM = mBaseBPM;
        if      (diff >  2) adjustedBPM -= 2;
        else if (diff >  0) adjustedBPM -= 1;
        else if (diff < -2) adjustedBPM += 2;
        else if (diff <  0) adjustedBPM += 1;

        BASS_ChannelSetAttribute(hSync, BASS_ATTRIB_MUSIC_BPM, static_cast<float>(adjustedBPM));
    }
}

void Music::MusicResync()
{
	if (mCurMusicFileMain == MusicFile::MUSIC_FILE_NONE)
        return;

    if (mCurMusicFileDrums  != MusicFile::MUSIC_FILE_NONE)
        MusicResyncChannel(mCurMusicFileMain, mCurMusicFileDrums);
    if (mCurMusicFileHihats != MusicFile::MUSIC_FILE_NONE)
        MusicResyncChannel(mCurMusicFileMain, mCurMusicFileHihats);
}

void Music::StartBurst()
{ 
	if (mMusicBurstState == MusicBurstState::MUSIC_BURST_OFF)
	{ 
		mMusicBurstState = MusicBurstState::MUSIC_BURST_STARTING;
		mBurstStateCounter = 400;
	}
}

void Music::FadeOut(int theFadeOutDuration)
{ 
	if (mCurMusicTune != MusicTune::MUSIC_TUNE_NONE)
    {
        mFadeOutCounter = theFadeOutDuration;
        mFadeOutDuration = theFadeOutDuration;
    }
}

void Music::UpdateMusicBurst()
{
	if (mApp->mBoard == nullptr)
		return;

	int aBurstScheme;
	if (mCurMusicTune == MusicTune::MUSIC_TUNE_DAY_GRASSWALK || mCurMusicTune == MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES ||
		mCurMusicTune == MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST || mCurMusicTune == MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF)
		aBurstScheme = 1;
	else if (mCurMusicTune == MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS)
		aBurstScheme = 2;
	else
		return;

	int aPackedOrderMain = GetMusicOrder(mCurMusicFileMain);
	if (mBurstStateCounter > 0)
		mBurstStateCounter--;
	if (mDrumsStateCounter > 0)
		mDrumsStateCounter--;

	float aFadeTrackVolume = 0.0f;
	float aDrumsVolume = 0.0f;
	float aMainTrackVolume = 1.0f;
	switch (mMusicBurstState)
	{
		case MusicBurstState::MUSIC_BURST_OFF:
			if (mApp->mBoard->CountZombiesOnScreen() >= 10 || mBurstOverride == 1)
				StartBurst();
			break;
		case MusicBurstState::MUSIC_BURST_STARTING:
			if (aBurstScheme == 1)
			{
				aFadeTrackVolume = TodAnimateCurveFloat(400, 0, mBurstStateCounter, 0.0f, 1.0f, TodCurves::CURVE_LINEAR);
				if (mBurstStateCounter == 100)
				{
					mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON_QUEUED;
					mQueuedDrumTrackPackedOrder = aPackedOrderMain;
				}
				else if (mBurstStateCounter == 0)
				{
					mMusicBurstState = MusicBurstState::MUSIC_BURST_ON;
					mBurstStateCounter = 800;
				}
			}
			else if (aBurstScheme == 2)
			{
				if (mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_OFF)
				{
					mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON_QUEUED;
					mQueuedDrumTrackPackedOrder = aPackedOrderMain;
					mBurstStateCounter = 400;
				}
				else if (mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_ON_QUEUED)
					mBurstStateCounter = 400;
				else
				{
					aMainTrackVolume = TodAnimateCurveFloat(400, 0, mBurstStateCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
					if (mBurstStateCounter == 0)
					{
						mMusicBurstState = MusicBurstState::MUSIC_BURST_ON;
						mBurstStateCounter = 800;
					}
				}
			}
			break;
		case MusicBurstState::MUSIC_BURST_ON:
			aFadeTrackVolume = 1.0f;
			if (aBurstScheme == 2)
				aMainTrackVolume = 0.0f;
			if (mBurstStateCounter == 0 && ((mApp->mBoard->CountZombiesOnScreen() < 4 && mBurstOverride == -1) || mBurstOverride == 2))
			{
				if (aBurstScheme == 1)
				{
					mMusicBurstState = MusicBurstState::MUSIC_BURST_FINISHING;
					mBurstStateCounter = 800;
					mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF_QUEUED;
					mQueuedDrumTrackPackedOrder = aPackedOrderMain;
				}
				else if (aBurstScheme == 2)
				{
					mMusicBurstState = MusicBurstState::MUSIC_BURST_FINISHING;
					mBurstStateCounter = 1100;
					mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_FADING;
					mDrumsStateCounter = 800;
				}
			}
			break;
		case MusicBurstState::MUSIC_BURST_FINISHING:
			if (aBurstScheme == 1)
				aFadeTrackVolume = TodAnimateCurveFloat(800, 0, mBurstStateCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
			else
				aMainTrackVolume = TodAnimateCurveFloat(400, 0, mBurstStateCounter, 0.0f, 1.0f, TodCurves::CURVE_LINEAR);
			if (mBurstStateCounter == 0 && mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_OFF)
				mMusicBurstState = MusicBurstState::MUSIC_BURST_OFF;
			break;
	}

	int aDrumsJumpOrder = -1;
	int aOrderMain = 0, aOrderDrum = 0;
	if (aBurstScheme == 1)
	{
		aOrderMain = HIWORD(aPackedOrderMain) / 128;
		aOrderDrum = HIWORD(mQueuedDrumTrackPackedOrder) / 128;
	}
	else if (aBurstScheme == 2)
	{
		aOrderMain = LOWORD(aPackedOrderMain);
		aOrderDrum = LOWORD(mQueuedDrumTrackPackedOrder);
		if (HIWORD(aPackedOrderMain) > 252)
			aOrderMain++;
		if (HIWORD(mQueuedDrumTrackPackedOrder) > 252)
			aOrderDrum++;
	}

	switch (mMusicDrumsState)
	{
		case MusicDrumsState::MUSIC_DRUMS_ON_QUEUED:
			if (aOrderMain != aOrderDrum)
			{
				aDrumsVolume = 1.0f;
				mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON;
				if (aBurstScheme == 2)
					aDrumsJumpOrder = (aOrderMain % 2 == 0) ? 76 : 77;
			}
			break;
		case MusicDrumsState::MUSIC_DRUMS_ON:
			aDrumsVolume = 1.0f;
			break;
		case MusicDrumsState::MUSIC_DRUMS_OFF_QUEUED:
			aDrumsVolume = 1.0f;
			if (aOrderMain != aOrderDrum && aBurstScheme == 1)
			{
				mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_FADING;
				mDrumsStateCounter = 50;
			}
			break;
		case MusicDrumsState::MUSIC_DRUMS_FADING:
			if (aBurstScheme == 2)
				aDrumsVolume = TodAnimateCurveFloat(800, 0, mDrumsStateCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
			else
				aDrumsVolume = TodAnimateCurveFloat(50, 0, mDrumsStateCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
			if (mDrumsStateCounter == 0)
				mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF;
			break;
	}

	if (aBurstScheme == 1)
	{
		mMusicInterface->SetSongVolume(mCurMusicFileHihats, aFadeTrackVolume);
		mMusicInterface->SetSongVolume(mCurMusicFileDrums, aDrumsVolume);
	}
	else if (aBurstScheme == 2)
	{
		mMusicInterface->SetSongVolume(mCurMusicFileMain, aMainTrackVolume);
		mMusicInterface->SetSongVolume(mCurMusicFileDrums, aDrumsVolume);
		if (aDrumsJumpOrder != -1)
			BASS_ChannelSetPosition(GetBassMusicHandle(mCurMusicFileDrums), LOWORD(aDrumsJumpOrder) | 0x80000000, -1);
	}
}

void Music::MusicUpdate()
{
	if (mFadeOutCounter > 0)
    {
        --mFadeOutCounter;
        if (mFadeOutCounter == 0)
            StopAllMusic();
        else
        {
            float fadeLevel = TodAnimateCurveFloat(
                mFadeOutDuration, 0, mFadeOutCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
            BASS_ChannelSetAttribute(
                GetBassMusicHandle(mCurMusicFileMain),
                BASS_ATTRIB_VOL, fadeLevel);
        }
    }

    if (mApp->mBoard == nullptr || mApp->mBoard->mPaused)
        return;

    UpdateMusicBurst();
    MusicResync();
}

void Music::MakeSureMusicIsPlaying(MusicTune theMusicTune)
{
	if (mCurMusicTune != theMusicTune)
	{
		StopAllMusic();
		PlayMusic(theMusicTune, -1, -1);
	}
}

void Music::StartGameMusic()
{
	TOD_ASSERT(mApp->mBoard);

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_ZEN_GARDEN);
	else if (mApp->IsFinalBossLevel())
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_FINAL_BOSS_BRAINIAC_MANIAC);
	else if (mApp->IsWallnutBowlingLevel() || mApp->IsWhackAZombieLevel() || mApp->IsLittleTroubleLevel() || mApp->IsBungeeBlitzLevel() ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_SPEED)
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_MINIGAME_LOONBOON);
	else if (mApp->IsMiniBossLevel() || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN)
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_CONVEYER);
	else if (mApp->IsStormyNightLevel())
		StopAllMusic();
	else if (mApp->IsScaryPotterLevel() || mApp->IsIZombieLevel())
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_PUZZLE_CEREBRAWL);
	else if (mApp->mBoard->StageHasFog())
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST);
	else if (mApp->mBoard->StageIsNight())
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS);
	else if (mApp->mBoard->StageHas6Rows())
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES);
	else if (mApp->mBoard->StageHasRoof())
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF);
	else
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_DAY_GRASSWALK);
}

void Music::GameMusicPause(bool thePause)
{
	if (thePause)
	{
		if (!mPaused && mCurMusicTune != MusicTune::MUSIC_TUNE_NONE)
		{
			BassMusicInterface* aBass = (BassMusicInterface*)mMusicInterface;
			auto anItr = aBass->mMusicMap.find(mCurMusicFileMain);
			TOD_ASSERT(anItr != aBass->mMusicMap.end());
			BassMusicInfo* aMusicInfo = &anItr->second;

			if (aMusicInfo->mHStream)
			{
				mPauseOffset = BASS_ChannelGetPosition(aMusicInfo->mHStream, BASS_POS_BYTE);
				mMusicInterface->StopMusic(mCurMusicFileMain);
			}
			else
			{
				int aOrderMain = GetMusicOrder(mCurMusicFileMain);
				mPauseOffset = MAKELONG(LOWORD(aOrderMain), HIWORD(aOrderMain));
				mMusicInterface->StopMusic(mCurMusicFileMain);

				if (mCurMusicTune == MusicTune::MUSIC_TUNE_DAY_GRASSWALK || mCurMusicTune == MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES ||
					mCurMusicTune == MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST || mCurMusicTune == MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF)
				{
					mMusicInterface->StopMusic(mCurMusicFileDrums);
					mMusicInterface->StopMusic(mCurMusicFileHihats);
				}
				else if (mCurMusicTune == MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS)
				{
					int aOrderDrum = GetMusicOrder(mCurMusicFileDrums);
					mPauseOffsetDrums = MAKELONG(LOWORD(aOrderDrum), HIWORD(aOrderDrum));
					mMusicInterface->StopMusic(mCurMusicFileDrums);
				}
			}
			mPaused = true;
		}
	}
	else if (mPaused)
	{
		if (mCurMusicTune != MusicTune::MUSIC_TUNE_NONE)
			PlayMusic(mCurMusicTune, mPauseOffset, mPauseOffsetDrums);
		mPaused = false;
	}
}

int Music::GetNumLoadingTasks()
{
	return 3500 * 2;  
}
