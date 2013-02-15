
#include "ORGUI.h"
#include "Events.h"

#include <algorithm>

namespace gui
{
	Button::Button (Shape * shape)
	{
		m_Shape = shape;
	}

	Button::~Button ()
	{
		if (m_Shape)
			delete m_Shape;
	}

	void Button::ProcessStateChange (InputStateChange * e)
	{
		switch (e->type)
		{
		case (MOUSE_BUTTON_DOWN):
			{
				if (!m_Shape->ContainsPoint (Point (e->mouseEvent.mouseX, e->mouseEvent.mouseY)))
					break;

				SCDButtonPress param;
				param.button = this;
				param.mouseButton = e->mouseEvent.button;
				param.mousex = e->mouseEvent.mouseX;
				param.mousey = e->mouseEvent.mouseY;
				CallHooks <SCDButtonPress *> (EVENT_CLICKED, &param);

				break;
			}
		}
	}

	void Button::Render (Rect * containerRect)
	{
		Rect representation;
		m_Shape->GetBoundingBox (&representation);

		g_RenderModule->PushTransformState ();
		g_RenderModule->Translate (representation.position.x, representation.position.y);

		representation.position.x += containerRect->position.x;
		representation.position.y += containerRect->position.y;

		if (HookExists (EVENT_DRAW))
		{
			SCDButtonRender param;
			param.button = this;
			param.containerRect = containerRect;
			CallHooks <SCDButtonRender *> (EVENT_DRAW, &param);
		}
		else
		{
			RenderModule * renderModule = RenderModule::GetRenderModule ();
			renderModule->SetActiveRenderColor (Color (240, 240, 240));
			m_Shape->Render ();
		}

		for (auto iter = m_Children.begin (); iter != m_Children.end (); iter++)
		{
			(*iter)->Render (&representation);
		}

		g_RenderModule->PopTransformState ();
	}
}