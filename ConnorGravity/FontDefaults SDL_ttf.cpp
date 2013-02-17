
#if defined (ORGUI_BUILD_SDL) && defined (ORGUI_BUILD_OPENGL)

#include "FontModule.h"
#include "RenderModule.h"
#include <ORE/String.h>
#include <ORE/FileIO.h>

#include <Windows.h>
#include <gl/GL.h>

#include <ctime>

int UpperPower (int value, int power)
{
	return powf (power, ceilf (log (value) / log (power)));
}

namespace gui
{
	namespace module
	{
		int FM_GetStringWidth (ore::t_String string, Font * activeFont)
		{
			if (!activeFont)
				return -1;

			short maxWidth = 0;

			ore::t_StringSet strings;
			ore::String::SplitStringAtSymbol (string, "\n", &strings);
			for (auto currentString = strings.begin (); currentString != strings.end (); currentString++)
			{
				short currentWidth = 0;
				for (int c = 0; c < currentString->size (); c++)
				{
					static int advance;
					TTF_GlyphMetrics (activeFont->font, currentString->at (c), NULL, NULL, NULL, NULL, &advance);
					currentWidth += advance;
				}

				if (currentWidth > maxWidth)
					maxWidth = currentWidth;
			}

			return maxWidth;
		}

		int FM_GetStringHeight (ore::t_String string, Font * activeFont)
		{
			return (ore::String::GetTermCount (string, "\n") + 1) * TTF_FontHeight (activeFont->font);
		}

		int FM_GetFontHeight (Font * activeFont)
		{
			return TTF_FontHeight (activeFont->font);
		}

		void FM_RenderString (float x, float y, ore::t_String text, Font * activeFont)
		{
			ore::t_UInt64 * textures = (ore::t_UInt64 *)activeFont->glyphCache;
			RenderModule * renderModule = RenderModule::GetRenderModule ();

			ore::t_StringSet strings;
			ore::String::SplitStringAtSymbol (text, "\n", &strings);

			//	TODO: Tabs
			int splitNumber = 0;
			for (auto iter = strings.begin (); iter != strings.end (); iter++, splitNumber++)
			{
				float currentX = x, yOffset = splitNumber * TTF_FontLineSkip (activeFont->font);
				for (int i = 0; i < iter->size (); i++)
				{
					int c = iter->at (i);
					int w, h;
					ore::t_UInt64 texLoc = textures [c];
					if (renderModule->TextureExists (texLoc))
					{
						FM_GetGlyphSize (c, &w, &h, activeFont);
						int glyphDrop = FM_GetGlyphDrop (c, activeFont);

						float tx = w/((float)UpperPower (w, 2)), ty = h/((float)UpperPower (h, 2));

						renderModule->SetCurrentTexture (texLoc);
					
						renderModule->RenderQuad (
							ComplexPoint (currentX,   y   + glyphDrop + yOffset, 0.f, 0.f),
							ComplexPoint (currentX,   y+h + glyphDrop + yOffset, 0.f, ty),
							ComplexPoint (currentX+w, y+h + glyphDrop + yOffset, tx,  ty),
							ComplexPoint (currentX+w, y   + glyphDrop + yOffset, tx,  0.f)
						);
					}
				
					currentX += FM_GetGlyphAdvance (c, activeFont);
				}
			}
		}

