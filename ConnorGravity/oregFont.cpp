
#include "oregFont.h"
#include "oregGeneric.h"

namespace ore
{
	Font::Font (ore::t_String font, int ptSize, Font::FontType type)
	{
		if (!ore::FileIO::FileExists (font))
		{
			ore::Diagnostics::Error (true)
				.Append ("ore::Font | Unable to load font ")
				.Append (ore::FileIO::TranslatePath (font))
				.Display ();
			return;
		}

		_GenerateGlyphs (ore::FileIO::TranslatePath (font), ptSize, type);
	}

	Font::~Font ()
	{
		for (int i = 0; i < m_GlyphData.size (); i++)
		{
			if (m_GlyphData [i])
			{
				delete m_GlyphData [i];
			}
		}

		glDeleteTextures (1, &m_GlyphMap);

		if (m_Font)
			TTF_CloseFont (m_Font);
	}

	void Font::RenderString (float x, float y, ore::t_String text, Font::AlignMode alignment, float angle, bool autoCenter)
	{
		void (ore::Font::*renderFunc)(float,float,ore::t_String,float,bool);

		switch (alignment)
		{
		case (LeftAligned): renderFunc = &ore::Font::_RenderStringLeftAligned; break;
		case (Centered): renderFunc = &ore::Font::_RenderStringCentered; break;
		case (RightAligned): renderFunc = &ore::Font::_RenderStringRightAligned; break;
		default: throw; break;
		}

		(this->*renderFunc) (x, y, text, angle, autoCenter);
	}

	ore::t_String Font::FitTextToWidth (ore::t_String text, int width, ore::t_String newlinePrefix)
	{
		//	Will need to figure out what to do with newlines and tabs later on
		ore::t_StringSet words;
		ore::String::SplitStringAtSymbol (text, " ", &words);

		ore::t_String output = "", currentLine = "";

		int currentWidth = 0;
		for (auto iter = words.begin (); iter != words.end (); iter++)
		{
			if (iter->find ("\n") != iter->npos)
			{
				// In this case there's a newline in the word itself; we have to process this differently.
				//	(Note that this algorithm only accounts for when there is ONE newline, and won't
				//	work properly when you have something like word\nword\nword. This is an abnormal
				//	occurrence though, and it isn't worth accomodating for that sort of thing)

				//	If the word is actually a newline, move to the next line and don't add a prefix.
				if (*iter == "\n")
				{
					output += currentLine;
					currentLine = "\n";
					continue;
				}

				// If the newline is at the beginning of the word, then we end the current line and start
				//	on the next one without a prefix, adding the word to the end.
				if (iter->front () == '\n')
				{
					output += currentLine;
					currentLine = "\n" + iter->substr (1);
					continue;
				}

				// If the newline is at the end, then we add the word and move onto the next line
				//	without adding a prefix.
				if (iter->back () == '\n')
				{
					output += currentLine + " " + iter->substr (0, iter->size () - 1);
					currentLine = "\n";
					continue;
				}

				// At this point the newline is in the middle of the word; split the string into two, add
				//	the first part to the current output, move to the next line, and add the second part
				//	without adding a prefix.
				output += currentLine + " " + iter->substr (0, iter->find ("\n"));
				currentLine = iter->substr (iter->find ("\n"));

				//	NOTE: This algorithm does not check for words existing within the bounds defined by 'width'.
			}
			else
			{
				//	It's just a normal word, process it normally
				int currentWordWidth = GetStringWidth (*iter + " ");
				if (currentWidth + currentWordWidth > width)
				{
					output += currentLine;
					currentWidth = 0;
					currentLine = "\n" + newlinePrefix + *iter;
				}
				else
				{
					if (iter != words.begin ())
						currentLine += " ";
					currentLine += *iter;
				}
			}

			currentWidth = GetStringWidth (currentLine);
		}

		output += currentLine;

		return output;
	}

