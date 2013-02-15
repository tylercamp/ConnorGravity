
#ifndef _ORGUI_FONT_MODULE_
#define _ORGUI_FONT_MODULE_

#include "Module.h"
#include "FontImpl.h"
#include <ORE/Types.h>

#include <map>

/* */

namespace gui
{
	namespace module
	{
		const t_ModuleFunctionID
			FM_GET_STRING_WIDTH = 0,
			FM_GET_STRING_HEIGHT = 1,
			FM_GET_FONT_HEIGHT = 2,
			FM_RENDER_STRING = 3,
			FM_LOAD_FONT = 5,
			FM_GET_GLYPH_SIZE = 6,
			FM_GET_GLYPH_ADVANCE = 7,
			FM_GET_GLYPH_DROP = 8,
			FM_GET_GLYPH_PIXELS = 9,
			FM_FREE_FONT = 10;

		int FM_GetStringWidth (ore::t_String string, Font * activeFont);
		int FM_GetStringHeight (ore::t_String string, Font * activeFont);
		int FM_GetFontHeight (Font * activeFont);

		void FM_RenderString (float x, float y, ore::t_String text, Font * activeFont);

		Font * FM_LoadFont (ore::t_String fontName, int ptSize);

		void FM_GetGlyphSize (int c, int * w, int * h, Font * activeFont);
		int FM_GetGlyphAdvance (int c, Font * activeFont);
		int FM_GetGlyphDrop (int c, Font * activeFont);
		ore::t_Byte * FM_GetGlyphPixels (int c, Font * activeFont, int * glyphWidth, int * glyphHeight);
		void FM_FreeFont (Font * font);
	}

	//	Manages font loading, glyph data extraction, metrics, and texture creation (via the render module) for glyphs
	class FontModule
		: public Module
	{
	public:
		static FontModule * GetFontModule ();

		FontModule ();

		/* Overloadables */
		void LoadFont (ore::t_String fontName, int ptSize, ore::t_String alias = "");

		int GetStringWidth (ore::t_String string);
		int GetStringHeight (ore::t_String string);
		int GetFontHeight ();

		void RenderString (float x, float y, ore::t_String text);

		void GetGlyphSize (int c, int * w, int * h);
		int GetGlyphAdvance (int c);
		int GetGlyphDrop (int c);
		ore::t_Byte * GetGlyphPixels (int c, int * glyphWidth, int * glyphHeight);


		/* Constants */
		void ClearFontCache ();
		void SetActiveFont (ore::t_String fontName);

	private:
		std::map <ore::t_UInt64, module::Font *> m_CachedFonts;
		module::Font * m_ActiveFont;
	};
}

#endif