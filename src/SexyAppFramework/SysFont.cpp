//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#include "SysFont.h"
#include "SDLImage.h"
#include "SexyAppBase.h"
#include "Graphics.h"
#include "ImageFont.h"
#include "MemoryImage.h"
#include "SDLInterface.h"
#include "WidgetManager.h"
#include <stdlib.h>

using namespace Sexy;

SysFont::SysFont(const std::string& theFace, int thePointSize, bool bold, bool italics, bool underline)
{
	Init(gSexyAppBase,theFace,thePointSize,ANSI_CHARSET,bold,italics,underline,false);
}

SysFont::SysFont(SexyAppBase* theApp, const std::string& theFace, int thePointSize, int theScript, bool bold, bool italics, bool underline)
{
	Init(theApp,theFace,thePointSize,theScript,bold,italics,underline,true);
}

void SysFont::Init(SexyAppBase* theApp, const std::string& theFace, int thePointSize, int theScript, bool bold, bool italics, bool underline, bool useDevCaps)
{
	mApp = theApp;

	mTTFFont = TTF_OpenFont("tahoma.ttf", thePointSize);
	if (!mTTFFont) {
		mApp->mSDLInterface->MakeSimpleMessageBox("Error", SDL_GetError(), SDL_MESSAGEBOX_ERROR);
	}

	TTF_SetFontStyle(mTTFFont, (bold ? TTF_STYLE_BOLD : 0) | (italics ? TTF_STYLE_ITALIC : 0) | (underline ? TTF_STYLE_UNDERLINE : 0));

	mAscent = TTF_GetFontAscent(mTTFFont);
	mHeight = TTF_GetFontHeight(mTTFFont);

	mDrawShadow = false;
	mSimulateBold = false;
}

SysFont::SysFont(const SysFont& theSysFont)
{
	mTTFFont = theSysFont.mTTFFont;
	mApp = theSysFont.mApp;
	mHeight = theSysFont.mHeight;
	mAscent = theSysFont.mAscent;

	mDrawShadow = false;
	mSimulateBold = false;
}

SysFont::~SysFont()
{
	TTF_CloseFont(mTTFFont);
}

ImageFont* SysFont::CreateImageFont()
{
	/*
	int i;
	MemoryImage*			anImage;
	int anImageCharWidth, anImageXOff, anImageYOff;

	////////////////////////////////////////////////////
	// Step 1: Create image 
	anImageCharWidth = CharWidth('W')*2;
	anImageXOff = anImageCharWidth/4;
	anImageYOff = mHeight/2;
	int aWidth = 257*anImageCharWidth;
	int aHeight = mHeight*2;

	BITMAPINFO aBitmapInfo;
	memset(&aBitmapInfo,0,sizeof(aBitmapInfo));
	BITMAPINFOHEADER &aHeader = aBitmapInfo.bmiHeader;
	aHeader.biSize = sizeof(aHeader);
	aHeader.biWidth = aWidth;
	aHeader.biHeight = -aHeight;
	aHeader.biPlanes = 1;
	aHeader.biBitCount = 32;
	aHeader.biCompression = BI_RGB;

	HDC aDC = CreateCompatibleDC(NULL);

	DWORD *aBits = NULL;
	HBITMAP aBitmap = CreateDIBSection(aDC,&aBitmapInfo,DIB_RGB_COLORS,(void**)&aBits,NULL,0);

	HBITMAP anOldBitmap = (HBITMAP)SelectObject(aDC,aBitmap);
	HFONT anOldFont = (HFONT)SelectObject(aDC,mHFont);

	HBRUSH anOldBrush = (HBRUSH)SelectObject(aDC,GetStockObject(BLACK_BRUSH));
	Rectangle(aDC,0,0,aWidth,aHeight);

	SetBkMode(aDC, TRANSPARENT); 	
	SetTextColor(aDC, RGB(255,255,255));
				
	int xpos = anImageXOff;
	int ypos = anImageYOff;
	for (i=0; i<256; i++)
	{
		char aChar = i;
		TextOutA(aDC,xpos,ypos,&aChar,1);
		xpos += anImageCharWidth;
	}
	GdiFlush();


	SelectObject(aDC,anOldBrush);
	SelectObject(aDC,anOldBitmap);
	SelectObject(aDC,anOldFont);

	int aSize = aWidth*aHeight;
	anImage = new MemoryImage(mApp);
	anImage->Create(aWidth,aHeight);
	DWORD *src = aBits;
	DWORD *dst = anImage->GetBits();
	for (i=0; i<aSize; i++)
	{
		DWORD anAlpha = ((*src++)&0xff)<<24;
		*dst++ = anAlpha | 0xFFFFFF;
	}
	anImage->BitsChanged();
	DeleteObject(aBitmap);


	////////////////////////////////////////////////////
	// Step 2: Create image font

	ImageFont *aFont = new ImageFont(anImage);
	FontLayer *aFontLayer = &aFont->mFontData->mFontLayerList.back();

	aFontLayer->mAscent = mAscent;
	aFontLayer->mHeight = mHeight;

	for (i=0; i<256; i++)
	{
		char aChar = i;

		aFontLayer->mCharData[(uchar) aChar].mImageRect = Rect(aChar*anImageCharWidth,0,anImageCharWidth,anImage->mHeight);
		aFontLayer->mCharData[(uchar) aChar].mWidth = CharWidth(aChar);
		aFontLayer->mCharData[(uchar) aChar].mOffset = Point(-anImageXOff,-anImageYOff);
	}

	aFont->GenerateActiveFontLayers();
	aFont->mActiveListValid = true;

	return aFont;
	*/
	return nullptr; //TODO: implement
}

int	SysFont::StringWidth(const SexyString& theString)
{
	int w = 0;
	TTF_GetStringSize(mTTFFont, theString.c_str(), NULL, &w, nullptr);

	return w;
}

void SysFont::DrawString(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, const Rect& theClipRect)
{
	SDL_Renderer* renderer = mApp->mSDLInterface->mRenderer;
	SDL_Color aColor = { theColor.mRed, theColor.mGreen, theColor.mBlue , theColor.mAlpha };
	SDL_Surface* textSurface = TTF_RenderText_Blended(mTTFFont, theString.c_str(), NULL, aColor);
	if (!textSurface) {
		//throw std::runtime_error("Failed to render text: " + std::string(TTF_GetError()));
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FRect dstRect = { theX, theY - mAscent, textSurface->w, textSurface->h };
	SDL_FRect srcRect = { 0, 0, dstRect.w, dstRect.h };
	SDL_DestroySurface(textSurface);

	if (!textTexture) {
		//throw std::runtime_error("Failed to create texture from surface: " + std::string(SDL_GetError()));
	}

	if (mDrawShadow) {
		SDL_FRect shadowRect = { theX + 1, theY - mAscent + 1, dstRect.w, dstRect.h };
		mApp->mSDLInterface->BltTexture(textTexture, theX, theY, srcRect, shadowRect, Color(0,0,0), g->GetDrawMode());
	}

	mApp->mSDLInterface->BltTexture(textTexture, theX, theY, srcRect, dstRect, theColor, g->GetDrawMode());

	SDL_DestroyTexture(textTexture);
}

Font* SysFont::Duplicate()
{
	return new SysFont(*this);
}