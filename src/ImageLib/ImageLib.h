//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __IMAGELIB_H__
#define __IMAGELIB_H__
#ifdef _WIN32
#pragma once
#endif

#ifndef XMD_H
#define XMD_H
#endif

#include <string>

namespace ImageLib
{

class Image
{
public:
	int						mWidth;
	int						mHeight;
	unsigned long*			mBits;

public:
	Image();
	virtual ~Image();

	int						GetWidth();
	int						GetHeight();
	unsigned long*			GetBits();
};

bool WriteJPEGImage(const std::string& theFileName, Image* theImage);
bool WritePNGImage(const std::string& theFileName, Image* theImage);
bool WriteTGAImage(const std::string& theFileName, Image* theImage);
bool WriteBMPImage(const std::string& theFileName, Image* theImage);
extern int gAlphaComposeColor;
extern bool gAutoLoadAlpha;

Image* GetImage(const std::string& theFileName, bool lookForAlphaImage = true);

}

#endif //__IMAGELIB_H__