﻿//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#include "SDLInterface.h"
#include "SDLImage.h"
#include "SexyAppBase.h"
#include "AutoCrit.h"
#include "CritSect.h"
#include "Graphics.h"
#include "MemoryImage.h"
#include <SDL3_ttf/SDL_ttf.h>

using namespace Sexy;

SDL_FPoint TransformToPoint(float x, float y, const SexyMatrix3& m, float aTransX = 0, float aTransY = 0) {
    SDL_FPoint result;
    result.x = m.m00 * x + m.m01 * y + m.m02 + aTransX;
    result.y = m.m10 * x + m.m11 * y + m.m12 + aTransY;
    return result;
}

SDLInterface::SDLInterface(SexyAppBase* theApp)
{
    mApp = theApp;
    mWidth = mApp->mWidth;
    mHeight = mApp->mHeight;
    mDisplayWidth = mWidth;
    mDisplayHeight = mHeight;
    mPresentationRect = Rect(0, 0, mWidth, mHeight);
    mScreenImage = NULL;
    mHasInitiated = false;
    mCursorX = 0;
    mCursorY = 0;
    mIs3D = false;
    mMillisecondsPerFrame = 0;
    mNextCursorX = 0;
    mNextCursorY = 0;
    mRefreshRate = 0;
    mRenderer = nullptr;
    mScreenTexture = nullptr;
    mWindow = nullptr;
}


SDLInterface::~SDLInterface()
{
	Cleanup();
	SDL_Quit();
}

void SDLInterface::Cleanup()
{
	ImageSet::iterator anItr;
	for (anItr = mImageSet.begin(); anItr != mImageSet.end(); ++anItr)
	{
		MemoryImage* anImage = *anItr;
		SDLTextureData*aData = (SDLTextureData*)anImage->mD3DData;
		delete aData;
		anImage->mD3DData = NULL;
	}
	mImageSet.clear();

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mHasInitiated = false;
}

void SDLInterface::AddSDLImage(SDLImage* theSDLImage)
{
	AutoCrit anAutoCrit(mCritSect);

	mSDLImageSet.insert(theSDLImage);
}

void SDLInterface::RemoveSDLImage(SDLImage* theSDLImage)
{
	AutoCrit anAutoCrit(mCritSect);

	SDLImageSet::iterator anItr = mSDLImageSet.find(theSDLImage);
	if (anItr != mSDLImageSet.end())
		mSDLImageSet.erase(anItr);
}

void SDLInterface::Remove3DData(MemoryImage* theImage)
{
	if (theImage->mD3DData != NULL)
	{
		delete (SDLTextureData*)theImage->mD3DData;
		theImage->mD3DData = NULL;

		AutoCrit aCrit(mCritSect); // Make images thread safe
		mImageSet.erase(theImage);
	}
}

SDLImage* SDLInterface::GetScreenImage()
{
	return mScreenImage;
}

void SDLInterface::UpdateViewport() {
    if (SDL_GetCurrentThreadID() != SDL_GetThreadID(nullptr))
        return;

    int windowWidth, windowHeight;
    if (SDL_GetWindowSize(mWindow, &windowWidth, &windowHeight) != 0)
        return;

    if (!SDL_SetRenderLogicalPresentation(mRenderer, windowWidth, windowHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX))
        return;

    mPresentationRect = Rect(0, 0, windowWidth, windowHeight);
}

int SDLInterface::Init(bool IsWindowed)
{
	int aResult = RESULT_OK;

	if (mHasInitiated)
		Cleanup();

	mRGBBits = 32;

	mRedBits = 8;
	mGreenBits = 8;
	mBlueBits = 8;

	mRedShift = 0;
	mGreenShift = 8;
	mBlueShift = 16;

	mRedMask = (0xFFU << mRedShift);
	mGreenMask = (0xFFU << mGreenShift);
	mBlueMask = (0xFFU << mBlueShift);

	aResult = InitSDLWindow(IsWindowed) ? aResult : RESULT_FAIL;
	mHasInitiated = true;
	return aResult;
}

