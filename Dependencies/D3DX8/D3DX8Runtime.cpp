/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2026 TheSuperHackers
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// VC6 runtime helpers referenced by the VC6 compiled d3dx8.lib that MinGW-w64 does not provide.

#include <stdlib.h>

typedef void (__attribute__((thiscall)) *XtorFunc)(void *object);

// operator new(unsigned int)
void *msvc_new(size_t size) __asm__("\"??2@YAPAXI@Z\"");
void *msvc_new(size_t size)
{
	return malloc(size);
}

// operator delete(void *)
void msvc_delete(void *ptr) __asm__("\"??3@YAXPAX@Z\"");
void msvc_delete(void *ptr)
{
	free(ptr);
}

// `eh vector constructor iterator'
// Unlike the VC6 one, it does not destroy the constructed elements when a constructor throws.
void __stdcall msvc_vector_constructor(void *ptr, size_t size, int count, XtorFunc ctor, XtorFunc dtor) __asm__("\"??_L@YGXPAXIHP6EX0@Z1@Z\"");
void __stdcall msvc_vector_constructor(void *ptr, size_t size, int count, XtorFunc ctor, XtorFunc)
{
	for (int i = 0; i < count; ++i)
		ctor(static_cast<char *>(ptr) + i * size);
}

// `eh vector destructor iterator'
void __stdcall msvc_vector_destructor(void *ptr, size_t size, int count, XtorFunc dtor) __asm__("\"??_M@YGXPAXIHP6EX0@Z@Z\"");
void __stdcall msvc_vector_destructor(void *ptr, size_t size, int count, XtorFunc dtor)
{
	for (int i = count - 1; i >= 0; --i)
		dtor(static_cast<char *>(ptr) + i * size);
}

__asm__(
	// Offset of the SEH frame list in the thread information block, used as fs:_except_list
	".globl __except_list\n"
	".set __except_list, 0\n"

	// Same contract as __chkstk: probe and allocate eax bytes of stack
	".globl __alloca_probe\n"
	"__alloca_probe:\n"
	"	jmp ___chkstk\n"

	// Unsigned 64-bit shift right of edx:eax by cl
	".globl __aullshr\n"
	"__aullshr:\n"
	"	cmpb $64, %cl\n"
	"	jae 2f\n"
	"	cmpb $32, %cl\n"
	"	jae 1f\n"
	"	shrdl %cl, %edx, %eax\n"
	"	shrl %cl, %edx\n"
	"	ret\n"
	"1:	movl %edx, %eax\n"
	"	xorl %edx, %edx\n"
	"	shrl %cl, %eax\n"
	"	ret\n"
	"2:	xorl %eax, %eax\n"
	"	xorl %edx, %edx\n"
	"	ret\n"
);
