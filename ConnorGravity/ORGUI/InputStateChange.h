
#ifndef _ORGUI_INPUT_STATE_CHANGE_H_
#define _ORGUI_INPUT_STATE_CHANGE_H_

#include <ORE/Types.h>
#include "InputKeyCodes.h"

/* (Scroll down to union InputStateChange declaration) */

namespace gui
{
	/* Keyboard event definition */
	enum
	{
		KEYBOARD_KEY_DOWN,
		KEYBOARD_KEY_UP,
		KEYBOARD_LAST
	};

	struct KeyboardEvent
	{
		ore::t_UInt8 type;
		t_KeyCode code;
		char charTranslation, modifiers;
	};


	/* Mouse event definition */
	enum
	{
		MOUSE_BUTTON_DOWN = KEYBOARD_LAST,
		MOUSE_BUTTON_UP,
		MOUSE_MOVE,
		MOUSE_SCROLL_WHEEL,
		MOUSE_LAST,

		MOUSE_LEFT_BUTTON	= 0x01,
		MOUSE_MIDDLE_BUTTON	= 0x02,
		MOUSE_RIGHT_BUTTON	= 0x04
	};

	struct MouseEvent
	{
		ore::t_UInt8 type;
		short mouseX, mouseY;
		char button, scrollAmount;
	};


	/* Window event definition */
	enum
	{
		WINDOW_RESIZE = MOUSE_LAST,
		WINDOW_MOVE,
		WINDOW_CLOSE_BUTTON,
		WINDOW_MINIMIZE_BUTTON,
		WINDOW_MAXIMIZE_BUTTON,
		WINDOW_GAINED_FOCUS,
		WINDOW_LOST_FOCUS,
		WINDOW_LAST
	};

	struct WindowEvent
	{
		ore::t_UInt8 type;
		short windowWidth, windowHeight, windowX, windowY;
	};

	//	t_Event is the adaptation that provides integration with various 3rd-party input mechanisms;
	//	InputStateChange is the internal data container used to pass around event information within ORGUI.
	union InputStateChange
	{
		ore::t_UInt8 type;

		KeyboardEvent	keyboardEvent;
		MouseEvent		mouseEvent;
		WindowEvent		windowEvent;
	};
}

#endif