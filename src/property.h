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

#ifndef PROPERTY_H
#define PROPERTY_H

#include "defines.h"
#include "conststring.h"
#include "any.h"
#include "type.h"

class Object;

/* \breif Internal struct to store property meta information.
*/
struct PropertyTable
{
	ConstString name;
	const TypeTable *type;
	ReadMem reader;
	WriteMem writer;
};

/*! \breif The Method class provides meta information for property.
*/
class UMOF_EXPORT Property
{
public:
	/*! \breif Constructs a Property with the given table.
		Property constructor should never be used directly.
		Please use PROPERTY() macros instead.
	*/
	Property(const PropertyTable *table);

	/*! Checks whether Property is valid object.
		Call to invalid property will result in application crash.
	*/
	bool valid() const;

	/*! Returns the name of the property.
		\sa type()
	*/
	ConstString name() const;

	/*! Returns the type of the property.
		\sa name()
	*/
	Type type() const;

	/*! Return the value of the property.
	*/
	Any read(Object *obj) const;

	/*! Sets the given value to the property.
	*/
	void write(Object *obj, const Any& value) const;

private:
	const PropertyTable *_table;

};

#endif