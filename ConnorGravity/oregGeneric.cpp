/* 
 * Allen Webster
 * Copy Right OverReact
 *
 */

#include "oregGeneric.h"

#include <SDL/SDL_image.h>

namespace ore
{
	void Generic::BindTexture (GLuint texture)
	{
		static GLuint activeTexture = -1;
		if (activeTexture != texture)
		{
			glBindTexture (GL_TEXTURE_2D, texture);
			activeTexture = texture;
		}
	}
}
