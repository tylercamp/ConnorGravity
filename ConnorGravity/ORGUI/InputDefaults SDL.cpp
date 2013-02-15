
#include "InputModule.h"

#include <ORE/Diagnostics.h>

namespace gui
{
	namespace module
	{
		void IM_FillStateChangeObject (const t_ImplEvent * src, InputStateChange * dest)
		{
			switch (src->type)
			{

			case (SDL_KEYDOWN):
				{
					dest->type = KEYBOARD_KEY_DOWN;
					dest->keyboardEvent.code = (t_KeyCode)src->key.keysym.sym;

					//	Apply modifiers
					dest->keyboardEvent.modifiers = 0;
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_LALT?GUIM_LALT:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_RALT?GUIM_RALT:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_CAPS?GUIM_CAPSLOCK:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_LCTRL?GUIM_LCTRL:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_RCTRL?GUIM_RCTRL:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_LSHIFT?GUIM_LSHIFT:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_RSHIFT?GUIM_RSHIFT:0);

					//	Translate character
					//	Check to see if UNICODE translation is enabled. If it isn't, display a warning.
					if (SDL_EnableUNICODE (-1) != 1)
					{
						DEBUG_RUN_ONCE (
						ore::Diagnostics::Warning (true)
							.Append ("ORGUI InputDefaults SDL.cpp | IM_FillStateChangeObject - UNICODE translation of events is not enabled within the SDL. Proper translation of keyboard events to their appropriate character equivalents will not be available.")
							.Display ();
						)
					}

					break;
				}

			case (SDL_KEYUP):
				{
					dest->type = KEYBOARD_KEY_UP;
					dest->keyboardEvent.charTranslation = (t_KeyCode)src->key.keysym.sym;

					dest->keyboardEvent.modifiers = 0;
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_LALT?GUIM_LALT:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_RALT?GUIM_RALT:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_CAPS?GUIM_CAPSLOCK:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_LCTRL?GUIM_LCTRL:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_RCTRL?GUIM_RCTRL:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_LSHIFT?GUIM_LSHIFT:0);
					dest->keyboardEvent.modifiers |= (src->key.keysym.mod & KMOD_RSHIFT?GUIM_RSHIFT:0);

					dest->keyboardEvent.charTranslation = 0;
					break;
				}

			case (SDL_MOUSEBUTTONDOWN):
				{
					dest->type = MOUSE_BUTTON_DOWN;

					dest->mouseEvent.button = 0;
					dest->mouseEvent.button |= (src->button.button & SDL_BUTTON_LMASK ? MOUSE_LEFT_BUTTON : 0);
					dest->mouseEvent.button |= (src->button.button & SDL_BUTTON_MMASK ? MOUSE_MIDDLE_BUTTON : 0);
					dest->mouseEvent.button |= (src->button.button & SDL_BUTTON_RMASK ? MOUSE_RIGHT_BUTTON : 0);

					dest->mouseEvent.mouseX = src->button.x;
					dest->mouseEvent.mouseY = src->button.y;
					break;
				}

			case (SDL_MOUSEBUTTONUP):
				{
					dest->type = MOUSE_BUTTON_UP;

					dest->mouseEvent.button = 0;
					dest->mouseEvent.button |= (src->button.button & SDL_BUTTON_LMASK ? MOUSE_LEFT_BUTTON : 0);
					dest->mouseEvent.button |= (src->button.button & SDL_BUTTON_MMASK ? MOUSE_MIDDLE_BUTTON : 0);
					dest->mouseEvent.button |= (src->button.button & SDL_BUTTON_RMASK ? MOUSE_RIGHT_BUTTON : 0);

					dest->mouseEvent.mouseX = src->button.x;
					dest->mouseEvent.mouseY = src->button.y;
				}

			case (SDL_QUIT):
				{
					dest->type = WINDOW_CLOSE_BUTTON;
					break;
				}

			case (SDL_MOUSEMOTION):
				{
					dest->type = MOUSE_MOVE;
					dest->mouseEvent.mouseX = src->motion.x;
					dest->mouseEvent.mouseY = src->motion.y;

					dest->mouseEvent.button = 0;
					dest->mouseEvent.button |= (src->motion.state & SDL_BUTTON_LMASK ? MOUSE_LEFT_BUTTON : 0);
					dest->mouseEvent.button |= (src->motion.state & SDL_BUTTON_MMASK ? MOUSE_MIDDLE_BUTTON : 0);
					dest->mouseEvent.button |= (src->motion.state & SDL_BUTTON_RMASK ? MOUSE_RIGHT_BUTTON : 0);

					break;
				}

			case (SDL_ACTIVEEVENT):
				{
					if (src->active.gain)
						dest->type = WINDOW_GAINED_FOCUS;
					else
						dest->type = WINDOW_LOST_FOCUS;
					break;
				}

			case (SDL_VIDEORESIZE):
				{
					dest->type = WINDOW_RESIZE;
					dest->windowEvent.windowWidth = src->resize.w;
					dest->windowEvent.windowHeight = src->resize.h;
					break;
				}
			}
		}
	}
}