bool SDLInterface::InitSDLWindow(bool IsWindowed)
{
	if (!SDL_Init(SDL_INIT_VIDEO) || !TTF_Init())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Initialization Failed", SDL_GetError(), nullptr);
		return false;
	}

	int aWindowFlags = IsWindowed ? 0 : SDL_WINDOW_FULLSCREEN;

	mWindow = SDL_CreateWindow(mApp->mTitle.c_str(), mWidth, mHeight, aWindowFlags);
	if (mWindow == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window Creation Failed", SDL_GetError(), nullptr);
		return false;
	}

	if (!IsWindowed)
		SDL_SetWindowFullscreen(mWindow, true);

	SDL_StartTextInput(mWindow);

	return InitSDLRenderer();
}

bool SDLInterface::InitSDLRenderer()
{
	mRenderer = SDL_CreateRenderer(mWindow, NULL);
	if (mRenderer == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Renderer Creation Failed", SDL_GetError(), nullptr);
		return false;
	}

	mScreenTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mWidth, mHeight);
	if (mScreenTexture == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Screen Texture-Buffer Creation Failed", SDL_GetError(), nullptr);
		return false;
	}

	const SDL_DisplayMode* aMode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(mWindow));
	mRefreshRate = aMode->refresh_rate;
	if (!mRefreshRate) mRefreshRate = 60;
	mMillisecondsPerFrame = 1000 / mRefreshRate;

	SetVideoOnlyDraw(false);

	UpdateViewport();

	return true;
}

bool gSDLInterfacePreDrawError = false;
bool SDLInterface::Redraw(Rect* theClipRect)
{
    SDL_SetRenderTarget(mRenderer, nullptr);

    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
    SDL_SetTextureBlendMode(mScreenTexture, SDL_BLENDMODE_BLEND);
    SDL_RenderClear(mRenderer);

    SDL_Rect clipRect = theClipRect
        ? SDL_Rect{theClipRect->mX, theClipRect->mY, theClipRect->mWidth, theClipRect->mHeight}
        : SDL_Rect{mPresentationRect.mX, mPresentationRect.mY, mPresentationRect.mWidth, mPresentationRect.mHeight};

    SDL_SetRenderClipRect(mRenderer, &clipRect);

    gSDLInterfacePreDrawError = (SDL_RenderTexture(mRenderer, mScreenTexture, nullptr, nullptr) < 0);

    SDL_RenderPresent(mRenderer);

    return !gSDLInterfacePreDrawError;
}

/// <summary>
/// Setup the mScreenImage
/// </summary>
/// <param name="videoOnly"></param>
void SDLInterface::SetVideoOnlyDraw(bool videoOnly)
{
	if (mScreenImage) delete mScreenImage;
	mScreenImage = new SDLImage(this);
	//mScreenImage->SetSurface(useSecondary ? mSecondarySurface : mDrawSurface);		
	//mScreenImage->mNoLock = mVideoOnlyDraw;
	//mScreenImage->mVideoMemory = mVideoOnlyDraw;
	mScreenImage->mWidth = mWidth;
	mScreenImage->mHeight = mHeight;
	mScreenImage->SetImageMode(false, false);
}

/// <summary>
/// Set the next cursor position
/// </summary>
/// <param name="theCursorX"></param>
/// <param name="theCursorY"></param>
void SDLInterface::SetCursorPos(int theCursorX, int theCursorY)
{
	mNextCursorX = theCursorX;
	mNextCursorY = theCursorY;
}

/// <summary>
/// Set the cursor image to a Image* or NULL to hide the cursor
/// </summary>
/// <param name="theImage"></param>
/// <returns></returns>
bool SDLInterface::SetCursorImage(Image* theImage)
{
	if (theImage == NULL)
	{
		SDL_SetCursor(nullptr);
		return true;
	}
	MemoryImage* aMemoryImage = (MemoryImage*)theImage;

	SDL_Surface* aSurface = SDL_CreateSurfaceFrom(aMemoryImage->mWidth, aMemoryImage->mHeight, SDL_PIXELFORMAT_ARGB8888, aMemoryImage->GetBits(), aMemoryImage->mWidth * sizeof(ulong));

	SDL_Cursor* aCursor = SDL_CreateColorCursor(aSurface, 0, 0);
	
	SDL_SetCursor(aCursor);

	SDL_DestroySurface(aSurface);
	SDL_DestroyCursor(aCursor);

	return true;
}

void SDLInterface::SetCursor(SDL_SystemCursor theCursorType)
{
	SDL_Cursor* aCursor = SDL_CreateSystemCursor(theCursorType);
	SDL_SetCursor(aCursor);
	SDL_DestroyCursor(aCursor);
}

