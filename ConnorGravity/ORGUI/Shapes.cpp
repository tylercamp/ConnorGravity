
#include "Shapes.h"
#include "RenderModule.h"

#include <cmath>

#define sqr(x) ((x)*(x))

namespace gui
{
	Shape::Shape ()
	{
		texture = -1;
	}

	Shape::~Shape ()
	{
	}

	bool Shape::ContainsPoint (const Point * p) const
	{
		return ContainsPoint (*p);
	}

	void Circle::GetBoundingBox (Rect * dest) const
	{
		dest->position = Point (position.x - radius, position.y - radius);
		dest->size = Point (radius * 2, radius * 2);
	}

	bool Circle::ContainsPoint (const Point & p) const
	{
		return sqrtf (sqr (p.x - position.x) + sqr (p.y - position.y)) <= radius;
	}

	void Circle::Render (float x, float y) const
	{
		RenderModule * renderModule = RenderModule::GetRenderModule ();
		if (texture != -1)
			renderModule->SetCurrentTexture (texture);
		else
			renderModule->RenderWithoutTexturing ();

		renderModule->RenderCircle (Point (position.x + x, position.y + y), radius, 32);
	}

	Rect::Rect ()
	{
	}

	Rect::Rect (const Point & positionIn, const Point & sizeIn)
	{
		position = positionIn;
		size = sizeIn;
	}

	Rect::Rect (float x, float y, float w, float h)
	{
		position.x = x;
		position.y = y;
		size.x = w;
		size.y = h;
	}

	void Rect::GetBoundingBox (Rect * dest) const
	{
		dest->position = position;
		dest->size = size;
	}

	bool Rect::ContainsPoint (const Point & p) const
	{
		return (
			p.x >= position.x &&
			p.x <= position.x + size.x &&
			p.y >= position.y &&
			p.y <= position.y + size.y
		);
	}

	void Rect::Render (float x, float y) const
	{
		RenderModule * renderModule = RenderModule::GetRenderModule ();
		if (texture != -1)
			renderModule->SetCurrentTexture (texture);
		else
			renderModule->RenderWithoutTexturing ();

		renderModule->RenderQuad (
			ComplexPoint (position.x + x, position.y + y, 0.f, 0.f),
			ComplexPoint (position.x + x, position.y + y + size.y, 0.f, 1.f),
			ComplexPoint (position.x + x + size.x, position.y + y + size.y, 1.f, 1.f),
			ComplexPoint (position.x + x + size.x, position.y + y, 1.f, 0.f)
		);
	}
}