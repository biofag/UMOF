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

#ifndef CONSTSTRING_H
#define CONSTSTRING_H

#include <cstddef>
#include <stdexcept>
#include <cstring>

/*! Compile-time string with known size.
*/
class ConstString
{
public:
	//Workaround for GCC bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=59879
	constexpr ConstString(const char *a) :
		_string(a), _size(std::strlen(a)) {}
		
	template<std::size_t N>
	constexpr ConstString(const char(&a)[N]) :
		_string(a), _size(N - 1) {}

	constexpr char operator[](std::size_t n) {
		return n < _size ? _string[n] :
			throw std::out_of_range("");
	}

	constexpr operator const char *() const { return _string; }
	constexpr std::size_t size() const { return _size; }

private:
	const char* const _string;
	const std::size_t _size;
};

#endif