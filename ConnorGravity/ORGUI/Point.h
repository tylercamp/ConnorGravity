
#ifndef _ORGUI_POINT_H_
#define _ORGUI_POINT_H_

/* Definitions for the Point and ComplexPoint classes; Point is just a point, while ComplexPoint is a point with
	texture coordinates. Conversion between the two generally means that the texture coordinates are dropped
	somewhere along the line. */

namespace gui
{
	struct Point
	{
		float x, y;

		Point ()
		{
		}

		Point (float xx, float yy)
			: x (xx), y (yy)
		{
		}

		Point (const Point & p)
			: x (p.x), y (p.y)
		{
		}

		Point & operator= (const Point & p)
		{
			x = p.x; y = p.y;
			return *this;
		}

		Point & operator+= (const Point & p)
		{
			x += p.x; y += p.y;
			return *this;
		}

		Point & operator-= (const Point & p)
		{
			x -= p.x; y -= p.y;
			return *this;
		}

		Point operator+ (const Point & p) const
		{
			Point c (*this);
			c.x -= p.x; c.y -= p.y;
			return c;
		}

		Point operator- (const Point & p) const
		{
			Point c (*this);
			c.x -= p.x; c.y -= p.y;
			return c;
		}
	};

	struct ComplexPoint
		: public Point
	{
		float tx, ty;

		ComplexPoint ()
			: tx (0.f), ty (0.f)
		{
		}

		ComplexPoint (float xx, float yy)
			: Point (xx, yy), tx (0.f), ty (0.f)
		{
		}

		ComplexPoint (float xx, float yy, float txx, float tyy)
			: Point (xx, yy), tx (txx), ty (tyy)
		{
		}

		ComplexPoint (const ComplexPoint & p)
			: Point (p.x, p.y), tx (p.tx), ty (p.ty)
		{
		}

		ComplexPoint (const Point & p)
			: Point (p), tx (0.f), ty (0.f)
		{
		}

		ComplexPoint & operator= (const ComplexPoint & p)
		{
			x = p.x; y = p.y; tx = p.tx; ty = p.ty;
			return *this;
		}
	};

	inline ComplexPoint ConvertPoint (const Point & p)
	{
		return ComplexPoint (p.x, p.y);
	}

	inline Point ConvertPoint (const ComplexPoint & p)
	{
		return Point (p.x, p.y);
	}
}

#endif