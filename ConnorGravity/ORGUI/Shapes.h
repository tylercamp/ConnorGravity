
#ifndef _ORGUI_SHAPES_H_
#define _ORGUI_SHAPES_H_

#include "Point.h"
#include "Color.h"
#include <ORE/Types.h>

/* Shape definitions; A shape must be able to determine if a point exists within its bounds, it must have
	a method for rendering itself defined, and it must be able to provide a bounding box representing an
	estimate of the area that it takes up. */

namespace gui
{
	struct Rect;

	struct Shape
	{
	public:
		Point position;
		Color color;
		ore::t_UInt64 texture;

		Shape ();
		virtual ~Shape ();

		virtual bool ContainsPoint (const Point & p) const = 0;
		bool ContainsPoint (const Point * p) const;

		virtual void GetBoundingBox (Rect * dest) const = 0;

		virtual void Render (float x = 0.f, float y = 0.f) const = 0;
	};

	struct Circle
		: public Shape
	{
		float radius;

		void GetBoundingBox (Rect * dest) const;

		bool ContainsPoint (const Point & p) const;
		void Render (float x = 0.f, float y = 0.f) const;
	};

	struct Rect
		: public Shape
	{
		Point size;

		Rect ();
		Rect (const Point & position, const Point & size);
		Rect (float x, float y, float w, float h);

		void GetBoundingBox (Rect * dest) const;

		bool ContainsPoint (const Point & p) const;
		void Render (float x = 0.f, float y = 0.f) const;
	};
}

#endif