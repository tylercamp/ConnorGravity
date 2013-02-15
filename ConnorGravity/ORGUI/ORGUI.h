
#ifndef _ORGUI_H_
#define _ORGUI_H_

#include <ORE/Platform.h>

#ifdef ORE_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <gl/GL.h>

#include "FontModule.h"
#include "InputModule.h"
#include "RenderModule.h"

#include "Button.h"
#include "Text.h"

namespace gui
{
	class ORGUI
		: public ComplexInterfaceComponent
	{
	public:
		virtual void ProcessStateChange (InputStateChange * e);

		void PassInputEvent (const module::t_ImplEvent * e);

		void ProcessInput ();
	};

	extern RenderModule * g_RenderModule;
	extern InputModule * g_InputModule;
	extern FontModule * g_FontModule;

	enum
	{
		INIT_RENDER_MODULE = 0x01,
		INIT_INPUT_MODULE = 0x02,
		INIT_FONT_MODULE = 0x08,
		INIT_ALL = 0xFF
	};

	void InitializeModules (int moduleFlags);
	void UninitializeModules (int moduleFlags);
}

#endif