	int Font::GetStringWidth (ore::t_String text)
	{
		ore::t_StringSet lines;
		ore::String::SplitStringAtSymbol (text, "\n", &lines);

		int maxWidth = -1;

		for (auto iter = lines.begin (); iter != lines.end (); iter++)
		{
			int currentLineWidth = 0;
			ore::t_String currentLine = *iter;
			for (int i = 0; i < currentLine.size (); i++)
			{
				int advance = 0;
				TTF_GlyphMetrics (m_Font, currentLine [i], NULL, NULL, NULL, NULL, &advance);
				currentLineWidth += advance;
			}

			if (currentLineWidth > maxWidth)
				maxWidth = currentLineWidth;
		}

		return maxWidth;
	}

	int Font::GetStringHeight (ore::t_String text)
	{
		return (ore::String::GetTermCount (text, "\n") + 1) * TTF_FontLineSkip (m_Font);
	}

	int Font::GetPerLineHeight ()
	{
		return TTF_FontLineSkip (m_Font);
	}

	void Font::_RenderStringLeftAligned (float x, float y, ore::t_String text, float angle, bool autoCenter)
	{
		//	Used to move from line to line and character to character
		float yOffset = 0.f, xOffset = 0.f;

		Generic::BindTexture (m_GlyphMap);

		glMatrixMode (GL_MODELVIEW);
		glPushMatrix ();
		glTranslatef (floorf (x + 0.5f), floorf (y + 0.5f), 0.f);
		glRotatef (angle, 0.f, 0.f, 1.f);
		if (autoCenter)
			glTranslatef (floorf (-GetStringWidth (text) / 2.f + 0.5f), floorf (-GetStringHeight (text) / 2.f + 0.5f), 0.f);

		glBegin (GL_QUADS);

		for (int i = 0; i < text.size (); i++)
		{
			if (text [i] == '\n')
			{
				yOffset += TTF_FontLineSkip (m_Font);
				xOffset = 0.f;
				continue;
			}

			if (TTF_GlyphIsProvided (m_Font, text [i]) && m_GlyphData [text [i]] != NULL)
			{
				Glyph * currentGlyph = m_GlyphData [text [i]];

				//	Offsets for the current glyph
				float baseOffsetX = xOffset + currentGlyph->horizontalKerningOffset;
				float baseOffsetY = yOffset + currentGlyph->verticalKerningOffset;

				glTexCoord2f (currentGlyph->tx1, currentGlyph->ty1);
				glVertex2f (baseOffsetX, baseOffsetY);

				glTexCoord2f (currentGlyph->tx1, currentGlyph->ty2);
				glVertex2f (baseOffsetX, baseOffsetY + currentGlyph->height);

				glTexCoord2f (currentGlyph->tx2, currentGlyph->ty2);
				glVertex2f (baseOffsetX + currentGlyph->width, baseOffsetY + currentGlyph->height);

				glTexCoord2f (currentGlyph->tx2, currentGlyph->ty1);
				glVertex2f (baseOffsetX + currentGlyph->width, baseOffsetY);
			}

			int glyphAdvance = 0;
			TTF_GlyphMetrics (m_Font, text [i], NULL, NULL, NULL, NULL, &glyphAdvance);
			xOffset += glyphAdvance;
		}

		glEnd ();

		glPopMatrix ();
	}

	void Font::_RenderStringCentered (float x, float y, ore::t_String text, float angle, bool autoCenter)
	{
		ore::t_StringSet strings;
		ore::String::SplitStringAtSymbol (text, "\n", &strings);

		float yOffset = 0.f;

		for (auto iter = strings.begin (); iter != strings.end (); iter++)
		{
			ore::t_String currentString = *iter;
			_RenderStringLeftAligned (x - GetStringWidth (currentString) / 2.f, y + yOffset, currentString, angle, autoCenter);
			yOffset += TTF_FontLineSkip (m_Font);
		}
	}

