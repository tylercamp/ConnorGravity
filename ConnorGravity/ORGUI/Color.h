
#ifndef _COLOR_H_
#define _COLOR_H_

#include <ORE\Types.h>

/* Defines basic Color type, using 8-bits-per-channel format */

namespace gui
{
	struct Color
	{
		ore::t_Byte r, g, b, a;

		Color ()
			: a (255)
		{
		}

		Color (ore::t_Byte rr, ore::t_Byte gg, ore::t_Byte bb, ore::t_Byte aa = 255)
			: r (rr), g (gg), b (bb), a (aa)
		{
		}

		Color (const Color & c)
			: r (c.r), g (c.g), b (c.b), a (c.a)
		{
		}

		Color & operator= (const Color & o)
		{
			r = o.r; g = o.g;
			b = o.b; a = o.a;
			return *this;
		}
	};

	struct Colorf
	{
		float r, g, b, a;

		Colorf ()
			: a (1.f)
		{
		}

		Colorf (float rr, float gg, float bb, float aa = 1.f)
			: r (rr), g (gg), b (bb), a (aa)
		{
		}

		Colorf (const Color & c)
			: r (c.r), g (c.g), b (c.b), a (c.a)
		{
		}

		Colorf & operator= (const Colorf & o)
		{
			r = o.r; g = o.g;
			b = o.b; a = o.a;
			return *this;
		}
	};


	static const Color COLOR_BLACK	= Color (  0,   0,   0);
	static const Color COLOR_WHITE	= Color (255, 255, 255);
	static const Color COLOR_RED	= Color (255,   0,   0);
	static const Color COLOR_BLUE	= Color (  0,   0, 255);
	static const Color COLOR_GREEN	= Color (  0, 255,   0);
	static const Color COLOR_YELLOW	= Color (255, 245,   0);
}

#endif