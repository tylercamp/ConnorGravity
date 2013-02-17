
#ifndef _ORGUI_INTERFACE_COMPONENT_H_
#define _ORGUI_INTERFACE_COMPONENT_H_

#include <list>
#include "Hookable.h"
#include "InputStateChange.h"

#include "Shapes.h"

/* Defines the base classes for all GUI components. InterfaceComponent is renderable and can process input, while
	a ComplexInterfaceComponent has all of the aforementioned functionality + children. */

namespace gui
{
	enum
	{
		EVENT_RENDER
	};

	class ComplexInterfaceComponent;
	class InterfaceComponent;

	class InterfaceComponent
		: public Hookable
	{
	protected:
		ComplexInterfaceComponent * m_Parent;
		std::list <InterfaceComponent *>::iterator m_LocationInParent;

	public:

		InterfaceComponent ();
		virtual ~InterfaceComponent ();

		friend class ComplexInterfaceComponent;

		Shape * m_Shape;

		virtual Point GetBaseRenderPosition ();
		virtual void ProcessStateChange (InputStateChange * e) = 0;
		virtual void Render (Rect * boundingArea) = 0;
	};

	class ComplexInterfaceComponent
		: public InterfaceComponent
	{
	public:
		virtual ~ComplexInterfaceComponent ();

		//	TODO: Implement, moves a child from the current CIC to the given CIC
		virtual void MoveChild (InterfaceComponent * component, ComplexInterfaceComponent * destination);
		virtual void AddChild (InterfaceComponent * component);
		virtual void DetachChild (InterfaceComponent * component);
		virtual int ChildCount ();

		virtual void Render (Rect * boundingArea);
		virtual void RenderChildren (Rect * boundingArea);

		virtual void ProcessChildrenStateChanges (InputStateChange * e);

		const std::list <InterfaceComponent *> & GetChildren ();

	protected:
		std::list <InterfaceComponent *> m_Children;
	};

	struct SCDRender
	{
		void * object;
		Rect * containerRect;
	};
}

#endif