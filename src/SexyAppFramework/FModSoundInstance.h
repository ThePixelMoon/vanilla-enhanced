//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __FMODSOUNDINSTANCE_H__
#define __FMODSOUNDINSTANCE_H__
#ifdef _WIN32
#pragma once
#endif

#include "SoundInstance.h"
#include "fmod.h"

namespace Sexy
{

class FModSoundInstance : public SoundInstance
{
public:
	int						mChannelNum;
	FSOUND_STREAM*			mStream;

public:
	FModSoundInstance();
	virtual ~FModSoundInstance();
	virtual void			Release();
		
	virtual void			SetVolume(double theVolume); 
	virtual void			SetPan(int thePosition); //-db to +db = left to right

	void SetBaseVolume( double volume ) override;
	void SetBasePan( int pan ) override;
	void AdjustPitch( double pitch ) override;
	double GetVolume() override;

	virtual bool			Play(bool looping, bool autoRelease);
	virtual void			Stop();
	virtual bool			IsPlaying();
	virtual bool			IsReleased();
};

}

#endif //__FMODSOUNDINSTANCE_H__