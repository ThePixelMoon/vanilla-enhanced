//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#include "BassMusicInterface.h"
#include "BassLoader.h"
#include "PakLib\PakInterface.h"
#include "Sexy.TodLib\TodDebug.h"

#include <codecvt>
#include <vector>
#include <fstream>

using namespace Sexy;

#define BASS2_MUSIC_RAMP			BASS_MUSIC_RAMP	// normal ramping

#define BASS_CONFIG_BUFFER			0

BassMusicInfo::BassMusicInfo()
{	
	mVolume = 0.0;
	mVolumeAdd = 0.0;
	mVolumeCap = 1.0;
	mStopOnFade = false;
	mHMusic = NULL;
	mHStream = NULL;
}

BassMusicInterface::BassMusicInterface(HWND theHWnd)
{
	LoadBassDLL();

	MIXERCONTROLDETAILS mcd;
	MIXERCONTROLDETAILS_UNSIGNED mxcd_u;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROL mlct;
	MIXERLINE mixerLine;
	HMIXEROBJ phmx;
	MIXERCAPS pmxcaps;	

	mixerOpen((HMIXER*) &phmx, 0, 0, 0, MIXER_OBJECTF_MIXER);
	mixerGetDevCaps(0, &pmxcaps, sizeof(pmxcaps));

	mxlc.cbStruct = sizeof(mxlc);	
	mxlc.cbmxctrl = sizeof(mlct);
	mxlc.pamxctrl = &mlct;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mixerLine.cbStruct = sizeof(mixerLine);
	mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	mixerGetLineInfo(phmx, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
	mxlc.dwLineID = mixerLine.dwLineID;
	mixerGetLineControls(phmx, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);	

	mcd.cbStruct = sizeof(mcd);
	mcd.dwControlID = mlct.dwControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;
	mcd.cbDetails = sizeof(mxcd_u);
	mcd.paDetails = &mxcd_u;
	
	mixerGetControlDetails(phmx, &mcd, 0L);

	//return mxcd_u.dwValue;	

	BOOL success;

	success = BASS_Init(1, 44100, 0, theHWnd, NULL);
	BASS_SetConfig(BASS_CONFIG_BUFFER, 2000);

	BASS_PluginLoad( "plugins\\bassape.dll", 0 );
	BASS_PluginLoad( "plugins\\bassdsd.dll", 0 );
	BASS_PluginLoad( "plugins\\bassflac.dll", 0 );
	BASS_PluginLoad( "plugins\\basshls.dll", 0 );
	BASS_PluginLoad( "plugins\\bassmidi.dll", 0 );
	BASS_PluginLoad( "plugins\\bassmix.dll", 0 );
	BASS_PluginLoad( "plugins\\bassopus.dll", 0 );
	BASS_PluginLoad( "plugins\\basswv.dll", 0 );

	mixerSetControlDetails(phmx, &mcd, 0L);

	BASS_Start();

	mixerClose((HMIXER) phmx);

	mMaxMusicVolume = 40;

	mMusicLoadFlags = BASS_MUSIC_LOOP | BASS2_MUSIC_RAMP;
}

BassMusicInterface::~BassMusicInterface()
{
	BASS_Stop();
	BASS_Free();
}

bool BassMusicInterface::LoadMusic(int theSongId, const std::wstring& theFileName)
{
	HMUSIC aHMusic = NULL;
	HSTREAM aStream = NULL;
	
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::string utf8FileName = conv.to_bytes(theFileName);
	std::string anExt;

	auto aDotPos = utf8FileName.find_last_of('.');
	if (aDotPos != std::string::npos)
		anExt = utf8FileName.substr(aDotPos + 1);

	if (anExt=="wav" || anExt=="ogg" || anExt=="mp3")
		aStream = BASS_StreamCreateFile(FALSE, (void*)theFileName.c_str(), 0, 0, 0);
	else
	{
		std::ifstream file(utf8FileName, std::ios::binary | std::ios::ate);
		if (!file)
			return false;

		int aSize = static_cast<int>(file.tellg());
		file.seekg(0, std::ios::beg);

		std::vector<char> aData(aSize);
		file.read(aData.data(), aSize);

		aHMusic = BASS_MusicLoad(TRUE, aData.data(), 0, aSize, mMusicLoadFlags, 0);
	}

	if (!aHMusic && !aStream)
		return false;
	
	BassMusicInfo aMusicInfo;	
	aMusicInfo.mHMusic = aHMusic;
	aMusicInfo.mHStream = aStream;
	mMusicMap.insert(BassMusicMap::value_type(theSongId, aMusicInfo));

	return true;	
}

void BassMusicInterface::PlayMusic(int theSongId, int theOffset, bool noLoop)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = aMusicInfo->mVolumeCap;
		aMusicInfo->mVolumeAdd = 0.0;
		aMusicInfo->mStopOnFade = noLoop;

		BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, aMusicInfo->mVolume);
		BASS_ChannelStop(aMusicInfo->GetHandle());

		if (aMusicInfo->mHMusic)
		{
			BASS_ChannelPlay(aMusicInfo->mHMusic, TRUE);
			BASS_ChannelSetPosition(aMusicInfo->mHMusic, theOffset, BASS_POS_MUSIC_ORDER);
			BASS_ChannelFlags(aMusicInfo->mHMusic, BASS_MUSIC_POSRESET | BASS2_MUSIC_RAMP | (noLoop ? 0 : BASS_MUSIC_LOOP), 0);
		}
		else
		{
			BOOL flush = theOffset == -1 ? FALSE : TRUE;
			BASS_ChannelPlay(aMusicInfo->mHStream, flush);
			BASS_ChannelFlags(aMusicInfo->mHStream, noLoop ? 0 : BASS_MUSIC_LOOP, 0);

			if (theOffset > 0)
				BASS_ChannelSetPosition(aMusicInfo->mHStream, theOffset, BASS_POS_BYTE);
		}
	}
}

