
#ifndef _ORGUI_TEXTBOX_H_
#define _ORGUI_TEXTBOX_H_

#include "InterfaceComponent.h"
#include "FontModule.h"
#include "Shapes.h"
#include "Color.h"

namespace gui
{
	class Text
		: public InterfaceComponent
	{
	public:
		ore::t_String m_Text, m_Font;
		Color m_Color;

		Point m_Position;

		Text ();

		void ProcessStateChange (InputStateChange * e);
		virtual void Render (Rect * containerRect);
		void Center (bool x, bool y);

		static Text * Create (ore::t_String font, ore::t_String text);
	};

	//	Text StateChangeData
	struct SCDTextRender
	{
		Text * text;
		Rect * containerRect;
		
		enum {
			PRERENDER,
			POSTRENDER
		} renderStage;
	};
}

#endif