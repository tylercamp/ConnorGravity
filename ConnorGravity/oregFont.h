
#ifndef _ORE_FONT_H_
#define _ORE_FONT_H_

#include <ORE\Types.h>
#include <SDL\SDL_ttf.h>
#include <ORE\FileIO.h>
#include <ORE\String.h>
#include <vector>

#include "OpenGL.h"

namespace ore
{
	struct Glyph
	{
		//	Width and height of the glyph
		int width, height;

		//	Texture locations indicating the location of the glyph in the texture map
		float tx1, ty1, tx2, ty2;

		//	Kerning data; y offset for rendering the glyph
		float verticalKerningOffset;

		//	Kerning data; x offset for rendering the glyph;
		float horizontalKerningOffset;

		//	Character that the glyph represents (only ASCII for now)
		unsigned char character;
	};

	class Font
	{
	public:
		enum FontType
		{
			Normal,
			Bold,
			Italic,
			StrikeThrough,
			Underline
		};

		enum AlignMode
		{
			Centered,
			LeftAligned,
			RightAligned
		};
		
		struct Color
		{
			ore::t_Byte r, g, b;
		};

		Font (ore::t_String font, int ptSize, FontType type = Normal);

		~Font ();

		void GenerateGlyphData ();

		void RenderString (float x, float y, ore::t_String text, AlignMode alignment = LeftAligned, float angleDegrees = 0.f, bool autoCenter = false);

		ore::t_String FitTextToWidth (ore::t_String text, int width, ore::t_String newLinePrefix = "");

		int GetStringWidth (ore::t_String text);
		int GetStringHeight (ore::t_String text);

		int GetPerLineHeight ();

	private:
		void _RenderStringLeftAligned (float x, float y, ore::t_String text, float angle, bool autoCenter);
		void _RenderStringCentered (float x, float y, ore::t_String text, float angle, bool autoCenter);
		void _RenderStringRightAligned (float x, float y, ore::t_String text, float angle, bool autoCenter);

		void _GenerateGlyphs (ore::t_String font, int ptSize, FontType type);

		void _ApplyFontType (TTF_Font * font, FontType type);

		std::vector <Glyph *> m_GlyphData;
		GLuint m_GlyphMap;
		TTF_Font * m_Font;
	};

}

#endif