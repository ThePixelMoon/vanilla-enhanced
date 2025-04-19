//======= Copyright PopCap, All rights reserved =======//
//
//======================2009===========================//

#ifndef __SDLIMAGE_H__
#define __SDLIMAGE_H__
#ifdef _WIN32
#pragma once
#endif

#include "MemoryImage.h"

namespace Sexy
{
	class SDLInterface;
	class SysFont;

	class SDLImage : public MemoryImage
	{
	protected:
		friend class			SysFont;
	public:
		SDLInterface* mInterface;
	public:
		virtual void			FillScanLinesWithCoverage(Span* theSpans, int theSpanCount, const Color& theColor, int theDrawMode, const BYTE* theCoverage, int theCoverX, int theCoverY, int theCoverWidth, int theCoverHeight);

		static bool				Check3D(SDLImage* theImage);
		static bool				Check3D(Image* theImage);

	public:
		SDLImage();
		SDLImage(SDLInterface* theInterface);
		virtual ~SDLImage();

		virtual void Create(int theWidth, int theHeight);

		virtual bool			PolyFill3D(const Point theVertices[], int theNumVertices, const Rect* theClipRect, const Color& theColor, int theDrawMode, int tx, int ty);
		virtual void			FillRect(const Rect& theRect, const Color& theColor, int theDrawMode);
		virtual void			DrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode);
		virtual void			DrawLineAA(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode);
		virtual void			Blt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode);
		virtual void			BltF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode);
		virtual void			BltRotated(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, double theRot, float theRotCenterX, float theRotCenterY);
		virtual void			StretchBlt(Image* theImage, const Rect& theDestRect, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch);
		virtual void			BltMatrix(Image* theImage, float x, float y, const SexyMatrix3& theMatrix, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect& theSrcRect, bool blend);
		virtual void			BltTrianglesTex(Image* theTexture, const TriVertex theVertices[][3], int theNumTriangles, const Rect& theClipRect, const Color& theColor, int theDrawMode, float tx, float ty, bool blend);

		virtual void			BltMirror(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode);
		virtual void			StretchBltMirror(Image* theImage, const Rect& theDestRectOrig, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch);

		virtual void			PurgeBits();
	};
}

#endif