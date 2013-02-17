
#include "Module.h"
#include <ORE/Diagnostics.h>

namespace gui
{

	void Module::OverloadFunction (t_ModuleFunctionID id, t_ModuleFunction func)
	{
		auto location = m_FunctionMappings.find (id);
		if (location != m_FunctionMappings.end ())
		{
#ifdef ORE_DEBUG
			ore::Diagnostics::Message (true)
				.Append ("ORGUI Module.cpp | Module::OverloadFunction - Overloading already-loaded function with ID ").Append (id)
				.Display ();
#endif
		}

		m_FunctionMappings [id] = func;
	}

	t_ModuleFunction Module::GetFunction (t_ModuleFunctionID id)
	{
		auto location = m_FunctionMappings.find (id);
		if (location == m_FunctionMappings.end ())
		{
			ore::Diagnostics::Warning (true)
				.Append ("OREGUI Module.cpp | Module::GetFunction - Unable to find function mapped to ID ").Append (id)
				.Display ();
			return NULL;
		}

		return location->second;
	}
}