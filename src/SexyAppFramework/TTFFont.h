#pragma once

#include "Font.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>

namespace Sexy
{
	class SexyAppBase;

	class TTFFont : public Font //TODO: Implement
	{
	public:
		TTFFont(const std::string& theFace, int thePointSize, bool bold = false, bool italics = false, bool underline = false);
		TTFFont(SexyAppBase* theApp, const std::string& theFace, int thePointSize, int theScript = ANSI_CHARSET, bool bold = false, bool italics = false, bool underline = false);
		TTFFont(const TTFFont& theTTFFont);
		virtual ~TTFFont();

		virtual int				GetAscent() {return 0;};
		virtual int				GetAscentPadding() { return 0; };
		virtual int				GetDescent() { return 0; };
		virtual int				GetHeight() { return 0; };
		virtual int				GetLineSpacingOffset() { return 0; };
		virtual int				GetLineSpacing() { return 0; };
		virtual int				StringWidth(const SexyString& theString) { return 0; };
		virtual int				CharWidth(SexyChar theChar) { return 0; };
		virtual int				CharWidthKern(SexyChar theChar, SexyChar thePrevChar) { return 0; };

		virtual void			DrawString(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, const Rect& theClipRect);

		virtual Font* Duplicate();

	public:
		SexyAppBase* mApp;

		std::string mText;
		bool mDrawShadow;
		bool mSimulateBold;
		TTF_Font* mFont;
		Color mColor;
		SDL_Texture* mTexture;
		TTF_Text mTTFText;
		int mWidth;
		int mHeight;

		void Init(SexyAppBase* theApp, const std::string& theFace, int thePointSize, int theScript, bool bold, bool italics, bool underline, bool useDevCaps);
		void SetText(const std::string& textureText);

	private:


	};

}
