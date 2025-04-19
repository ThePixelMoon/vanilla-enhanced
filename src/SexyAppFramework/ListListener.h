//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __LISTLISTENER_H__
#define __LISTLISTENER_H__
#ifdef _WIN32
#pragma once
#endif

namespace Sexy 
{

class ListListener 
{
public:
	virtual void ListClicked(int theId, int theIdx, int theClickCount) {}
	virtual void ListClosed(int theId) {}
	virtual void ListHiliteChanged(int theId, int theOldIdx, int theNewIdx) {}
};

}

#endif //__LISTLISTENER_H__
