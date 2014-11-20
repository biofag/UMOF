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

#ifndef OBJECT_H
#define OBJECT_H

#include "objectdefs.h"
#include "api.h"

#define OBJECT_CHECK(Class) \
	static_assert(std::is_base_of<Object, Class>::value, "Class " #Class " should inherit from Object");

/*! This macro makes meta information available for object.
	\relates Object
*/
#define OBJECT(Class, Super) \
public: \
	static const Api *classApi() \
	{ \
		OBJECT_CHECK(Class) \
		static const ApiTable apiTable \
		{ \
			#Class, \
			Super::classApi(), \
			expose_method<Class>::exec().second, \
			expose_props_method<Class>::exec().second, \
			expose_method<Class>::exec().first, \
			expose_props_method<Class>::exec().first \
		}; \
		static const Api staticApi(&apiTable); \
		return &staticApi; \
	} \
	virtual const Api *api() const \
	{ \
		return Class::classApi(); \
	} \
private:

/*! This macro exposes class methods in Api.
	\relates Object
*/
#define EXPOSE(...) \
public: \
	static const std::pair<int, const MethodTable *> expose() \
	{ \
		static const MethodTable methods[] \
		{ \
			__VA_ARGS__ \
		}; \
		return { sizeof(methods) / sizeof(MethodTable *), methods }; \
	} \
private:

/*! This macro exposes class properties in Api.
	\relates Object
*/
#define PROPERTIES(...) \
public: \
	static const std::pair<int, const PropertyTable *> expose_props() \
	{ \
		static const PropertyTable props[] \
		{ \
			__VA_ARGS__ \
		}; \
		return { sizeof(props) / sizeof(PropertyTable *), props }; \
	} \
private:

/*! \breif The Object class is the base class of all objects.
*/
class UMOF_EXPORT Object
{
public:
	/*! Constructs an object.
	*/
	Object();

	/*! Destroys an object.
	*/
	virtual ~Object();

	/*! Return the Api of the class.
	*/
	static const Api *classApi();

	/*! Returns the Api of this object.
	*/
	virtual const Api *api() const;
};

#endif