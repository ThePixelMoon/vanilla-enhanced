//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __PROFILEMGR_H__
#define __PROFILEMGR_H__
#ifdef _WIN32
#pragma once
#endif

#include <map>
#include <string>
#include "SexyAppFramework/Common.h"

class DataSync;
class PlayerInfo;
typedef std::pair<SexyString, PlayerInfo> ProfilePair;
typedef std::map<SexyString, PlayerInfo, Sexy::StringLessNoCase> ProfileMap;

class ProfileMgr
{
protected:
	ProfileMap			mProfileMap;			
	unsigned long		mNextProfileId;			
	unsigned long		mNextProfileUseSeq;		

protected:
	void				SyncState(DataSync& theSync);
	void				DeleteOldestProfile();
	inline void			DeleteOldProfiles() { while(mProfileMap.size() > 200) DeleteOldestProfile(); }

public:
	bool				DeleteProfile(const SexyString& theName);

protected:
	/*inline*/ void		DeleteProfile(ProfileMap::iterator theProfile);

public:
	ProfileMgr() { Clear(); }  
	virtual ~ProfileMgr() { ; }  

	/*inline*/ void		Clear();
	void				Load();
	void				Save();
	inline int			GetNumProfiles() const { return mProfileMap.size(); }
	PlayerInfo*			GetProfile(const SexyString& theName);
	PlayerInfo*			AddProfile(const SexyString& theName);
	PlayerInfo*			GetAnyProfile();
	bool				RenameProfile(const SexyString& theOldName, const SexyString& theNewName);
	inline ProfileMap&	GetProfileMap() { return mProfileMap; }
};

#endif
