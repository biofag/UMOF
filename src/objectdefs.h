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

#ifndef OBJECTDEFS_H
#define OBJECTDEFS_H

#include "defines.h"
#include "method.h"
#include "methoddefs.h"
#include "property.h"
#include "propertydefs.h"

#include <type_traits>
#include <utility>

#define METHOD(m) \
{ \
	#m, \
	(InvokeMem)&Invoker<decltype(&m)>::template invoke<&m>, \
	Invoker<decltype(&m)>::argCount(), \
	Invoker<decltype(&m)>::types() \
}
#define OVERLOAD(c, m, r, ...) \
{ \
	#m, \
	(InvokeMem)&Invoker<r(c::*)(__VA_ARGS__)>::template invoke<&m>, \
	Invoker<r(c::*)(__VA_ARGS__)>::argCount(), \
	Invoker<r(c::*)(__VA_ARGS__)>::types() \
}
#define FUNCTION(f) METHOD(f)
#define PROPERTY(p, r, w) \
{ \
	#p, \
	Reader<decltype(&r), &r>::type(), \
	&Reader<decltype(&r), &r>::read, \
	&Writer<decltype(&w), &w>::write \
}

template<typename T>
struct expose_method
{
private:
	typedef std::true_type yes;
	typedef std::false_type no;
	template<typename U>
	static auto test(int) -> decltype(std::declval<U>().expose(), yes())
	{
		return yes();
	}
	template<typename>
	static no test(...)
	{
		return no();
	}
	static const std::pair<int, const MethodTable *> exec_impl(std::true_type)
	{
		return T::expose();
	}
	static const std::pair<int, const MethodTable *> exec_impl(...)
	{
		return{ 0, nullptr };
	}
public:
	static const std::pair<int, const MethodTable *> exec()
	{
		return exec_impl(test<T>(0));
	}
	enum { exists = std::is_same<decltype(test<T>(0)), yes>::value };
};

template<typename T>
struct expose_props_method
{
private:
	typedef std::true_type yes;
	typedef std::false_type no;
	template<typename U>
	static auto test(int) -> decltype(std::declval<U>().expose_props(), yes())
	{
		return yes();
	}
	template<typename>
	static no test(...)
	{
		return no();
	}
	static const std::pair<int, const PropertyTable *> exec_impl(std::true_type)
	{
		return T::expose_props();
	}
	static const std::pair<int, const PropertyTable *> exec_impl(...)
	{
		return{ 0, nullptr };
	}
public:
	static const std::pair<int, const PropertyTable *> exec()
	{
		return exec_impl(test<T>(0));
	}
	enum { exists = std::is_same<decltype(test<T>(0)), yes>::value };
};

#endif