void SDLInterface::MakeSimpleMessageBox(const char* theTitle, const char* theMessage, SDL_MessageBoxFlags flags)
{
	SDL_ShowSimpleMessageBox(flags, theTitle, theMessage, mWindow);
}

void SDLInterface::PushTransform(const SexyMatrix3& theTransform, bool concatenate)
{
	if (mTransformStack.empty() || !concatenate)
		mTransformStack.push_back(theTransform);
	else
	{
		SexyMatrix3& aTrans = mTransformStack.back();
		mTransformStack.push_back(theTransform * aTrans);
	}
}

void SDLInterface::PopTransform()
{
	if (!mTransformStack.empty())
		mTransformStack.pop_back();
}

bool SDLInterface::PreDraw()
{
	return true;
}

bool SDLInterface::CreateImageTexture(MemoryImage* theImage)
{
	bool wantPurge = false;

	if (theImage->mD3DData == NULL)
	{
		theImage->mD3DData = new SDLTextureData(mRenderer);

		// The actual purging was deferred
		wantPurge = theImage->mPurgeBits;

		AutoCrit aCrit(mCritSect); // Make images thread safe
		mImageSet.insert(theImage);
	}

	SDLTextureData* aData = static_cast<SDLTextureData*>(theImage->mD3DData);
	aData->CheckCreateTextures(theImage);

	if (wantPurge)
		theImage->PurgeBits();

	return true;
}

bool SDLInterface::RecoverBits(MemoryImage* theImage)
{
	if (theImage->mD3DData == NULL)
		return false;

	SDLTextureData* aData = (SDLTextureData*)theImage->mD3DData;
	if (aData->mBitsChangedCount != theImage->mBitsChangedCount) // bits have changed since texture was created
		return false;

	// Reverse the process: copy texture data to theImage
	float aWidth;
	float aHeight;
	void* pixels;
	int pitch;

	if (SDL_LockTexture(aData->mTexture, NULL, &pixels, &pitch) && SDL_GetTextureSize(aData->mTexture, &aWidth, &aHeight))
	{
		theImage->SetBits((ulong*)pixels, (int)aWidth, (int)aHeight);
		SDL_UnlockTexture(aData->mTexture);
	}

	return true;
}


SDL_BlendMode SDLInterface::ChooseBlendMode(int theBlendMode)
{
	SDL_BlendMode theSDLBlendMode;
	switch (theBlendMode)
	{
	case Graphics::DRAWMODE_ADDITIVE:
		theSDLBlendMode = SDL_BLENDMODE_ADD;
		break;
	default:
	case Graphics::DRAWMODE_NORMAL:
		theSDLBlendMode = SDL_BLENDMODE_BLEND;
		break;
	}
	return theSDLBlendMode;
}


SDLTextureData::SDLTextureData(SDL_Renderer* theRenderer)
{
	mWidth = 0;
	mHeight = 0;
	mBitsChangedCount = 0;
	mRenderer = theRenderer;
	mTexture = nullptr;
}

SDLTextureData::~SDLTextureData()
{
	ReleaseTextures();
}

void SDLTextureData::ReleaseTextures()
{
	if (mTexture != nullptr)
		SDL_DestroyTexture(mTexture);
}

void SDLTextureData::CreateTextures(MemoryImage* theImage)
{
    theImage->DeleteSWBuffers(); // we don't need the software buffers anymore
    theImage->CommitBits();

    bool createTexture = false;

    // only recreate the texture if the dimensions or image data have changed
    if (mWidth != theImage->mWidth || mHeight != theImage->mHeight || mBitsChangedCount != theImage->mBitsChangedCount)
    {
        ReleaseTextures();
        createTexture = true;
    }

    int aWidth = theImage->GetWidth();
    int aHeight = theImage->GetHeight();

    if (createTexture)
    {
        mTexture = SDL_CreateTexture(
            mRenderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STATIC,
            aWidth,
            aHeight
        );

        if (mTexture)
        {
            const char* rendererName = SDL_GetRendererName(mRenderer);

            SDL_SetTextureScaleMode(
                mTexture,
                (rendererName && strcmp(rendererName, "software") == 0)
                    ? SDL_SCALEMODE_NEAREST
                    : SDL_SCALEMODE_LINEAR
            );

            void* bits = theImage->GetBits();
            if (bits)
            {
                SDL_UpdateTexture(
                    mTexture,
                    nullptr,
                    bits,
                    aWidth * SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_ARGB8888)
                );
            }
            else
            {
                SDL_Log("Error: Image bits are NULL, cannot update texture.");
            }
        }
        else
        {
            SDL_Log("Failed to create texture: %s", SDL_GetError());
        }
    }
    else if (mBitsChangedCount != theImage->mBitsChangedCount)
    {
        void* bits = theImage->GetBits();
        if (bits)
        {
            SDL_UpdateTexture(
                mTexture,
                nullptr,
                bits,
                aWidth * SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_ARGB8888)
            );
        }
        else
        {
            SDL_Log("Error: Image bits are NULL, cannot update texture.");
        }
    }

    mWidth = theImage->mWidth;
    mHeight = theImage->mHeight;
    mBitsChangedCount = theImage->mBitsChangedCount;
}

