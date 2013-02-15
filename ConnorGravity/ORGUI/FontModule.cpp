
#include "FontModule.h"
#include "Hash.h"
#include <ORE/Diagnostics.h>

namespace gui
{
	using namespace module;

	FontModule::FontModule ()
	{
		OverloadFunction (FM_GET_STRING_WIDTH, MODULE_FUNCTION (FM_GetStringWidth));
		OverloadFunction (FM_GET_STRING_HEIGHT, MODULE_FUNCTION (FM_GetStringHeight));
		OverloadFunction (FM_GET_FONT_HEIGHT, MODULE_FUNCTION (FM_GetFontHeight));
		OverloadFunction (FM_RENDER_STRING, MODULE_FUNCTION (FM_RenderString));
		OverloadFunction (FM_LOAD_FONT, MODULE_FUNCTION (FM_LoadFont));
		OverloadFunction (FM_GET_GLYPH_SIZE, MODULE_FUNCTION (FM_GetGlyphSize));
		OverloadFunction (FM_GET_GLYPH_ADVANCE, MODULE_FUNCTION (FM_GetGlyphAdvance));
		OverloadFunction (FM_GET_GLYPH_DROP, MODULE_FUNCTION (FM_GetGlyphDrop));
		OverloadFunction (FM_GET_GLYPH_PIXELS, MODULE_FUNCTION (FM_GetGlyphPixels));
		OverloadFunction (FM_FREE_FONT, MODULE_FUNCTION (FM_FreeFont));
	}

	FontModule * FontModule::GetFontModule ()
	{
		static FontModule * fontModule = NULL;
		if (!fontModule)
		{
			fontModule = new FontModule;
		}
		return fontModule;
	}

	void FontModule::LoadFont (ore::t_String fontName, int ptSize, ore::t_String alias)
	{
		if (alias.size () == 0)
		{
			alias = fontName;
		}

		ore::t_UInt64 hash = Hash (alias);
		if (m_CachedFonts.find (hash) != m_CachedFonts.end ())
		{
			ore::Diagnostics::Warning (true)
				.Append ("FontModule.cpp | gui::FontModule::LoadFont - Overwriting existing alias \"")
				.Append (alias)
				.Append ("\" with font \"")
				.Append (fontName)
				.Append ("\"")
				.Display ();
		}

		Font * newFont =
			((Font * (*)(ore::t_String, int))GetFunction (FM_LOAD_FONT)) (fontName, ptSize);
		m_CachedFonts [hash] = newFont;
		m_ActiveFont = newFont;
	}

	int FontModule::GetStringWidth (ore::t_String string)
	{
		return ((int (*)(ore::t_String, Font *))GetFunction (FM_GET_STRING_WIDTH)) (string, m_ActiveFont);
	}

	int FontModule::GetStringHeight (ore::t_String string)
	{
		return ((int (*)(ore::t_String, Font *))GetFunction (FM_GET_STRING_HEIGHT)) (string, m_ActiveFont);
	}

	int FontModule::GetFontHeight ()
	{
		return ((int (*)(Font *))GetFunction (FM_GET_FONT_HEIGHT)) (m_ActiveFont);
	}

	void FontModule::RenderString (float x, float y, ore::t_String text)
	{
		((void (*)(float, float, ore::t_String, Font *))GetFunction (FM_RENDER_STRING)) (x, y, text, m_ActiveFont);
	}

	void FontModule::GetGlyphSize (int c, int * w, int * h)
	{
		((void (*)(int, int *, int *, Font *))GetFunction (FM_GET_GLYPH_SIZE)) (c, w, h, m_ActiveFont);
	}

	int FontModule::GetGlyphAdvance (int c)
	{
		return ((int (*)(int, Font *))GetFunction (FM_GET_GLYPH_ADVANCE)) (c, m_ActiveFont);
	}

	int FontModule::GetGlyphDrop (int c)
	{
		return ((int (*)(int, Font *)) GetFunction (FM_GET_GLYPH_DROP)) (c, m_ActiveFont);
	}
	
	ore::t_Byte * FontModule::GetGlyphPixels (int c, int * glyphWidth, int * glyphHeight)
	{
		return ((ore::t_Byte * (*)(int, int *, int *, Font *))GetFunction (FM_GET_GLYPH_PIXELS)) (c, glyphWidth, glyphHeight, m_ActiveFont);
	}





	void FontModule::ClearFontCache ()
	{
		for (auto iter = m_CachedFonts.begin (); iter != m_CachedFonts.end (); iter++)
		{
			((void (*)(Font *))GetFunction (FM_FREE_FONT)) (iter->second);
		}
	}

	void FontModule::SetActiveFont (ore::t_String fontName)
	{
		auto location = m_CachedFonts.find (Hash (fontName));

		if (location == m_CachedFonts.end ())
		{
			auto stringer = ore::Diagnostics::Error (true);

			stringer
				.Append ("Unable to set active font to \"" + fontName + "\" for hash ")
				.Append (Hash (fontName))
				.Append ("\nFont dump for ").Append (m_CachedFonts.size ()).Append (" fonts:\n");

			for (auto iter = m_CachedFonts.begin (); iter != m_CachedFonts.end (); iter++)
			{
				stringer.Append (iter->first).Append ("\n");
			}

			stringer.Display ();
		}

		m_ActiveFont = location->second;
	}
}