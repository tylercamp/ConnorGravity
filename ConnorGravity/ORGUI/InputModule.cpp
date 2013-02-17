
#include "InputModule.h"

namespace gui
{

	using namespace module;

	InputModule::InputModule ()
	{
		//	OverloadFunction (IM, MODULE_FUNCTION (IM));
		OverloadFunction (IM_FILL_STATE_CHANGE_OBJECT, MODULE_FUNCTION (IM_FillStateChangeObject));
	}

	InputModule * InputModule::GetInputModule ()
	{
		static InputModule * inputModule = NULL;
		if (!inputModule)
		{
			inputModule = new InputModule;
		}
		return inputModule;
	}

	void InputModule::ProcessEvent (const module::t_ImplEvent * e)
	{
		InputStateChange newChange;
		((void (*)(const module::t_ImplEvent *, InputStateChange *))GetFunction (IM_FILL_STATE_CHANGE_OBJECT)) (e, &newChange);
		AddStateChange (&newChange);
	}

	bool InputModule::GetNextStateChange (InputStateChange * dest)
	{
		if (m_StateChanges.size () == 0)
			return false;

		memcpy (dest, &(m_StateChanges.front ()), sizeof (InputStateChange));
		m_StateChanges.pop_front ();
		return true;
	}

	void InputModule::AddStateChange (InputStateChange * stateChange)
	{
		m_StateChanges.push_back (*stateChange);
	}

}