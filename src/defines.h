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

#ifndef DEFINES_H
#define DEFINES_H

#if defined(_WIN32) || defined(__CYGWIN__)
#if defined UMOF_LIBRARY
#if defined(__GNUC__)
#define UMOF_EXPORT __attribute__ ((dllexport))
#elif defined(_MSC_VER)
#define UMOF_EXPORT __declspec(dllexport)
#else
#define UMOF_EXPORT
#endif
#else
#if defined(__GNUC__)
#define UMOF_EXPORT __attribute__ ((dllimport))
#elif defined(_MSC_VER)
#define UMOF_EXPORT __declspec(dllimport)
#else
#define  UMOF_EXPORT
#endif
#endif
#else
#if __GNUC__ >= 4
#define UMOF_EXPORT __attribute__ ((visibility ("default")))
#else
#define UMOF_EXPORT
#endif
#endif

class Any;
class Object;

typedef Any(*InvokeMem)(Object *, int argc, const Any *);
typedef Any(*ReadMem)(Object *);
typedef void(*WriteMem)(Object *, const Any&);

#define VA_NARGS_IMPL_(_1, _2, _3, _4, _5, _6, _7, _8, \
						_9, _10, _11, _12, _13, _14, _15, _16, \
						_17, _18, _19, _20, _21, _22, _23, _24, \
						_25, _26, _27, _28, _29, _30, _31, _32, \
					N, ...) N
#define VA_NARGS_IMPL(tuple) VA_NARGS_IMPL_ tuple
#define VA_NARGS(...) VA_NARGS_IMPL((__VA_ARGS__,  \
						32, 31, 30, 29, 28, 27, 26, 25, \
						24, 23, 22, 21, 20, 19, 18, 17, \
						16, 15, 14, 13, 12, 11, 10, 9, \
						8, 7, 6, 5, 4, 3, 2, 1))

#endif