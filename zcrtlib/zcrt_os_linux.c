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


#ifdef LINUX_OS
#include <pthread.h>
#include <semaphore.h> 
#include <signal.h>
#include <time.h>
#include <dlfcn.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <sched.h>

#include "zcrt/ztype.h"
#include "zcrt/zcrt_os.h"
#include "zcrt/zmacro.h"

#ifndef PTHREAD_MUTEX_RECURSIVE
#define PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
#endif

static uint32_t s_tag=ZCRT_MAKE_FOURCC('Z','O','S',' ');
static pthread_mutexattr_t s_mutex_attr;
static bool_t s_mutex_inited=false;
int32_t zcrt_atom_inc(int32_t * cntPtr)
{
	/*return *(int32_t*)atomic_inc ((atomic_t *) cntPtr);*/
	return 0;
}

int32_t zcrt_atom_dec(int32_t * cntPtr)
{
	/*return *(int32_t*)atomic_dec ((atomic_t *) cntPtr);*/
	return 0;
}

void* zcrt_malloc(uint32_t size, uint32_t tag)
{
	void *p = malloc (size);
	ARG_UNUSED(tag);
	if (p)
	{
		memset (p, 0, size);
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
	sem_t* sem=zcrt_malloc(sizeof(sem_t), s_tag);
	memset(sem, 0, sizeof(sem_t));
	ARG_UNUSED(name);
	ARG_UNUSED(initial);
	if (sem_init(sem, 0, initial) !=0 )
	{
		zcrt_free(sem);
		return NULL;
	}
	
	return (ZHANDLE_t)sem;
}
void zcrt_sem_delete( ZHANDLE_t h )
{
	if (h)
	{
		 sem_destroy((sem_t *)h);
		 zcrt_free(h);
	}
}
#if defined(MACOSX)
typedef struct
{
    pthread_mutex_t count_lock;
    pthread_cond_t  count_bump;
    unsigned count;
}
bosal_sem_t;

int sem_timedwait(sem_t *psem, const struct timespec *abstim)
{
    bosal_sem_t *pxsem;
    int result, xresult;

    if (! psem)
    {
        return EINVAL;
    }
    pxsem = (bosal_sem_t *)*psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count == 0)
    {
        xresult = pthread_cond_timedwait(&pxsem->count_bump, &pxsem->count_lock, abstim);
    }
    if (! xresult)
    {
        if (pxsem->count > 0)
        {
            pxsem->count--;
        }
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}
#endif

EZCRTErr zcrt_sem_take( ZHANDLE_t h,uint32_t timeout )
{
	sem_t *sem = (sem_t *)h;
	if (timeout!=ZCRT_WAIT_FOREVER)
	{
		struct timespec ts;
		ts.tv_sec=time(NULL)+timeout;
		if (sem_timedwait(sem,&ts) == -1)
		{
			return EZCRTErr_timeout;
		}
		return 0;
	}
	sem_wait(sem);
	return 0;
}
void zcrt_sem_give( ZHANDLE_t h )
{
	sem_post((sem_t *)h);
}

ZHANDLE_t zcrt_mutex_create( char * name )
{
	pthread_mutex_t* m=zcrt_malloc(sizeof(pthread_mutex_t), s_tag);
	memset(m, 0, sizeof(pthread_mutex_t));
	ARG_UNUSED(name);
	if (!s_mutex_inited)
	{
		pthread_mutexattr_init(&s_mutex_attr);
		pthread_mutexattr_settype(&s_mutex_attr, PTHREAD_MUTEX_RECURSIVE);
		s_mutex_inited = true;
	}
	pthread_mutex_init(m, &s_mutex_attr);
	return (ZHANDLE_t)m;
}

void zcrt_mutex_delete( ZHANDLE_t h )
{
	if (h!=NULL)
	{
		pthread_mutex_destroy((pthread_mutex_t*)h);
		zcrt_free(h);
	}
}

void zcrt_mutex_lock( ZHANDLE_t h )
{
	pthread_mutex_lock((pthread_mutex_t*)h);
}

void zcrt_mutex_unlock( ZHANDLE_t h )
{
	pthread_mutex_unlock((pthread_mutex_t*)h);
}
EZCRTErr zcrt_mutex_trylock(ZHANDLE_t h)
{
	if (pthread_mutex_trylock((pthread_mutex_t *)h) ==0)
		return EZCRTErr_success;
	return EZCRTErr_timeout;
}
ZHANDLE_t zcrt_thread_create(char *name, int32_t ss, int32_t prio, int (f)(void *), void *arg)
{
	pthread_t pid;
	int errPri;
	pthread_t mythread;
	pthread_attr_t threadattr;
	struct sched_param schtParam;

	ARG_UNUSED(name);
	if (f == NULL)
	{
		return NULL;
	}

	//pthread_create( &mythread, NULL, pfnthread, arg);
	pthread_attr_init(&threadattr);

	/* if not default to zero, set the thread's stack size */
	if (ss != 0 && pthread_attr_setstacksize(&threadattr, ss))
	{
		return NULL;
	}

	/* shcedule policy must set to real-time - SCHED_RR. */
	/*it must be run with the root right*/
/*
#ifndef HW_SIMU
	pthread_attr_setschedpolicy(&threadattr,SCHED_RR);
#endif
*/
	/* set the thread's priority */
	/* for it is not clear hwo to map between the gsf priority 
	and posix thread priority, so this must be done later.
	*/
	pthread_attr_getschedparam(&threadattr, &schtParam);
	schtParam.sched_priority=prio;
	pthread_attr_setschedparam(&threadattr, &schtParam);

	if ( pthread_create( &mythread, &threadattr, f, arg)!=0 )
	{
		return NULL;
	}

	return (ZHANDLE_t)mythread;
}

void zcrt_thread_stop( ZHANDLE_t h )
{
	if (h)
	{
		pthread_cancel((pthread_t)h);
	}
}

bool_t zcrt_thread_should_stop( ZHANDLE_t h )
{
	ARG_UNUSED(h);
	return false;
}

void zcrt_sleep( int32_t msec )
{
	if (msec==0)
		sched_yield();
	else
	{
		struct timeval tv;

		tv.tv_sec  = msec / 1000;
		tv.tv_usec = (msec % 1000) * 1000;

		select(0, 0, 0, 0, &tv);
	}
}

uint32_t zcrt_get_time_freq()
{
	return 1000000;
}
void zcrt_get_time64(int64_t* ts)
{
	struct timeval _ts;
	gettimeofday(&_ts, NULL);		

	*ts = _ts.tv_sec*1000000+_ts.tv_usec;
}

#endif
