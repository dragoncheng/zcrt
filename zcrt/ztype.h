/****************************************************************************
COPYRIGHT NOTICE  
Copyright (c) 2012 - 2014  Alex Cheng
All rights reserved.  
Author: Alex Cheng

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/


#ifndef __ptcrt_type_h__
#define __ptcrt_type_h__

#ifndef true
#define true  1
#define false 0
#endif

typedef  char bool_t;

#ifdef WIN32
	typedef unsigned char  uint8_t;
	typedef unsigned short uint16_t;
	typedef unsigned  uint32_t;
	typedef long long uint64_t;

	typedef signed char  int8_t;
	typedef short int16_t;
	typedef int   int32_t;
	typedef long long int64_t;
	#define INLINE		/*VS2008 不支持C的inline关键字*/
#else
#ifndef __KERNEL__
#include <stdint.h>
#else
#include <linux/types.h>
#endif
#if 0
	typedef unsigned char  uint8_t;
	typedef unsigned short uint16_t;
	typedef unsigned long  uint32_t;
	typedef long long uint64_t;

	typedef signed char  int8_t;
	typedef short int16_t;
	typedef long  int32_t;
	typedef long long int64_t;
#endif

	#define INLINE inline
#endif

typedef struct tagZStr_t
{
	uint32_t len;
	char* ptr;
}zstr_t;

typedef void* ZHANDLE_t;

/** callback definine  */
typedef void(*ZCRT_CB)(void* p1, void* p2, void*, void*, void*, void*);

/** crt module definine  */
typedef struct tagZCRTModule 
{
	ZHANDLE_t schthr_list;
	ZHANDLE_t timer_mng;
	ZHANDLE_t evt_mng;
	ZHANDLE_t crt_mutex;
	ZHANDLE_t zcrtthr;
	ZHANDLE_t zipc;
}ZCRTModule, *ZModule_t;

#endif
