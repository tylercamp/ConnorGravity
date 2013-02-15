
#include "Text.h"
#include "RenderModule.h"

namespace gui
{
	Text::Text ()
		: m_Color (0, 0, 0, 255), m_Position (0.f, 0.f)
	{
	}

	void Text::ProcessStateChange (InputStateChange * e)
	{
	}

	void Text::Render (Rect * containerRect)
	{
		FontModule * fontModule = FontModule::GetFontModule ();

		if (m_Font.size () == 0)
		{
			ore::Diagnostics::Warning (true)
				.Append ("gui::Text::Render - Attempting to render text using unassigned font; text:\n")
				.Append (m_Text)
				.Display ();
		}

		RenderModule::GetRenderModule ()->SetActiveRenderColor (m_Color);

		fontModule->SetActiveFont (m_Font);
		fontModule->RenderString (containerRect->position.x + m_Position.x, containerRect->position.y + m_Position.y, m_Text);
	}

	Text * Text::Create (ore::t_String font, ore::t_String text)
	{
		Text * newText = new Text;
		newText->m_Font = font;
		newText->m_Text = text;
		return newText;
	}

	void Text::Center (bool x, bool y)
	{
		if (!m_Parent)
			return;

		Rect boundingBox;
		m_Parent->m_Shape->GetBoundingBox (&boundingBox);

		if (x)
		{
			float textWidth = FontModule::GetFontModule ()->GetStringWidth (m_Text);
			m_Position.x = boundingBox.size.x / 2.f - textWidth / 2.f;
		}

		if (y)
		{
			float textHeight = FontModule::GetFontModule ()->GetStringHeight (m_Text);
			m_Position.y = boundingBox.size.y / 2.f - textHeight / 2.f;
		}
	}
}