void BassMusicInterface::StopMusic(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;
		BASS_ChannelStop(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::StopAllMusic()
{
	BassMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;
		BASS_ChannelStop(aMusicInfo->GetHandle());
		++anItr;
	}
}

void BassMusicInterface::UnloadMusic(int theSongId)
{
	StopMusic(theSongId);
	
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		if (aMusicInfo->mHStream)
			BASS_StreamFree(aMusicInfo->mHStream);
		else if (aMusicInfo->mHMusic)
			BASS_MusicFree(aMusicInfo->mHMusic);

		mMusicMap.erase(anItr);
	}
}

void BassMusicInterface::UnloadAllMusic()
{
	StopAllMusic();
	for (BassMusicMap::iterator anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr)
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		if (aMusicInfo->mHStream)
			BASS_StreamFree(aMusicInfo->mHStream);
		else if (aMusicInfo->mHMusic)
			BASS_MusicFree(aMusicInfo->mHMusic);
	}
	mMusicMap.clear();
}

void BassMusicInterface::PauseMusic(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		BASS_ChannelPause(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::PauseAllMusic()
{
	for (BassMusicMap::iterator anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr)
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		if (BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PLAYING)
			BASS_ChannelPause(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::ResumeAllMusic()
{
	for (BassMusicMap::iterator anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr)
	{
		BassMusicInfo* aMusicInfo = &anItr->second;

		if (BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PAUSED)
			BASS_ChannelPlay(aMusicInfo->GetHandle(), FALSE);
	}
}

void BassMusicInterface::ResumeMusic(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		BASS_ChannelPlay(aMusicInfo->GetHandle(), FALSE);
	}
}

void BassMusicInterface::FadeIn(int theSongId, int theOffset, double theSpeed, bool noLoop)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
				
		aMusicInfo->mVolumeAdd = theSpeed;
		aMusicInfo->mStopOnFade = noLoop;

		BASS_ChannelStop(aMusicInfo->GetHandle());
		BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int)(aMusicInfo->mVolume/100));
		if (aMusicInfo->mHMusic)
		{
			if (theOffset == -1)
				BASS_ChannelPlay(aMusicInfo->mHMusic, FALSE);
			else
			{
				BASS_ChannelPlay(aMusicInfo->mHMusic, TRUE);
				BASS_ChannelSetPosition( aMusicInfo->mHMusic, theOffset, BASS_POS_BYTE );
				BASS_ChannelFlags(aMusicInfo->mHMusic, BASS2_MUSIC_RAMP | (noLoop ? 0 : BASS_MUSIC_LOOP), -1);
			}
		}
		else
		{
			BOOL flush = theOffset == -1 ? FALSE : TRUE;
			BASS_ChannelPlay(aMusicInfo->mHStream, flush);
			BASS_ChannelFlags(aMusicInfo->mHStream, noLoop ? 0 : BASS_MUSIC_LOOP, -1);
			if (theOffset > 0)
				BASS_ChannelSetPosition(aMusicInfo->mHStream, theOffset, BASS_POS_BYTE);
		}

	}
}

