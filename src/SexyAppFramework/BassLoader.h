#ifndef __BASSLOADER_H__
#define __BASSLOADER_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#include "bass.h"

#define MAKEMUSICPOS(order,row) (0x80000000|MAKELONG(order,row))

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
namespace Sexy
{

struct BASS_INSTANCE
{
	BASS_INSTANCE(const char *dllName);
	virtual ~BASS_INSTANCE();

	BOOL BASS_MusicSetAmplify(HMUSIC handle, DWORD amp);
	BOOL BASS_MusicPlay(HMUSIC handle);
	BOOL BASS_MusicPlayEx(HMUSIC handle, DWORD pos, int flags, BOOL reset);
	BOOL BASS_ChannelResume(DWORD handle);
	BOOL BASS_StreamPlay(HSTREAM handle, BOOL flush, DWORD flags);

    HMODULE         mModule;
	bool			mVersion2;
};


extern BASS_INSTANCE *gBass;

void LoadBassDLL(); // exits on failure
void FreeBassDLL();

} // namespace Sexy

#endif