	void Font::_RenderStringRightAligned (float x, float y, ore::t_String text, float angle, bool autoCenter)
	{
		ore::t_StringSet strings;
		ore::String::SplitStringAtSymbol (text, "\n", &strings);

		float yOffset = 0.f;
		for (auto iter = strings.begin (); iter != strings.end (); iter++)
		{
			ore::t_String currentString = *iter;
			_RenderStringLeftAligned (x - GetStringWidth (currentString), y + yOffset, currentString, angle, autoCenter);
			yOffset += TTF_FontLineSkip (m_Font);
		}
	}

	void Font::_GenerateGlyphs (ore::t_String font, int ptSize, FontType type)
	{
		m_Font = TTF_OpenFont (C_STRING (font), ptSize);

		if (!m_Font)
		{
			ore::Diagnostics::Error (true)
				.Append ("ore::Font | Call to TTF_OpenFont failed for font:\n").Append (font)
				.Append ("\nTTF_GetError reports:\n").Append (TTF_GetError ())
				.Display ();
		}

		_ApplyFontType (m_Font, type);

		SDL_Surface * glyphMap = SDL_CreateRGBSurface (SDL_SWSURFACE, 1024, 1024, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		SDL_FillRect (glyphMap, NULL, 0x00000000);

		//						  ha
		for (int c = 0; c <= 0xFF; c++)
		{
			int minx, maxx, miny, maxy;
			TTF_GlyphMetrics (m_Font, c, &minx, &maxx, &miny, &maxy, NULL);

			if (!TTF_GlyphIsProvided (m_Font, c) || maxx-minx <= 0 || maxy-miny <= 0)
			{
				m_GlyphData.push_back (NULL);
				continue;
			}

			int glyphGridY = c / 16;
			int glyphGridX = c - glyphGridY * 16;

			int glyphMapX = glyphGridX * 64;
			int glyphMapY = glyphGridY * 64;

			SDL_Color white;
			white.r = 255; white.g = 255; white.b = 255;
			SDL_Surface * glyph = TTF_RenderGlyph_Blended (m_Font, c, white);
			SDL_SetAlpha (glyph, 0, SDL_ALPHA_OPAQUE);

			SDL_Rect destRect;
			destRect.x = glyphMapX; destRect.y = glyphMapY;
			destRect.w = 64; destRect.h = 64;
			SDL_BlitSurface (glyph, NULL, glyphMap, &destRect);
			
			Glyph * glyphData = new Glyph;
			glyphData->character = c;
			glyphData->height = glyph->h;
			glyphData->horizontalKerningOffset = minx;
			glyphData->tx1 = (float)glyphMapX / 1024.f;
			glyphData->ty1 = (float)glyphMapY / 1024.f;
			glyphData->tx2 = (float)(glyphMapX + glyph->w) / 1024.f;
			glyphData->ty2 = (float)(glyphMapY + glyph->h) / 1024.f;
			glyphData->verticalKerningOffset = TTF_FontAscent (m_Font) - maxy;
			glyphData->width = glyph->w;

			m_GlyphData.push_back (glyphData);
			SDL_FreeSurface (glyph);
		}

		glGenTextures (1, &m_GlyphMap);
		Generic::BindTexture (m_GlyphMap);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, glyphMap->w, glyphMap->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, glyphMap->pixels);

		SDL_FreeSurface (glyphMap);
	}

	void Font::_ApplyFontType (TTF_Font * font, FontType type)
	{
		switch (type)
		{
		case (Normal):			TTF_SetFontStyle (font, TTF_STYLE_NORMAL);			break;
		case (Underline):		TTF_SetFontStyle (font, TTF_STYLE_UNDERLINE);		break;
		case (StrikeThrough):	TTF_SetFontStyle (font, TTF_STYLE_STRIKETHROUGH);	break;
		case (Italic):			TTF_SetFontStyle (font, TTF_STYLE_ITALIC);			break;
		case (Bold):			TTF_SetFontStyle (font, TTF_STYLE_BOLD);			break;
		}
	}
}