void BassMusicInterface::FadeOut(int theSongId, bool stopSong, double theSpeed)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;
		
		if (aMusicInfo->mVolume != 0.0)
		{
			aMusicInfo->mVolumeAdd = -theSpeed;			
		}

		aMusicInfo->mStopOnFade = stopSong;
	}
}

void BassMusicInterface::FadeOutAll(bool stopSong, double theSpeed)
{
	BassMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
				
		aMusicInfo->mVolumeAdd = -theSpeed;
		aMusicInfo->mStopOnFade = stopSong;

		++anItr;
	}
}

void BassMusicInterface::SetVolume(double theVolume)
{
	int aVolume = theVolume * mMaxMusicVolume;
	
	BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, theVolume * 100);
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, theVolume * 100);
}

void BassMusicInterface::SetSongVolume(int theSongId, double theVolume)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolume = theVolume;
		BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, aMusicInfo->mVolume);
	}
}

void BassMusicInterface::SetSongMaxVolume(int theSongId, double theMaxVolume)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolumeCap = theMaxVolume;
		aMusicInfo->mVolume = min(aMusicInfo->mVolume, theMaxVolume);
		BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, aMusicInfo->mVolume);
	}
}

bool BassMusicInterface::IsPlaying(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;
		return BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PLAYING;
	}

	return false;
}

void BassMusicInterface::SetMusicAmplify(int theSongId, double theAmp)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;		
		BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_MUSIC_AMPLIFY, theAmp * 100);
	}
}

void BassMusicInterface::Update()
{
	BassMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;

		if (aMusicInfo->mVolumeAdd != 0.0)
		{
			aMusicInfo->mVolume += aMusicInfo->mVolumeAdd;

			if (aMusicInfo->mVolume > aMusicInfo->mVolumeCap)
			{
				aMusicInfo->mVolume = aMusicInfo->mVolumeCap;
				aMusicInfo->mVolumeAdd = 0.0;
			}
			else if (aMusicInfo->mVolume < 0.0)
			{
				aMusicInfo->mVolume = 0.0;
				aMusicInfo->mVolumeAdd = 0.0;

				if (aMusicInfo->mStopOnFade)
					BASS_ChannelStop(aMusicInfo->GetHandle());
			}

			BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, aMusicInfo->mVolume);
		}

		++anItr;
	}
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// MODs are broken up into several orders or patterns. This returns the current order a song is on.
int BassMusicInterface::GetMusicOrder(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;
		int aPosition = BASS_ChannelGetPosition(aMusicInfo->GetHandle(), BASS_POS_MUSIC_ORDER);
		return aPosition;
	}
	return -1;
}