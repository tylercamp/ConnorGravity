
#ifndef _ORGUI_INPUT_KEY_CODES_H_
#define _ORGUI_INPUT_KEY_CODES_H_

/*
Credit can be given to Sam Lantinga, writer of the SDL, for organization
and definition of keycode mechanisms. I would've kept the names
the same here as well if the prefix didn't cause confusion.

As a side-note, while SDL did provide many more key definitions than are
used here, I didn't want to make the creation of ports for other systems
any more tedious than it already was. (i.e. using SFML or Irrlicht keycodes 
instead of SDL.)
*/

namespace gui
{

	enum t_KeyCode
	{
		GUIK_BACKSPACE		= 8,
		GUIK_TAB		= 9,
		GUIK_CLEAR		= 12,
		GUIK_RETURN		= 13,
		GUIK_PAUSE		= 19,
		GUIK_ESCAPE		= 27,
		GUIK_SPACE		= 32,
		GUIK_EXCLAIM		= 33,
		GUIK_QUOTEDBL		= 34,
		GUIK_HASH		= 35,
		GUIK_DOLLAR		= 36,
		GUIK_AMPERSAND		= 38,
		GUIK_QUOTE		= 39,
		GUIK_LEFTPAREN		= 40,
		GUIK_RIGHTPAREN		= 41,
		GUIK_ASTERISK		= 42,
		GUIK_PLUS		= 43,
		GUIK_COMMA		= 44,
		GUIK_MINUS		= 45,
		GUIK_PERIOD		= 46,
		GUIK_SLASH		= 47,
		GUIK_0			= 48,
		GUIK_1			= 49,
		GUIK_2			= 50,
		GUIK_3			= 51,
		GUIK_4			= 52,
		GUIK_5			= 53,
		GUIK_6			= 54,
		GUIK_7			= 55,
		GUIK_8			= 56,
		GUIK_9			= 57,
		GUIK_COLON		= 58,
		GUIK_SEMICOLON		= 59,
		GUIK_LESS		= 60,
		GUIK_EQUALS		= 61,
		GUIK_GREATER		= 62,
		GUIK_QUESTION		= 63,
		GUIK_AT			= 64,
		/* Skip uppercase letters */

		GUIK_LEFTBRACKET	= 91,
		GUIK_BACKSLASH		= 92,
		GUIK_RIGHTBRACKET	= 93,
		GUIK_CARET		= 94,
		GUIK_UNDERSCORE		= 95,
		GUIK_BACKQUOTE		= 96,
		GUIK_a			= 97,
		GUIK_b			= 98,
		GUIK_c			= 99,
		GUIK_d			= 100,
		GUIK_e			= 101,
		GUIK_f			= 102,
		GUIK_g			= 103,
		GUIK_h			= 104,
		GUIK_i			= 105,
		GUIK_j			= 106,
		GUIK_k			= 107,
		GUIK_l			= 108,
		GUIK_m			= 109,
		GUIK_n			= 110,
		GUIK_o			= 111,
		GUIK_p			= 112,
		GUIK_q			= 113,
		GUIK_r			= 114,
		GUIK_s			= 115,
		GUIK_t			= 116,
		GUIK_u			= 117,
		GUIK_v			= 118,
		GUIK_w			= 119,
		GUIK_x			= 120,
		GUIK_y			= 121,
		GUIK_z			= 122,
		GUIK_DELETE		= 127,

		/* Numeric keypad */
		GUIK_KP0		= 256,
		GUIK_KP1		= 257,
		GUIK_KP2		= 258,
		GUIK_KP3		= 259,
		GUIK_KP4		= 260,
		GUIK_KP5		= 261,
		GUIK_KP6		= 262,
		GUIK_KP7		= 263,
		GUIK_KP8		= 264,
		GUIK_KP9		= 265,
		GUIK_KP_PERIOD		= 266,
		GUIK_KP_DIVIDE		= 267,
		GUIK_KP_MULTIPLY	= 268,
		GUIK_KP_MINUS		= 269,
		GUIK_KP_PLUS		= 270,
		GUIK_KP_ENTER		= 271,
		GUIK_KP_EQUALS		= 272,

		/* Arrows + Home/End pad */
		GUIK_UP			= 273,
		GUIK_DOWN		= 274,
		GUIK_RIGHT		= 275,
		GUIK_LEFT		= 276,
		GUIK_INSERT		= 277,
		GUIK_HOME		= 278,
		GUIK_END		= 279,
		GUIK_PAGEUP		= 280,
		GUIK_PAGEDOWN		= 281,

		/** Function keys */
		GUIK_F1			= 282,
		GUIK_F2			= 283,
		GUIK_F3			= 284,
		GUIK_F4			= 285,
		GUIK_F5			= 286,
		GUIK_F6			= 287,
		GUIK_F7			= 288,
		GUIK_F8			= 289,
		GUIK_F9			= 290,
		GUIK_F10		= 291,
		GUIK_F11		= 292,
		GUIK_F12		= 293,

		/* Key state modifier keys */
		GUIK_NUMLOCK		= 300,
		GUIK_CAPSLOCK		= 301,
		GUIK_SCROLLOCK		= 302,
		GUIK_RSHIFT		= 303,
		GUIK_LSHIFT		= 304,
		GUIK_RCTRL		= 305,
		GUIK_LCTRL		= 306,
		GUIK_RALT		= 307,
		GUIK_LALT		= 308,

		GUIK_LAST
	};

	enum t_KeyModifier
	{
		GUIM_LALT		= 0x01,
		GUIM_RALT		= 0x02,
		GUIM_LSHIFT		= 0x04,
		GUIM_RSHIFT		= 0x08,
		GUIM_LCTRL		= 0x10,
		GUIM_RCTRL		= 0x20,
		GUIM_CAPSLOCK	= 0x40,

		GUIM_ALT		= 0x03,
		GUIM_SHIFT		= 0x0C,
		GUIM_CTRL		= 0x30
	};

}

#endif