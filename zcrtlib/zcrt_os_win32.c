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


#ifdef WIN32
#include "zcrt/ztype.h"
#include "zcrt/zcrt_os.h"
#include "zcrt/zmacro.h"

#include <stdarg.h>
#include <windows.h>
#include <time.h>
#include <process.h>

int32_t zcrt_atom_inc(int32_t * cntPtr)
{
	return InterlockedIncrement((LONG*)cntPtr);
}

int32_t zcrt_atom_dec(int32_t * cntPtr)
{
	return InterlockedDecrement((LONG*)cntPtr);
}

void* zcrt_malloc(uint32_t size, uint32_t tag)
{
	void *p = malloc (size);
	ARG_UNUSED(tag);
	if (p)
	{
		/*memset (p, 0, size);*/
	}
	return p;
}
void zcrt_free(void* p)
{
	free(p);
}
void* zcrt_realloc(void* p, uint32_t size, uint32_t tag)
{
	ARG_UNUSED(tag);
	return realloc (p,size);
}

ZHANDLE_t zcrt_sem_create( char * name, uint16_t initial )
{
	ARG_UNUSED(name);
	ARG_UNUSED(initial);
	return CreateEvent(NULL,FALSE,initial>0,NULL);
}
void zcrt_sem_delete( ZHANDLE_t h )
{
	CloseHandle(h);
}
EZCRTErr zcrt_sem_take( ZHANDLE_t h,uint32_t timeout )
{
	DWORD dw=0;
	if (timeout==ZCRT_WAIT_FOREVER)
	{
		dw = WaitForSingleObject(h,INFINITE) == WAIT_OBJECT_0;
	}
	else
	{
		dw = WaitForSingleObject(h,timeout) == WAIT_OBJECT_0;
	}
	switch (dw) {
		case WAIT_OBJECT_0:
			return 0;
		case WAIT_TIMEOUT:
			return EZCRTErr_timeout;
		default:
			return EZCRTErr_badObject;
	}
}
void zcrt_sem_give( ZHANDLE_t h )
{
	SetEvent(h);
}

ZHANDLE_t zcrt_mutex_create( char * name )
{
	CRITICAL_SECTION* m=zcrt_malloc(sizeof(CRITICAL_SECTION),0);
	ARG_UNUSED(name);
	InitializeCriticalSection(m);
	return m;
}

void zcrt_mutex_delete( ZHANDLE_t h )
{
	if (h!=NULL)
	{
		DeleteCriticalSection(h);
		zcrt_free(h);
	}
}

void zcrt_mutex_lock( ZHANDLE_t h )
{
	EnterCriticalSection(h);
}

void zcrt_mutex_unlock( ZHANDLE_t h )
{
	LeaveCriticalSection(h);
}

EZCRTErr zcrt_mutex_trylock(ZHANDLE_t h)
{
	if (TryEnterCriticalSection(h))
		return EZCRTErr_success;
	return EZCRTErr_timeout;
}

ZHANDLE_t zcrt_thread_create(char *name, int32_t ss, int32_t prio, int (f)(void *), void *arg)
{
	ARG_UNUSED(name);
	ARG_UNUSED(prio);
	ARG_UNUSED(ss);
	return (ZHANDLE_t)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))f, arg, 0, NULL);
}

void zcrt_thread_stop( ZHANDLE_t h )
{
	ARG_UNUSED(h);
}

bool_t zcrt_thread_should_stop( ZHANDLE_t h )
{
	ARG_UNUSED(h);
	return false;
}

void zcrt_sleep( int32_t msec )
{
	Sleep(msec);
}

static uint32_t s_time_freq = 0xffffffff;
uint32_t zcrt_get_time_freq()
{
	if (s_time_freq==0xffffffff)
	{
		LARGE_INTEGER val;
		QueryPerformanceFrequency(&val);
		s_time_freq = val.u.LowPart;
	}
	return s_time_freq;
}

void zcrt_get_time64(int64_t* ts)
{
	QueryPerformanceCounter((LARGE_INTEGER*)ts);
}


#endif
