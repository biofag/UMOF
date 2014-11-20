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

#ifndef API_H
#define API_H

#include "defines.h"
#include "conststring.h"
#include "method.h"
#include "property.h"

class Api;

/*! \breif Internal struct to store class meta information.
*/
struct ApiTable
{
	ConstString name;
	const Api *super;
	const MethodTable *methods;
	const PropertyTable *props;
	int methodCount;
	int propCount;
};

/*! \breif The Api class contains meta information about objects.
	An Api instance is created for every object with OBJECT macro
	defined inside its declaration. The api is available as
	Object::api() or Object::classApi().
	This class is used for type introspection. It is useful for
	scripting.
	\sa Method, Property, Enumerator
*/
class UMOF_EXPORT Api
{
public:
	/*! \breif Constructs an Api with the given table.
		Api constructor should never be used directly. 
		Please use OBJECT() macro instead.
	*/
	Api(const ApiTable *table);

	/*! Returns the class name
		\sa super()
	*/
	ConstString name() const;

	/*! Returns the Api of the superclass, 
		or nullptr if there is no such object.
		\warning Only single inheritance is supported in current release.
		\sa name()
	*/
	const Api *super() const;

	//Any data(const char *) const; //Additional data connected to class
	//int indexOfConstructor(const char *) const; //Find index of constructor
	//Method constructor(const char *) const; //Find constructor by index

	/*! Finds the method with given signature
		and returns its index, otherwise returns -1.
		Signature should be normalized.
		\sa indexOfProperty()
	*/
	int indexOfMethod(const char *) const;
	
	/*! Returns the meta information for the method with the given index.
		\sa property()
	*/
	Method method(int index) const;

	/*! Returns the number of methods in class api,
		including number of methods in each base class.
		\sa propertyCount()
	*/
	int methodCount() const;

	/*! Returns the index of class api's first method.
		\sa propertyOffset()
	*/
	int methodOffset() const;

	/*! Finds the property with given name
		and returns its index, otherwise returns -1.
		\sa indexOfMethod()
	*/
	int indexOfProperty(const char *name) const;

	/*! Returns the meta information for the property with the given index.
		\sa method()
	*/
	Property property(int) const;

	/*! Returns the number of properties in class api,
		including number of properties in each base class.
		\sa methodCount()
	*/
	int propertyCount() const;

	/*! Returns the index of class api's first property.
		\sa methodOffset()
	*/
	int propertyOffset() const;

	//int indexOfEnumerator(const char *name) const; //find enum by name
	//Enumerator enumerator(int index) const; //returns enum
	//int enumeratorCount() const; //number of enums
	//int enumeratorOffset() const; //enums offset
	
	//static Object *create(ArgPack args) const; //create new instance of the class

	/*! Invokes the method with the given name and args.
	*/
	static Any invoke(Object *obj, const char *name, std::initializer_list<Any> args);

private:
	const ApiTable *_table;

};

#endif