void SDLTextureData::CheckCreateTextures(MemoryImage* theImage)
{
	if (mTexture != nullptr)
	{
		if (mWidth != theImage->mWidth || mHeight != theImage->mHeight || mBitsChangedCount != theImage->mBitsChangedCount)
			CreateTextures(theImage);
		return;
	}
	CreateTextures(theImage);
}

/////////////////////////////////////////////////////////////////
///				DRAWING/BLITTING FUNCTIONS		    	   //////
/////////////////////////////////////////////////////////////////

void SDLInterface::Blt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode, bool linearFilter)
{
	MemoryImage* memImg = static_cast<MemoryImage*>(theImage);
    if (!CreateImageTexture(memImg))
        return;

    SDLTextureData* texData = static_cast<SDLTextureData*>(memImg->mD3DData);
    SDL_Texture* texture    = texData->mTexture;

    SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_SetTextureColorMod(texture,
                           theColor.GetRed(),
                           theColor.GetGreen(),
                           theColor.GetBlue());
    SDL_SetTextureAlphaMod(texture,
                           theColor.GetAlpha());
    SDL_SetTextureBlendMode(texture,
                            ChooseBlendMode(theDrawMode));

    SDL_FRect srcF = {
        (float)theSrcRect.mX,
        (float)theSrcRect.mY,
        (float)theSrcRect.mWidth,
        (float)theSrcRect.mHeight
    };
    SDL_FRect dstF = {
		(float)theX,
		(float)theY,
        (float)theSrcRect.mWidth,
        (float)theSrcRect.mHeight
    };

    bool ok = SDL_RenderTexture(mRenderer, texture, &srcF, &dstF);
	if (ok)
		SDL_SetRenderTarget(mRenderer, nullptr);
	
    return;
}

