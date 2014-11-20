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

#ifndef ANY_H
#define ANY_H

#include "defines.h"
#include "type_traits.h"
#include <new>
#include <stdexcept>

template<typename T, typename S>
struct AnyHelper;

//Type helper for small types, such as pointers
template<typename T>
struct AnyHelper<T, True>
{
	typedef typename std::is_pointer<T>::type is_pointer;
	typedef typename std::decay<T>::type T_dec;
	typedef typename Pointer<T_dec, is_pointer>::type T_no_cv;

	inline static void clone(const T **src, void **dest)
	{
		new (dest)T(*reinterpret_cast<T const*>(src));
	}

	inline static T *cast(void **object)
	{
		return const_cast<T*>(reinterpret_cast<T_no_cv*>(object));
	}
};

//Type helper for bigger types
template<typename T>
struct AnyHelper<T, False>
{
	typedef typename std::is_pointer<T>::type is_pointer;
	typedef typename std::decay<T>::type T_dec;
	typedef typename Pointer<T_dec, is_pointer>::type T_no_cv;

	inline static void clone(const T **src, void **dest)
	{
		*dest = new T(**src);
	}

	inline static T *cast(void **object)
	{
		return const_cast<T*>(reinterpret_cast<T_no_cv*>(*object));
	}
};

/*! \breif The Any class holds the copy of any data type.
*/
class UMOF_EXPORT Any
{
public:
	/*! Constructs invalid Any holder.
	*/
	Any();

	/*! Copy value from other holder.
	*/
	Any(Any const& other);

	/*! Move value from other holder.
	*/
	Any(Any &&other);

	/*! Destroys Any and contained object.
	*/
	~Any();

	/*! Constructs Any with the given value.
	*/
	template<typename T>
	Any(T const& value);

	/*! Constructs Any with array value.
		Special constructor for static arrays.
	*/
	template<typename T, std::size_t N>
	Any(T(&value)[N]);

	/*! Destroys containing object and set new value.
	*/
	template<typename T>
	void reset(T const& value);

	/*! Destroys containing object.
	*/
	void reset();

	/*! Get containing object type information.
	*/
	std::type_info const& type() const;

private:
	template<typename T>
	friend T* any_cast(Any*);

	TypeTable* _table;
	void* _object;
};

template<typename T>
Any::Any(T const& x) :
	_table(Table<T>::get()),
	_object(nullptr)
{
	const T *src = &x;
	AnyHelper<T, typename Table<T>::is_small>::clone(&src, &_object);
}

template<typename T, std::size_t N>
Any::Any(T(&x)[N]) :
	_table(Table<T*>::get()),
	_object(nullptr)
{
	new (&_object) T*(&x[0]);
}

template<typename T>
void Any::reset(T const& x)
{
	if (_table)
		_table->static_delete(&_object);
	_table = Table<T>::get();
	const T *src = &x;
	AnyHelper<T, typename Table<T>::is_small>::clone(&src, &_object);
}

template <typename T>
inline T* any_cast(Any* operand)
{
	if (operand && operand->_table == Table<T>::get())
		return AnyHelper<T, typename Table<T>::is_small>::cast(&operand->_object);

	return nullptr;
}

template <typename T>
inline T* any_cast(Any const* operand)
{
	return any_cast<T>(const_cast<Any*>(operand));
}

/*! Casts Any container to a given type T.
	\relates Any
	Use it as follows:
	\code{.cpp}
	Any a{5.0};
	double d = any_cast<double>(a);
	\endcode
*/
template <typename T>
inline T any_cast(Any& operand)
{
	typedef typename std::remove_reference<T>::type nonref;

	nonref* result = any_cast<nonref>(&operand);
	if (!result)
		throw std::runtime_error("Bad cast");
	return *result;
}

/*! Casts Any container to a given type T.
	\relates Any
	Use it as follows:
	\code{.cpp}
	Any a{5.0};
	double d = any_cast<double>(a);
	\endcode
*/
template <typename T>
inline T const& any_cast(Any const& operand)
{
	typedef typename std::remove_reference<T>::type nonref;
	return any_cast<nonref const&>(const_cast<Any&>(operand));
}

#endif