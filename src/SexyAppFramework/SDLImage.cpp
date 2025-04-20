//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#include "SDLImage.h"
#include "CritSect.h"
#include "Debug.h"
#include "SDLInterface.h"
#include "SexyAppBase.h"
#include "Image.h"

using namespace Sexy;

SDLImage::SDLImage() : MemoryImage(gSexyAppBase)
{
    mInterface = gSexyAppBase->mSDLInterface;
    mInterface->AddSDLImage(this);
}

SDLImage::SDLImage(SDLInterface* theInterface) : MemoryImage(theInterface->mApp)
{
    mInterface = theInterface;
    mInterface->AddSDLImage(this);
}

SDLImage::~SDLImage()
{
    mInterface->RemoveSDLImage(this);
}

void SDLImage::Create(int theWidth, int theHeight)
{
    delete[] mBits;
    mBits = nullptr;

    mWidth = theWidth;
    mHeight = theHeight;

    mHasTrans = true;
    mHasAlpha = true;

    BitsChanged();
}

bool SDLImage::PolyFill3D(const Point theVertices[], int theNumVertices, const Rect* theClipRect, const Color& theColor, int theDrawMode, int tx, int ty)
{
    mInterface->FillPoly(theVertices, theNumVertices, theClipRect, theColor, theDrawMode, tx, ty);
    return true;
}

void SDLImage::FillRect(const Rect& theRect, const Color& theColor, int theDrawMode)
{
    mInterface->FillRect(theRect, theColor, theDrawMode);
}

void SDLImage::DrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode)
{
    mInterface->DrawLine(theStartX, theStartY, theEndX, theEndY, theColor, theDrawMode);
}

void SDLImage::DrawLineAA(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode)
{
    mInterface->DrawLine(theStartX, theStartY, theEndX, theEndY, theColor, theDrawMode);
}

void SDLImage::Blt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode)
{
    theImage->mDrawn = true;

    DBG_ASSERTE((theColor.mRed >= 0) && (theColor.mRed <= 255));
    DBG_ASSERTE((theColor.mGreen >= 0) && (theColor.mGreen <= 255));
    DBG_ASSERTE((theColor.mBlue >= 0) && (theColor.mBlue <= 255));
    DBG_ASSERTE((theColor.mAlpha >= 0) && (theColor.mAlpha <= 255));

    CommitBits();

    mInterface->Blt(theImage, theX, theY, theSrcRect, theColor, theDrawMode);
}

void SDLImage::BltF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode)
{
    theImage->mDrawn = true;

    FRect aClipRect(theClipRect.mX, theClipRect.mY, theClipRect.mWidth, theClipRect.mHeight);
    FRect aDestRect(theX, theY, theSrcRect.mWidth, theSrcRect.mHeight);

    FRect anIntersect = aDestRect.Intersection(aClipRect);
    if (anIntersect.mWidth != aDestRect.mWidth || anIntersect.mHeight != aDestRect.mHeight)
    {
        if (anIntersect.mWidth != 0 && anIntersect.mHeight != 0)
            mInterface->BltClipF(theImage, theX, theY, theSrcRect, &theClipRect, theColor, theDrawMode);
    }
    else
        mInterface->Blt(theImage, theX, theY, theSrcRect, theColor, theDrawMode, true);
}

void SDLImage::BltRotated(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, double theRot, float theRotCenterX, float theRotCenterY)
{
    theImage->mDrawn = true;

    CommitBits();

    mInterface->BltRotated(theImage, theX, theY, &theClipRect, theColor, theDrawMode, theRot, theRotCenterX, theRotCenterY, theSrcRect);
}

void SDLImage::StretchBlt(Image* theImage, const Rect& theDestRect, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch)
{
    theImage->mDrawn = true;

    CommitBits();

    mInterface->StretchBlt(theImage, theDestRect, theSrcRect, &theClipRect, theColor, theDrawMode, fastStretch);
}

void SDLImage::BltMatrix(Image* theImage, float x, float y, const SexyMatrix3& theMatrix, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect& theSrcRect, bool blend)
{
    theImage->mDrawn = true;

    mInterface->BltTransformed(theImage, &theClipRect, theColor, theDrawMode, theSrcRect, theMatrix, blend, x, y, true);
}

void SDLImage::BltTrianglesTex(Image* theTexture, const TriVertex theVertices[][3], int theNumTriangles, const Rect& theClipRect, const Color& theColor, int theDrawMode, float tx, float ty, bool blend)
{
    theTexture->mDrawn = true;

    mInterface->DrawTrianglesTex(theVertices, theNumTriangles, theColor, theDrawMode, theTexture, tx, ty, blend);
}

void SDLImage::BltMirror(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode)
{
    theImage->mDrawn = true;

    DBG_ASSERTE((theColor.mRed >= 0) && (theColor.mRed <= 255));
    DBG_ASSERTE((theColor.mGreen >= 0) && (theColor.mGreen <= 255));
    DBG_ASSERTE((theColor.mBlue >= 0) && (theColor.mBlue <= 255));
    DBG_ASSERTE((theColor.mAlpha >= 0) && (theColor.mAlpha <= 255));

    CommitBits();

    mInterface->BltMirror(theImage, theX, theY, theSrcRect, theColor, theDrawMode);
}

void SDLImage::StretchBltMirror(Image* theImage, const Rect& theDestRectOrig, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch)
{
    theImage->mDrawn = true;

    CommitBits();

    mInterface->StretchBlt(theImage, theDestRectOrig, theSrcRect, &theClipRect, theColor, theDrawMode, fastStretch, true);
}

void SDLImage::FillScanLinesWithCoverage(Span* theSpans, int theSpanCount, const Color& theColor, int theDrawMode, const BYTE* theCoverage, int theCoverX, int theCoverY, int theCoverWidth, int theCoverHeight)
{
    if (theSpanCount == 0) return;

    int l = theSpans[0].mX, t = theSpans[0].mY;
    int r = l + theSpans[0].mWidth, b = t;
    for (int i = 1; i < theSpanCount; ++i)
    {
        l = min(theSpans[i].mX, l);
        r = max(theSpans[i].mX + theSpans[i].mWidth - 1, r);
        t = min(theSpans[i].mY, t);
        b = max(theSpans[i].mY + theSpans[i].mWidth - 1, b);
    }
    for (int i = 0; i < theSpanCount; ++i)
    {
        theSpans[i].mX -= l;
        theSpans[i].mY -= t;
    }

    MemoryImage aTempImage;
    aTempImage.Create(r - l + 1, b - t + 1);
    aTempImage.FillScanLinesWithCoverage(theSpans, theSpanCount, theColor, theDrawMode, theCoverage, theCoverX - l, theCoverY - t, theCoverWidth, theCoverHeight);
    Blt(&aTempImage, l, t, Rect(0, 0, r - l + 1, b - t + 1), Color::White, theDrawMode);
}

bool SDLImage::Check3D(SDLImage* theImage)
{
    return true;
}

bool SDLImage::Check3D(Image* theImage)
{
    SDLImage* anImage = dynamic_cast<SDLImage*>(theImage);
    return anImage != nullptr;
}

void SDLImage::PurgeBits()
{
    mPurgeBits = true;

    CommitBits();
    GetBits();

    MemoryImage::PurgeBits();
}
