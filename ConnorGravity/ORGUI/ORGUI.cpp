
#include "ORGUI.h"
#include "InputModule.h"

namespace gui
{
	RenderModule * g_RenderModule = NULL;
	InputModule * g_InputModule = NULL;
	FontModule * g_FontModule = NULL;

	void InitializeModules (int moduleFlags)
	{
		if (moduleFlags & INIT_FONT_MODULE)
			g_FontModule = FontModule::GetFontModule ();
		if (moduleFlags & INIT_INPUT_MODULE)
			g_InputModule = InputModule::GetInputModule ();
		if (moduleFlags & INIT_RENDER_MODULE)
			g_RenderModule = RenderModule::GetRenderModule ();
	}

	void ORGUI::ProcessStateChange (InputStateChange * e)
	{
		for (auto iter = m_Children.begin (); iter != m_Children.end (); iter++)
		{
			(*iter)->ProcessStateChange (e);
		}
	}

	void ORGUI::PassInputEvent (const module::t_ImplEvent * e)
	{
		InputModule::GetInputModule ()->ProcessEvent (e);
	}

	void ORGUI::ProcessInput ()
	{
		InputStateChange e;
		InputModule * im = InputModule::GetInputModule ();
		while (im->GetNextStateChange (&e))
		{
			ProcessStateChange (&e);
		}
	}
}