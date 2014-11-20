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

#ifndef PROPERTYDEFS_H
#define PROPERTYDEFS_H

#include "type_traits.h"
#include "any.h"

//Property read
template<typename Signature, Signature S>
struct Reader;

template<typename T, typename Class, T(Class::*ReadFunc)()>
struct Reader<T(Class::*)(), ReadFunc>
{
	inline static const TypeTable *type()
	{
		return Table<T>::get();
	}

	inline static Any read(Object *obj)
	{
		return (static_cast<Class *>(obj)->*ReadFunc)();
	}
};

template<typename T, typename Class, T(Class::*ReadFunc)()const>
struct Reader<T(Class::*)()const, ReadFunc>
{
	inline static const TypeTable *type()
	{
		return Table<T>::get();
	}

	inline static Any read(Object *obj)
	{
		return (static_cast<Class *>(obj)->*ReadFunc)();
	}
};

//Property write
template<typename Signature, Signature S>
struct Writer;

template<typename T, typename Class, void(Class::*WriteFunc)(T)>
struct Writer<void(Class::*)(T), WriteFunc>
{
	inline static void write(Object *obj, const Any& value)
	{
		return (static_cast<Class *>(obj)->*WriteFunc)(any_cast<T>(value));
	}
};

template<typename T, typename Class, void(Class::*WriteFunc)(const T&)>
struct Writer<void(Class::*)(const T&), WriteFunc>
{
	inline static void write(Object *obj, const Any& value)
	{
		return (static_cast<Class *>(obj)->*WriteFunc)(any_cast<T>(value));
	}
};

#endif