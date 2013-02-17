#ifndef _ORGUI_BUTTON_H_
#define _ORGUI_BUTTON_H_

#include "Shapes.h"
#include "InterfaceComponent.h"

namespace gui
{
	class Button
		: public ComplexInterfaceComponent
	{
	public:
		static enum t_ButtonShape
		{
			SHAPE_RECTANGLE,
			SHAPE_CIRCLE
		};

		Button (Shape * shape = NULL);
		~Button ();

		void ProcessStateChange (InputStateChange * e);
		virtual void Render (Rect * containerRect);

		static Button * Create (t_ButtonShape shape);

	private:
		Point m_PreviousMousePosition;
	};

	//	Button StateChangeData
	struct SCDButtonPress
	{
		Button * button;
		char mouseButton;
		int mousex, mousey;
	};

	struct SCDButtonRender
	{
		Button * button;
		Rect * containerRect;
	};
}

#endif