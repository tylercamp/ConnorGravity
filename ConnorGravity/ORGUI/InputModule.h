
#ifndef _ORGUI_INPUT_MODULE_H_
#define _ORGUI_INPUT_MODULE_H_

#include <ORE/Types.h>
#include <list>

#include "InputEvent.h"
#include "Module.h"
#include "InputStateChange.h"

namespace gui
{

	namespace module
	{
		const t_ModuleFunctionID
			IM_FILL_STATE_CHANGE_OBJECT = 16;

		void IM_FillStateChangeObject (const t_ImplEvent * src, InputStateChange * dest);
	}

	class InputModule
		: public Module
	{
	public:
		InputModule ();

		static InputModule * GetInputModule ();

		/* Event-driven functions, used to update the state checking functions and create new InputStateChange entries */

		/* Updates internal states such that the generic state checking functions operate with the most
		up-to-date information. Note that this automatically invokes GetNextStateChange. */
		void ProcessEvent (const module::t_ImplEvent * e);

		/* Takes the most recent state change, pops it off the list and returns it */
		bool GetNextStateChange (InputStateChange * dest);
		/* Adds a new state change to the end of the list. Can be useful for manually triggering events. */
		void AddStateChange (InputStateChange * stateChange);

	private:
		std::list <InputStateChange> m_StateChanges;
	};

}

#endif