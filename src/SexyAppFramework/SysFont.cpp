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

static std::string getDefaultFontPath()
{
#ifdef _WIN32
	return "C:\\Windows\\Fonts\\arial.ttf";
#elif __APPLE__
	return "/System/Library/Fonts/Arial.ttf";
#elif __linux__
	return "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf"; 
#else
	return "arial.ttf";
#endif
}

SDL_Surface* renderCharToSurface(TTF_Font* font, char c, SDL_Color color) {
    return TTF_RenderGlyph_Blended(font, c, color);
}

SDL_Texture* createTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(renderer, surface);
}

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

	mTTFFont = TTF_OpenFont(getDefaultFontPath().c_str(), thePointSize);
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
	const int numChars = 256;
	const int charSpacing = 2;
	SDL_Color color = {255, 255, 255, 255};

	int maxCharWidth = 0;
	int totalWidth = 0;

	for (int i = 0; i < numChars; i++) {
		char c = static_cast<char>(i);
		int w = 0;
		int h = 0;
		if (TTF_GetGlyphMetrics(mTTFFont, c, nullptr, nullptr, nullptr, nullptr, &w) == 0) {
			maxCharWidth = max(maxCharWidth, w);
			totalWidth += w + charSpacing;
		}
	}

	int height = mHeight;
	SDL_Surface* fontSurface = SDL_CreateSurface(totalWidth, height, SDL_PIXELFORMAT_RGBA32);
	Uint32 colorKey = SDL_MapRGBA((SDL_PixelFormatDetails*)fontSurface->format, NULL, 0, 0, 0, 0);
	SDL_FillSurfaceRect(fontSurface, nullptr, colorKey);

	std::vector<CharData> charData(256);
	int x = 0;

	for (int i = 0; i < numChars; i++) {
		char c = static_cast<char>(i);
		SDL_Surface* charSurface = TTF_RenderGlyph_Blended(mTTFFont, c, color);
		if (!charSurface) continue;

		SDL_Rect dstRect = { x, 0, charSurface->w, charSurface->h };
		SDL_BlitSurface(charSurface, nullptr, fontSurface, &dstRect);

		charData[i].mImageRect = Rect(x, 0, charSurface->w, charSurface->h);
		charData[i].mWidth = charSurface->w;
		charData[i].mOffset = Point(0, 0);

		x += charSurface->w + charSpacing;
		SDL_DestroySurface(charSurface);
	}

	MemoryImage* image = new MemoryImage(mApp);
	image->Create(fontSurface->w, fontSurface->h);
	SDL_LockSurface(fontSurface);
	memcpy(image->GetBits(), fontSurface->pixels, fontSurface->h * fontSurface->pitch);
	SDL_UnlockSurface(fontSurface);
	SDL_DestroySurface(fontSurface);

	ImageFont* imageFont = new ImageFont(image);
    FontLayer* fontLayer = &imageFont->mFontData->mFontLayerList.back();

    fontLayer->mAscent = mAscent;
    fontLayer->mHeight = mHeight;

    for (int i = 0; i < numChars; ++i) {
        fontLayer->mCharData[i] = charData[i];
    }

    imageFont->GenerateActiveFontLayers();
    imageFont->mActiveListValid = true;

    return imageFont;
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
	if ( !textSurface )
		return;

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FRect dstRect = { theX, theY - mAscent, textSurface->w, textSurface->h };
	SDL_FRect srcRect = { 0, 0, dstRect.w, dstRect.h };
	SDL_DestroySurface(textSurface);

	if (!textTexture)
		return;

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