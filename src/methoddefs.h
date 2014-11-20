/*********************************************************************
This file is part of the uMOF library.
Copyright (C) 2014 Artem Shal
artiom.shal@gmail.com

The uMOF library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.
**********************************************************************/

#ifndef METHODDEFS_H
#define METHODDEFS_H

#include "type_traits.h"
#include "any.h"

class Object;

//Helper stuff
namespace unpack
{
	template<unsigned...> struct indices {};

	template<unsigned N, unsigned... Is>
	struct indices_gen : indices_gen < N - 1, N - 1, Is... > {};

	template<unsigned... Is>
	struct indices_gen<0, Is...> : indices < Is... > {};
}

//Function to unpack args properly
template<typename T>
inline static TypeTable *getTable()
{
	return Table<T>::get();
}

//Base template for function and methods invocation
template<typename Signature>
struct Invoker;

//Free functions
template<typename Return, typename... Args>
struct Invoker<Return(*)(Args...)>
{
	// Workaround for the Visual Studio bug
	typedef Return(*Fun)(Args...);

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get(),
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		return f(any_cast<Args>(args[Is])...);
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != sizeof...(Args))
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Free function with no parameters
template<typename Return>
struct Invoker<Return(*)()>
{
	typedef Return(*Fun)();

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get()
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		return f();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != 0)
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

//Void free function
template<typename... Args>
struct Invoker<void(*)(Args...)>
{
	typedef void(*Fun)(Args...);

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr,
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		f(any_cast<Args>(args[Is])...);
		return Any();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != sizeof...(Args))
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Void free function with no arguments
template<>
struct Invoker<void(*)()>
{
	typedef void(*Fun)();

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr
		};
		return staticTypes;
	}

	template<unsigned... Is>
	inline static Any invoke(Object *obj, Fun f, const Any *args, unpack::indices<Is...>)
	{
		f();
		return Any();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != 0)
			throw std::runtime_error("bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

//Base method template
template<typename Class, typename Return, typename... Args>
struct Invoker<Return(Class::*)(Args...)>
{
	typedef Return(Class::*Fun)(Args...);

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get(),
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		return (static_cast<Class *>(obj)->*f)(any_cast<Args>(args[Is])...);
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != sizeof...(Args))
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Method with no parameters
template<typename Class, typename Return>
struct Invoker<Return(Class::*)()>
{
	typedef Return(Class::*Fun)();

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get()
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *, unpack::indices<Is...>)
	{
		return (static_cast<Class *>(obj)->*f)();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != 0)
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

//Void method
template<typename Class, typename... Args>
struct Invoker<void(Class::*)(Args...)>
{
	typedef void(Class::*Fun)(Args...);

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr,
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		(static_cast<Class *>(obj)->*f)(any_cast<Args>(args[Is])...);
		return Any();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != sizeof...(Args))
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Void method with no parameters
template<typename Class>
struct Invoker<void(Class::*)()>
{
	typedef void(Class::*Fun)();

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		(static_cast<Class *>(obj)->*f)();
		return Any();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != 0)
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

//Base const method template
template<typename Class, typename Return, typename... Args>
struct Invoker<Return(Class::*)(Args...)const>
{
	typedef Return(Class::*Fun)(Args...)const;

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get(),
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		return (const_cast<const Class *>(static_cast<Class *>(obj))->*f)(any_cast<Args>(args[Is])...);
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != sizeof...(Args))
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Const method with no parameters
template<typename Class, typename Return>
struct Invoker<Return(Class::*)()const>
{
	typedef Return(Class::*Fun)()const;

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get()
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *, unpack::indices<Is...>)
	{
		return (const_cast<const Class *>(static_cast<Class *>(obj))->*f)();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != 0)
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

//Void const method
template<typename Class, typename... Args>
struct Invoker<void(Class::*)(Args...)const>
{
	typedef void(Class::*Fun)(Args...)const;

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	template<typename T>
	inline static TypeTable *getTable()
	{
		return Table<T>::get();
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr,
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		(const_cast<const Class *>(static_cast<Class *>(obj))->*f)(any_cast<Args>(args[Is])...);
		return Any();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != sizeof...(Args))
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Void const method with no parameters
template<typename Class>
struct Invoker<void(Class::*)()const>
{
	typedef void(Class::*Fun)()const;

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		(const_cast<const Class *>(static_cast<Class *>(obj))->*f)();
		return Any();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != 0)
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

#endif