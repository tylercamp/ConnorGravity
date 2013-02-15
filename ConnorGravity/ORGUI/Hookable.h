
#ifndef _ORGUI_HOOKABLE_H_
#define _ORGUI_HOOKABLE_H_

#include <map>
#include <exception>
#include <ORE/Diagnostics.h>

/* Implements the hooking system; Callback is the generic class used for calling functions/methods while Function and
	Method are the specific implementations. They are generated within the HOOK_FUNCTION and HOOK_METHOD macros and
	referenced within the class derived from Hookable. */

namespace gui
{

#define HandleExceptionMethod()	\
	ore::Diagnostics::Error (true).Append ("ORGUI Hookable.h | Method::operator(...) - Attempted to call method pointer of value NULL.").Display ();
#define HandleExceptionFunction()	\
	ore::Diagnostics::Error (true).Append ("ORGUI Hookable.h | Function::operator(...) - Attempted to call function pointer of value NULL.").Display ();

	template <typename ReturnType = void, typename ParameterType = void>
	class Callback
	{
	public:
		virtual ReturnType	operator () (ParameterType) = 0;
		virtual	bool		IsValid () = 0;
		virtual				~Callback () {}
	};

	template <typename ReturnType>
	class Callback <ReturnType, void>
	{
	public:
		virtual ReturnType	operator () () = 0;
		virtual bool		IsValid () = 0;
		virtual				~Callback () {}
	};

	template <typename ClassType, typename ReturnType, typename ParameterType>
	class Method
		: public Callback <ReturnType, ParameterType>
	{
	private:
		ReturnType (ClassType::*m_Method) (ParameterType);
		ClassType * m_Instance;
	public:
		Method ()
			: m_Method (NULL), m_Instance (NULL)
		{
		}

		Method (ReturnType (ClassType::*method)(ParameterType), ClassType * instance)
			: m_Method (method), m_Instance (instance)
		{
		}

		Method (const Method & o)
			: m_Method (o.m_Method), m_Instance (o.m_Instance)
		{
		}

		virtual ReturnType operator () (ParameterType Parameter)
		{
			if (m_Method)
				return (m_Instance->*m_Method) (Parameter);
			else
				HandleExceptionMethod ();
		}

		virtual bool IsValid ()
		{
			return m_Method != NULL && m_Instance != NULL;
		}
	};

	template <typename ReturnType, typename ParameterType>
	class Function
		: public Callback <ReturnType, ParameterType>
	{
	private:
		ReturnType (*m_Function) (ParameterType);
	public:
		Function ()
			: m_Function (NULL)
		{
		}

		Function (ReturnType (*function)(ParameterType))
			: m_Function (function)
		{
		}

		Function (const Function & o)
			: m_Function (o.m_Function)
		{
		}

		virtual ReturnType operator () (ParameterType parameter)
		{
			if (m_Function)
				return (*m_Function) (parameter);
			else
				HandleExceptionFunction ()
		}

		virtual bool IsValid ()
		{
			return pFunction != NULL;
		}
	};

	template <typename ClassType, typename ReturnType>
	class Method <ClassType, ReturnType, void>
		: public Callback <ReturnType, void>
	{
	private:
		ReturnType (ClassType::*m_Method) ();
		ClassType * m_Instance;
	public:
		Method ()
			: m_Method (NULL), m_Instance (NULL)
		{
		}

		Method (ReturnType (ClassType::*method)(), ClassType * instance)
			: m_Method (method), m_Instance (instance)
		{
		}

		Method (const Method & o)
			: m_Method (o.m_Method), m_Instance (o.m_Instance)
		{
		}

		virtual ReturnType operator () ()
		{
			if (m_Method)
				return (m_Instance->*m_Method) ();
			else
				HandleExceptionMethod ();
		}

		virtual bool IsValid ()
		{
			return m_Method != NULL && m_Instance != NULL;
		}
	};

	template <typename ReturnType>
	class Function <ReturnType, void>
		: public Callback <ReturnType, void>
	{
	private:
		ReturnType (*m_Function) ();
	public:
		Function ()
			: m_Function (NULL)
		{
		}

		Function (ReturnType (*function)())
			: m_Function (function)
		{
		}

		Function (const Function & o)
			: m_Function (o.m_Function)
		{
		}

		virtual ReturnType operator () ()
		{
			if (m_Function)
				return (*m_Function) ();
			else
				HandleExceptionFunction ()
		}

		virtual bool IsValid ()
		{
			return pFunction != NULL;
		}
	};

	template <typename ClassType, typename ParameterType>
	class Method <ClassType, void, ParameterType>
		: public Callback <void, ParameterType>
	{
	private:
		void (ClassType::*m_Method) (ParameterType);
		ClassType * m_Instance;
	public:
		Method ()
			: m_Method (NULL), m_Instance (NULL)
		{
		}

		Method (void (ClassType::*method)(ParameterType), ClassType * instance)
			: m_Method (method), m_Instance (instance)
		{
		}

		Method (const Method & o)
			: m_Method (o.m_Method), m_Instance (o.m_Instance)
		{
		}

		virtual void operator () (ParameterType Parameter)
		{
			if (m_Method)
				(m_Instance->*m_Method) (Parameter);
			else
				HandleExceptionMethod ();
		}

		virtual bool IsValid ()
		{
			return m_Method != NULL && m_Instance != NULL;
		}
	};

