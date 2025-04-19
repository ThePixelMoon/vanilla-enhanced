//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __FMODSOUNDMANAGER_H__
#define __FMODSOUNDMANAGER_H__
#ifdef _WIN32
#pragma once
#endif

#include "SoundManager.h"
#include "FModSoundInstance.h"
#include "fmod.h"

namespace Sexy
{

class FModSoundManager : public SoundManager
{
public:
	double					mMasterVolume;
	FSOUND_STREAM*			mSourceStreams[MAX_SOURCE_SOUNDS];

public:
	FModSoundManager(HWND theWindow);
	virtual ~FModSoundManager();

	virtual bool			Initialized();

	virtual bool			LoadSound(unsigned int theSfxID, const std::string& theFilename);
	virtual bool			LoadSound(const std::string& theFilename);
	virtual void			SetVolume(double theVolume);

	virtual SoundInstance*	GetSoundInstance(unsigned int theSfxID);

	virtual void			ReleaseSounds();
	virtual void			ReleaseChannels();

	virtual double			GetMasterVolume();
	virtual void			SetMasterVolume(double theVolume);

	virtual void			Flush();
	virtual void			SetCooperativeWindow(HWND theHWnd, bool isWindowed);
};

}

#endif //__FMODSOUNDMANAGER_H__