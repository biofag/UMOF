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

#ifndef TYPE_H
#define TYPE_H

#include "defines.h"
#include "type_traits.h"

class Any;

/* \breif The Type class manages types.
*/
class UMOF_EXPORT Type
{
public:
	/*! Constructs the meta type with the given table.
		Type constructor should never be used directly.
		Please use from() function instead.
	*/
	Type(const TypeTable *table);

	/*! Construct the meta information for the given type T.
	*/
	template<class T>
	static Type from();

	/*! Return true if type are equal.
		Any qualifiers will be omitted.
		So int, int&, int&&, const int& are all the same types.
	*/
	bool operator==(const Type& other);

	/*! Checks whether Type is valid object.
		Call to invalid type will result in application crash.
	*/
	bool valid() const;

	/*! Returns RTTI information
	*/
	std::type_info const& id() const;

	/*! Returns the size of type.
	*/
	int size() const;

	/*! Construct the value of the type in the existing address in memory.
		If copy is provided it will be copy constructed, otherwise it will
		be default constructed. The function returns where.
	*/
	void *construct(void *where, void *const copy = nullptr) const;

	/*! Creates the value of the type.
	*/
	void *create(void *const copy = nullptr) const;

	/*! Destructs the value located at data.
	*/
	void destruct(void *data) const;

	/*! Destruct the value and free memory.
	*/
	void destroy(void *data) const;

private:
	const TypeTable *_table;

};

template<class T>
Type Type::from()
{
	return Table<T>::get();
}

#endif