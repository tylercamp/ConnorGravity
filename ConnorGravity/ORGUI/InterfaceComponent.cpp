
#include "ORGUI.h"

namespace gui
{
	InterfaceComponent::InterfaceComponent ()
	{
		m_Shape = NULL;
		m_Parent = NULL;
	}

	InterfaceComponent::~InterfaceComponent ()
	{
	}

	Point InterfaceComponent::GetBaseRenderPosition ()
	{
		Rect boundingArea;
		m_Shape->GetBoundingBox (&boundingArea);
		return boundingArea.position;
	}

	ComplexInterfaceComponent::~ComplexInterfaceComponent ()
	{
		for (auto iter = m_Children.begin (); iter != m_Children.end (); iter++)
		{
			delete (*iter);
		}
	}

	void ComplexInterfaceComponent::MoveChild (InterfaceComponent * component, ComplexInterfaceComponent * destination)
	{
		DetachChild (component);
		destination->AddChild (component);
	}

	void ComplexInterfaceComponent::AddChild (InterfaceComponent * component)
	{
		component->m_Parent = this;
		m_Children.push_back (component);
		component->m_LocationInParent = --m_Children.end ();
	}

	void ComplexInterfaceComponent::DetachChild (InterfaceComponent * component)
	{
		m_Children.erase (component->m_LocationInParent);
		component->m_Parent = NULL;
	}

	int ComplexInterfaceComponent::ChildCount ()
	{
		return m_Children.size ();
	}

	void ComplexInterfaceComponent::Render (Rect * container)
	{
		g_RenderModule->PushTransformState ();

		Rect representation;
		if (m_Shape)
		{
			m_Shape->GetBoundingBox (&representation);
			g_RenderModule->Translate (representation.position.x, representation.position.y);
			representation.position.x += container->position.x;
			representation.position.y += container->position.y;
		}
		else
		{
			representation = *container;
		}

		

		SCDRender render;
		render.containerRect = &representation;
		render.object = this;

		CallHooks <SCDRender *> (EVENT_RENDER, &render);
		RenderChildren (container);

		g_RenderModule->PopTransformState ();
	}

	void ComplexInterfaceComponent::RenderChildren (Rect * container)
	{
		Rect representation;
		if (m_Shape)
		{
			m_Shape->GetBoundingBox (&representation);
			representation.position.x += container->position.x;
			representation.position.y += container->position.y;
		}
		else
		{
			representation = *container;
		}

		for (auto iter = m_Children.begin (); iter != m_Children.end (); iter++)
		{
			(*iter)->Render (&representation);
		}
	}

	void ComplexInterfaceComponent::ProcessChildrenStateChanges (InputStateChange * e)
	{
		for (auto iter = m_Children.begin (); iter != m_Children.end (); iter++)
		{
			(*iter)->ProcessStateChange (e);
		}
	}

	const std::list <InterfaceComponent *> & ComplexInterfaceComponent::GetChildren ()
	{
		return m_Children;
	}
}