	template <typename ParameterType>
	class Function <void, ParameterType>
		: public Callback <void, ParameterType>
	{
	private:
		void (*m_Function) (ParameterType);
	public:
		Function ()
			: m_Function (NULL)
		{
		}

		Function (void (*function)(ParameterType))
			: m_Function (function)
		{
		}

		Function (const Function & o)
			: m_Function (o.m_Function)
		{
		}

		virtual void operator () (ParameterType parameter)
		{
			if (m_Function)
				(*m_Function) (parameter);
			else
				HandleExceptionFunction ()
		}

		virtual bool IsValid ()
		{
			return m_Function != NULL;
		}
	};

	template <typename ClassType>
	class Method <ClassType, void, void>
		: public Callback <void, void>
	{
	private:
		void (ClassType::*m_Method) ();
		ClassType * m_Instance;
	public:
		Method ()
			: m_Method (NULL), m_Instance (NULL)
		{
		}

		Method (void (ClassType::*method)(), ClassType * instance)
			: m_Method (method), m_Instance (instance)
		{
		}

		Method (const Method & o)
			: m_Method (o.m_Method), m_Instance (o.m_Instance)
		{
		}

		virtual void operator () ()
		{
			if (m_Method)
				(m_Instance->*m_Method) ();
			else
				HandleExceptionMethod ();
		}

		virtual bool IsValid ()
		{
			return m_Method != NULL && m_Instance != NULL;
		}

		void SetInstance (ClassType * instance)
		{
			m_Instance = instance;
		}

		ClassType * GetInstance ()
		{
			return m_Instance; 
		}
	};

	template <>
	class Function <void, void>
		: public Callback <void, void>
	{
	private:
		void (*m_Function) ();
	public:
		Function ()
			: m_Function (NULL)
		{
		}

		Function (void (*function)())
			: m_Function (function)
		{
		}

		Function (const Function & o)
			: m_Function (o.m_Function)
		{
		}

		virtual void operator () ()
		{
			if (m_Function)
				(*m_Function) ();
			else
				HandleExceptionFunction ()
		}

		virtual bool IsValid ()
		{
			return m_Function != NULL;
		}
	};

#define HOOK_FUNCTION(func, returnType, argType) ((void *)new gui::Function <returnType,argType> (func))
#define HOOK_METHOD(func, returnType, argType, classType, instance) ((void *)new gui::Method <classType,returnType,argType> (&classType::func, instance))

	// The hope was that we could avoid having to specify the return and argument types every single time, but
	//	that never really came to fruition. I'm sure there's some magical template way to do it, but it's not
	//	of enough concern at this point to screw with.

	//#define HOOK_FUNCTION(func) ((void *)new Function <void,void>((void (*)())(func)))
	//#define HOOK_METHOD(func, classType, instance) ((void *)new Method <classType,void,void> ((void (classType::*)())&(func), instance))

	typedef std::multimap <int, void *>::iterator t_HookLocation;

	class Hookable
	{
	public:
		/*
		template <typename ReturnType, typename ParameterType, typename ClassType>
		t_HookLocation AddHook (int hookTarget, ClassType * referredInstance, ReturnType (ClassType::*hookFunction)(ParameterType))
		{
		return m_Hooks.insert (std::pair <int, void *> (hookTarget, (void *)new Method (referredInstance, hookFunction)));
		}

		template <typename ReturnType, typename ParameterType>
		t_HookLocation AddHook <ReturnType, ParameterType, void> (int hookTarget, ReturnType (*hookFunction) (ParameterType))
		{
		return m_Hooks.insert (std::pair <int, void *> (hookTarget, (void *)new Function (hookFunction)));
		}
		*/

		virtual ~Hookable ()
		{
			for (auto iter = m_Hooks.begin (); iter != m_Hooks.end (); iter++)
			{
				free (iter->second);
			}
		}

		t_HookLocation AddHook (int hookTarget, void * hookFunction)
		{
			return m_Hooks.insert (std::pair <int, void *> (hookTarget, hookFunction));
		}

		void RemoveHook (t_HookLocation location)
		{
			m_Hooks.erase (location);
		}

		bool HookExists (int hookName)
		{
			return m_Hooks.find (hookName) != m_Hooks.end ();
		}

		//	Assumes return type is insignificant
		template <typename ParameterType>
		void CallHooks (int hookName, ParameterType param)
		{
			std::pair <t_HookLocation, t_HookLocation> bounds = m_Hooks.equal_range (hookName);
			for (t_HookLocation iter = bounds.first; iter != bounds.second; iter++)
			{
				(*(Callback <void, ParameterType> *)(iter->second)) (param);
			}
		}

		//	TODO: CallHooks implementation allowing functions to return data

	private:
		std::multimap <int, void *> m_Hooks;
	};

}

#endif