		Font * FM_LoadFont (ore::t_String fontName, int ptSize)
		{
			ore::t_String path = ore::FileIO::TranslatePath (fontName);

			Font * newFont = new Font;
			newFont->font = TTF_OpenFont (C_STRING (path), ptSize);
			
			if (!newFont->font)
			{
				ore::Diagnostics::Error (true)
					.Append ("FM_LoadFont - Unable to load font:\n")
					.Append (path)
					.Display ();
			}

			//	TODO: Algorithm for creating a single texture containing coordinates for glyphs
			ore::t_UInt64 * textures = new ore::t_UInt64 [255];

			srand (time (NULL));

			//	We only concern ourselves with the textures in ASCII
			for (int i = 0; i < 255; i++)
			{
				int minx, miny, maxx, maxy;
				TTF_GlyphMetrics (newFont->font, i, &minx, &maxx, &miny, &maxy, NULL);

				if (!TTF_GlyphIsProvided (newFont->font, i) || ((maxx-minx) <= 0 && (maxy-miny) <= 0))
				{
					textures [i] = -1;
					continue; 
				}

				void * pixelData = FM_GetGlyphPixels (i, newFont);
				int w = UpperPower (maxx-minx, 2), h = UpperPower (maxy-miny, 2);
				
				ore::t_UInt64 currentTexture = RenderModule::GetRenderModule ()->CreateTexture ((int *)pixelData, w, h, ore::RealToString (rand ()*rand ()));
				textures [i] = currentTexture;

				free (pixelData);
			}

			newFont->glyphCache = textures;
			newFont->cacheSize = 255 * sizeof (ore::t_UInt64);

			return newFont;
		}

		void FM_GetGlyphSize (int c, int * w, int * h, Font * activeFont)
		{
			int lowx, lowy, highx, highy;
			TTF_GlyphMetrics (activeFont->font, c, &lowx, &highx, &lowy, &highy, NULL);

			if (w)
				*w = highx - lowx;
			if (h)
				*h = highy - lowy;
		}

		int FM_GetGlyphAdvance (int c, Font * activeFont)
		{
			int advance;
			TTF_GlyphMetrics (activeFont->font, c, NULL, NULL, NULL, NULL, &advance);
			return advance;
		}

		int FM_GetGlyphDrop (int c, Font * activeFont)
		{
			int maxy;
			TTF_GlyphMetrics (activeFont->font, c, NULL, NULL, NULL, &maxy, NULL);
			return TTF_FontAscent (activeFont->font) - maxy;
		}

		ore::t_Byte * FM_GetGlyphPixels (int c, Font * activeFont)
		{
			SDL_Color color;
			color.r = 255; color.g = 255; color.b = 255;
			SDL_Surface * rawGlyph = TTF_RenderGlyph_Blended (activeFont->font, c, color);
			//	TTF_RenderGlyph_Whatever returns an 8-bit surface with a pallette, we need to convert it to 32-bit RGBA
			//		before we can return the pixels
			SDL_Surface * translation = SDL_CreateRGBSurface (SDL_SWSURFACE, UpperPower (rawGlyph->w, 2), UpperPower (rawGlyph->h, 2), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
			SDL_FillRect (translation, NULL, 0x00000000);
			SDL_BlitSurface (rawGlyph, NULL, translation, NULL);

			//	The blitting tends to forget the alpha channel when moving to the translation surface, so we need to calculate it ourselves
			for (int y = 0; y < translation->h; y++)
			{
				for (int x = 0; x < translation->w; x++)
				{
					//	Calculate alpha by the brightness of the pixel as indicated by the RGB channels
					ore::t_Byte * currentPixel = ((ore::t_Byte *)translation->pixels) + (4 * (y*translation->w + x));
					currentPixel [3] = floorf ((currentPixel [0] + currentPixel [1] + currentPixel [2]) / 3.f + 0.5f);
				}
			}
			
			//	We've gotta' create a new copy of this array, since we need to free the SDL_Surfaces to prevent
			//		memory leaks
			ore::t_Byte * newCopy = new ore::t_Byte [translation->w*translation->h*4];
			memcpy (newCopy, translation->pixels, translation->w*translation->h*4);

			SDL_FreeSurface (translation);
			SDL_FreeSurface (rawGlyph);
			return newCopy;
		}

		void FM_FreeFont (Font * font)
		{
			TTF_CloseFont (font->font);
			free (font->glyphCache);
		}
	}
}

#endif