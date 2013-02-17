
#ifndef _ORGUI_MODULE_H_
#define _ORGUI_MODULE_H_

#define MODULE_FUNCTION(x) (static_cast <void *> (x))

typedef int t_ModuleFunctionID;
typedef void * t_ModuleFunction;

#include <map>

namespace gui
{

	class Module
	{
	public:
		void OverloadFunction (t_ModuleFunctionID id, t_ModuleFunction func);
		t_ModuleFunction GetFunction (t_ModuleFunctionID id);

	protected:
		std::map <t_ModuleFunctionID, t_ModuleFunction> m_FunctionMappings;
	};

}

#endif