void SDLInterface::BltClipF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect* theClipRect, const Color& theColor, int theDrawMode)
{
	MemoryImage* aSrcMemoryImage = (MemoryImage*)theImage;

	if (!CreateImageTexture(aSrcMemoryImage))
		return;

	SDLTextureData* aData = (SDLTextureData*)aSrcMemoryImage->mD3DData;

	SDL_SetRenderTarget(mRenderer, mScreenTexture);

	SDL_Texture* aTexture = aData->mTexture;
	SDL_SetTextureColorMod(aTexture, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue());
	SDL_SetTextureAlphaMod(aTexture, theColor.GetAlpha());
	SDL_SetTextureScaleMode(aTexture,
							theDrawMode ? SDL_SCALEMODE_LINEAR
										: SDL_SCALEMODE_NEAREST);

	SDL_FRect destRect = { theX, theY, theSrcRect.mWidth, theSrcRect.mHeight };
	SDL_Rect clipRect;
	if (theClipRect == nullptr)
		clipRect = { 0,0,theImage->mWidth, theImage->mHeight };
	else
		clipRect = { theClipRect->mX, theClipRect->mY, theClipRect->mWidth, theClipRect->mHeight };
	SDL_FRect srcRect = { theSrcRect.mX, theSrcRect.mY, theSrcRect.mWidth, theSrcRect.mHeight };

	SDL_SetRenderClipRect(mRenderer, &clipRect);
	SDL_SetTextureBlendMode(aTexture, ChooseBlendMode(theDrawMode));
	SDL_RenderTexture(mRenderer, aTexture, &srcRect, &destRect);
	SDL_SetRenderClipRect(mRenderer, NULL);
	SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::BltMirror(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode, bool linearFilter)
{
    MemoryImage* aSrcMemoryImage = (MemoryImage*)theImage;

    if (!CreateImageTexture(aSrcMemoryImage))
        return;

    SDLTextureData* aData = (SDLTextureData*)aSrcMemoryImage->mD3DData;

    SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_Texture* aTexture = aData->mTexture;
    SDL_SetTextureColorMod(aTexture, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue());
    SDL_SetTextureAlphaMod(aTexture, theColor.GetAlpha());

    SDL_FRect destRect = { theX, theY, theSrcRect.mWidth, theSrcRect.mHeight };
    SDL_FRect srcRect = { theSrcRect.mX, theSrcRect.mY, theSrcRect.mWidth, theSrcRect.mHeight };

    SDL_SetTextureBlendMode(aTexture, ChooseBlendMode(theDrawMode));

    SDL_RenderTextureRotated(mRenderer, aTexture, &srcRect, &destRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::StretchBlt(Image* theImage,
                              const Rect& theDestRect,
                              const Rect& theSrcRect,
                              const Rect* theClipRect,
                              const Color& theColor,
                              int theDrawMode,
                              bool fastStretch,
                              bool mirror)
{
    MemoryImage* aSrcMemoryImage = static_cast<MemoryImage*>(theImage);
    if (!CreateImageTexture(aSrcMemoryImage))
        return;

    SDLTextureData* aData = static_cast<SDLTextureData*>(aSrcMemoryImage->mD3DData);
    SDL_Texture* aTexture = aData->mTexture;

    SDL_SetRenderTarget(mRenderer, mScreenTexture);
    SDL_SetTextureColorMod(aTexture, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue());
    SDL_SetTextureAlphaMod(aTexture, theColor.GetAlpha());
    SDL_SetTextureScaleMode(aTexture,
        fastStretch ? SDL_SCALEMODE_NEAREST
                    : SDL_SCALEMODE_LINEAR);

    SDL_FRect destRect = {
        (float)theDestRect.mX,
        (float)theDestRect.mY,
        (float)theDestRect.mWidth,
        (float)theDestRect.mHeight
    };

    SDL_Rect clipRect;
    if (theClipRect == nullptr) {
        clipRect = { 0, 0, theImage->mWidth, theImage->mHeight };
    } else {
        clipRect = { theClipRect->mX,
                     theClipRect->mY,
                     theClipRect->mWidth,
                     theClipRect->mHeight };
    }

    SDL_FRect srcRect = {
        (float)theSrcRect.mX,
        (float)theSrcRect.mY,
        (float)theSrcRect.mWidth,
        (float)theSrcRect.mHeight
    };

    SDL_SetRenderClipRect(mRenderer, &clipRect);
    SDL_SetTextureBlendMode(aTexture, ChooseBlendMode(theDrawMode));
    SDL_RenderTextureRotated(mRenderer,
                             aTexture,
                             &srcRect,
                             &destRect,
                             0.0,
                             nullptr,
                             mirror ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    SDL_SetRenderClipRect(mRenderer, nullptr);
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::BltRotated(Image* theImage,
                              float theX,
                              float theY,
                              const Rect* theClipRect,
                              const Color& theColor,
                              int theDrawMode,
                              double theRot,
                              float theRotCenterX,
                              float theRotCenterY,
                              const Rect& theSrcRect)
{
    MemoryImage* aSrcMemoryImage = static_cast<MemoryImage*>(theImage);
    if (!CreateImageTexture(aSrcMemoryImage))
        return;

    SDLTextureData* aData = static_cast<SDLTextureData*>(aSrcMemoryImage->mD3DData);
    SDL_Texture* aTexture = aData ? aData->mTexture : nullptr;
    if (!aTexture)
        return;

    SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_SetTextureColorMod(aTexture,
                           theColor.GetRed(),
                           theColor.GetGreen(),
                           theColor.GetBlue());
    SDL_SetTextureAlphaMod(aTexture,
                           theColor.GetAlpha());

    SDL_FRect destRect = {
        theX,
        theY,
        static_cast<float>(theSrcRect.mWidth),
        static_cast<float>(theSrcRect.mHeight)
    };
    SDL_FRect srcRect = {
        static_cast<float>(theSrcRect.mX),
        static_cast<float>(theSrcRect.mY),
        static_cast<float>(theSrcRect.mWidth),
        static_cast<float>(theSrcRect.mHeight)
    };

    SDL_Rect clipRect;
    if (theClipRect)
    {
        clipRect = { theClipRect->mX,
                     theClipRect->mY,
                     theClipRect->mWidth,
                     theClipRect->mHeight };
    }
    else
    {
        clipRect = { 0, 0, theImage->mWidth, theImage->mHeight };
    }

    SDL_FPoint rotationCenter = { theRotCenterX, theRotCenterY };

    SDL_SetRenderClipRect(mRenderer, &clipRect);
    SDL_SetTextureBlendMode(aTexture, ChooseBlendMode(theDrawMode));
    SDL_RenderTextureRotated(mRenderer,
                             aTexture,
                             &srcRect,
                             &destRect,
                             theRot,
                             &rotationCenter,
                             SDL_FLIP_NONE);
    SDL_SetRenderClipRect(mRenderer, nullptr);
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::BltTransformed(Image* theImage,
                                  const Rect* theClipRect,
                                  const Color& theColor,
                                  int theDrawMode,
                                  const Rect& theSrcRect,
                                  const SexyMatrix3& theTransform,
                                  bool linearFilter,
                                  float theX,
                                  float theY,
                                  bool center)
{
    MemoryImage* aSrcMemoryImage = static_cast<MemoryImage*>(theImage);

    if (!CreateImageTexture(aSrcMemoryImage))
        return;

    SDLTextureData* aData = static_cast<SDLTextureData*>(aSrcMemoryImage->mD3DData);
    if (!aData || !aData->mTexture)
        return;

    SDL_Texture* aTexture = aData->mTexture;
    SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_SetTextureColorMod(aTexture, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue());
    SDL_SetTextureAlphaMod(aTexture, theColor.GetAlpha());

    SDL_Rect clipRect = theClipRect
                        ? SDL_Rect{ theClipRect->mX, theClipRect->mY, theClipRect->mWidth, theClipRect->mHeight }
                        : SDL_Rect{ 0, 0, theImage->mWidth, theImage->mHeight };

    SDL_SetTextureBlendMode(aTexture, ChooseBlendMode(theDrawMode));

    float halfWidth = theSrcRect.mWidth * 0.5f;
    float halfHeight = theSrcRect.mHeight * 0.5f;

    float x1 = center ? -halfWidth : 0;
    float y1 = center ? -halfHeight : 0;
    float x2 = x1 + theSrcRect.mWidth;
    float y2 = y1;
    float x3 = x1;
    float y3 = y1 + theSrcRect.mHeight;
    float x4 = x2;
    float y4 = y3;

    float u1 = static_cast<float>(theSrcRect.mX) / theImage->mWidth;
    float v1 = static_cast<float>(theSrcRect.mY) / theImage->mHeight;
    float u2 = static_cast<float>(theSrcRect.mX + theSrcRect.mWidth) / theImage->mWidth;
    float v2 = static_cast<float>(theSrcRect.mY + theSrcRect.mHeight) / theImage->mHeight;

    SDL_FColor aColor = {
        theColor.GetRed() / 255.0f,
        theColor.GetGreen() / 255.0f,
        theColor.GetBlue() / 255.0f,
        theColor.GetAlpha() / 255.0f
    };

    SDL_Vertex vertices[4] = {
        { TransformToPoint(x1, y1, theTransform, theX, theY), aColor, {u1, v1} }, // TL
        { TransformToPoint(x2, y2, theTransform, theX, theY), aColor, {u2, v1} }, // TR
        { TransformToPoint(x3, y3, theTransform, theX, theY), aColor, {u1, v2} }, // BL
        { TransformToPoint(x4, y4, theTransform, theX, theY), aColor, {u2, v2} }  // BR
    };

    int indices[] = { 0, 1, 2, 1, 3, 2 };

    SDL_SetRenderClipRect(mRenderer, &clipRect);
    SDL_RenderGeometry(mRenderer, aTexture, vertices, 4, indices, 6);
    SDL_SetRenderClipRect(mRenderer, nullptr);
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::DrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode)
{
	if (!mRenderer)
        return;

	SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_SetRenderDrawBlendMode(mRenderer, ChooseBlendMode(theDrawMode));
    SDL_SetRenderDrawColor(mRenderer, theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);

    SDL_RenderLine(mRenderer, theStartX, theStartY, theEndX, theEndY);

    SDL_SetRenderDrawBlendMode(mRenderer, ChooseBlendMode(Graphics::DRAWMODE_NORMAL));
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::FillRect(const Rect& theRect, const Color& theColor, int theDrawMode)
{
	if (!mRenderer)
        return;

	SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_FRect theSDLRect = { theRect.mX, theRect.mY, theRect.mWidth, theRect.mHeight };
	
    SDL_SetRenderDrawColor(mRenderer, theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);

    SDL_SetRenderDrawBlendMode(mRenderer, ChooseBlendMode(theDrawMode));
    SDL_RenderFillRect(mRenderer, &theSDLRect);

    SDL_SetRenderDrawBlendMode(mRenderer, ChooseBlendMode(Graphics::DRAWMODE_NORMAL));
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::DrawTriangle(const TriVertex& p1, const TriVertex& p2, const TriVertex& p3, const Color& theColor, int theDrawMode)
{
    SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_FColor aColor = { theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue(), theColor.GetAlpha() };

    int indices[] = { 0, 1, 2 }; 

    SDL_Vertex vertices[3] = {
        { SDL_FPoint {p1.x, p1.y}, aColor, {p1.u, p1.v} },
        { SDL_FPoint {p2.x, p2.y}, aColor, {p2.u, p2.v} },
        { SDL_FPoint {p3.x, p3.y}, aColor, {p3.u, p3.v} }
    };

    SDL_RenderGeometry(mRenderer, NULL, vertices, 3, indices, 3);
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::DrawTriangleTex(const TriVertex& p1, const TriVertex& p2, const TriVertex& p3, const Color& theColor, int theDrawMode, Image* theTexture, bool blend)
{
    MemoryImage* aSrcMemoryImage = (MemoryImage*)theTexture;

    if (!CreateImageTexture(aSrcMemoryImage))
        return;

    SDLTextureData* aData = (SDLTextureData*)aSrcMemoryImage->mD3DData;

    SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_Texture* aTexture = aData->mTexture;
    SDL_SetTextureColorMod(aTexture, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue());
    SDL_SetTextureAlphaMod(aTexture, theColor.GetAlpha());
    SDL_SetTextureBlendMode(aTexture, ChooseBlendMode(theDrawMode));

    SDL_FColor aColor = { theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue(), theColor.GetAlpha() };

    int indices[] = { 0, 1, 2 };

    SDL_Vertex vertices[3] = {
        { SDL_FPoint {p1.x, p1.y}, aColor, {p1.u, p1.v} },
        { SDL_FPoint {p2.x, p2.y}, aColor, {p2.u, p2.v} },
        { SDL_FPoint {p3.x, p3.y}, aColor, {p3.u, p3.v} }
    };

    SDL_RenderGeometry(mRenderer, aTexture, vertices, 3, indices, 3);
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::DrawTrianglesTex(const TriVertex theVertices[][3], int theNumTriangles, const Color& theColor, int theDrawMode, Image* theTexture, float tx, float ty, bool blend)
{
    MemoryImage* aSrcMemoryImage = (MemoryImage*)theTexture;

    if (!CreateImageTexture(aSrcMemoryImage))
        return;

    SDLTextureData* aData = (SDLTextureData*)aSrcMemoryImage->mD3DData;

    SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_Texture* aTexture = aData->mTexture;
    SDL_SetTextureColorMod(aTexture, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue());
    SDL_SetTextureAlphaMod(aTexture, theColor.GetAlpha());
    SDL_SetTextureBlendMode(aTexture, ChooseBlendMode(theDrawMode));

    for (int aTriangleNum = 0; aTriangleNum < theNumTriangles; aTriangleNum++)
    {
        TriVertex theCurrentVertex[3];
        theCurrentVertex[0] = theVertices[aTriangleNum][0];
        theCurrentVertex[1] = theVertices[aTriangleNum][1];
        theCurrentVertex[2] = theVertices[aTriangleNum][2];

        SDL_Vertex vertices[3];

        SDL_FColor aColor[3];
        aColor[0].r = (theCurrentVertex[0].color >> 16) & 0xFF;
        aColor[0].g = (theCurrentVertex[0].color >> 8) & 0xFF;
        aColor[0].b = theCurrentVertex[0].color & 0xFF;
        aColor[0].a = (theCurrentVertex[0].color >> 24) & 0xFF; // alpha extraction

        aColor[1].r = (theCurrentVertex[1].color >> 16) & 0xFF;
        aColor[1].g = (theCurrentVertex[1].color >> 8) & 0xFF;
        aColor[1].b = theCurrentVertex[1].color & 0xFF;
        aColor[1].a = (theCurrentVertex[1].color >> 24) & 0xFF;

        aColor[2].r = (theCurrentVertex[2].color >> 16) & 0xFF;
        aColor[2].g = (theCurrentVertex[2].color >> 8) & 0xFF;
        aColor[2].b = theCurrentVertex[2].color & 0xFF;
        aColor[2].a = (theCurrentVertex[2].color >> 24) & 0xFF;

        vertices[0].position.x = theCurrentVertex[0].x + tx;
        vertices[0].position.y = theCurrentVertex[0].y + ty;
        vertices[1].position.x = theCurrentVertex[1].x + tx;
        vertices[1].position.y = theCurrentVertex[1].y + ty;
        vertices[2].position.x = theCurrentVertex[2].x + tx;
        vertices[2].position.y = theCurrentVertex[2].y + ty;
        vertices[0].color = aColor[0];
        vertices[1].color = aColor[1];
        vertices[2].color = aColor[2];

        SDL_RenderGeometry(mRenderer, aTexture, vertices, 3, NULL, 3);
    }

    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::DrawTrianglesTexStrip(const TriVertex theVertices[], int theNumTriangles, const Color& theColor, int theDrawMode, Image* theTexture, float tx, float ty, bool blend)
{
    if (theNumTriangles < 3) return;

    MemoryImage* aSrcMemoryImage = (MemoryImage*)theTexture;
    if (!CreateImageTexture(aSrcMemoryImage)) return;
    SDLTextureData* aData = (SDLTextureData*)aSrcMemoryImage->mD3DData;
    SDL_Texture* aTexture = aData->mTexture;

    std::vector<float> positions;
    std::vector<SDL_FColor> colors;
    std::vector<float> uvs;

    for (int i = 0; i < theNumTriangles; ++i)
    {
        const TriVertex& v = theVertices[i];
        positions.push_back(v.x + tx);
        positions.push_back(v.y + ty);
        uvs.push_back(v.u);
        uvs.push_back(v.v);

        SDL_FColor color = {
            (v.color >> 16) & 0xFF,
            (v.color >> 8) & 0xFF,
            v.color & 0xFF,
            (v.color >> 24) & 0xFF
        };
        colors.push_back(color);
    }

    SDL_SetRenderTarget(mRenderer, mScreenTexture);
    SDL_SetTextureBlendMode(aTexture, ChooseBlendMode(theDrawMode));
    SDL_SetTextureColorMod(aTexture, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue());
    SDL_SetTextureAlphaMod(aTexture, theColor.GetAlpha());

    SDL_RenderGeometryRaw(
        mRenderer,
        aTexture,
        positions.data(), sizeof(float) * 2,
        colors.data(), sizeof(SDL_FColor),
        uvs.data(), sizeof(float) * 2,
        positions.size() / 2,
        nullptr, 0, 0
    );

    SDL_SetRenderTarget(mRenderer, nullptr);
}

void SDLInterface::FillPoly(const Point theVertices[], int theNumVertices, const Rect* theClipRect, const Color& theColor, int theDrawMode, int tx, int ty)
{
    if (theNumVertices < 3)
        return;

    for (int i = 0; i < theNumVertices - 1; i++)
        DrawLine(theVertices[i].mX, theVertices[i].mY, theVertices[i + 1].mX, theVertices[i + 1].mY, theColor, theDrawMode);

    DrawLine(theVertices[theNumVertices - 1].mX, theVertices[theNumVertices - 1].mY, theVertices[0].mX, theVertices[0].mY, theColor, theDrawMode);
}

void SDLInterface::BltTexture(SDL_Texture* theTexture, int theX, int theY, const SDL_FRect& theSrcRect, const SDL_FRect& theDestRect, const Color& theColor, int theDrawMode)
{
    SDL_SetRenderTarget(mRenderer, mScreenTexture);

    SDL_SetTextureColorMod(theTexture, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue());
    SDL_SetTextureAlphaMod(theTexture, theColor.GetAlpha());

    SDL_SetTextureBlendMode(theTexture, ChooseBlendMode(theDrawMode));
    SDL_RenderTexture(mRenderer, theTexture, &theSrcRect, &theDestRect);

    SDL_SetTextureBlendMode(theTexture, SDL_BLENDMODE_NONE);

    SDL_SetRenderTarget(mRenderer, nullptr);
}