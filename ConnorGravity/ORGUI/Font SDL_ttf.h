
#ifndef _ORGUI_FONT_SDL_TTF_H_
#define _ORGUI_FONT_SDL_TTF_H_

#include <SDL/SDL_ttf.h>

namespace gui
{
	namespace module
	{
		struct Font
		{
			TTF_Font * font;
			void * glyphCache;
			int cacheSize;
		};
	}
}

#endif