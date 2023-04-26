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


#ifdef __KERNEL__
#include <linux/kthread.h>
#include <linux/module.h>
#include <asm/atomic.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/semaphore.h>
#include <linux/sched.h>
//#include <asm/system.h>

#include "zcrt/ztype.h"
#include "zcrt/zcrt_os.h"
#include "zcrt/zmacro.h"

static uint32_t s_tag=ZCRT_MAKE_FOURCC('Z','O','S',' ');
static unsigned long s_irqflags=0;

typedef struct tagZcrtMutex
{
	struct semaphore sem;
    uint32_t owner;
    int			recurse_count;
}ZCRTMutex,*ZCRTMutex_t;

int32_t zcrt_atom_inc(int32_t * cntPtr)
{
	atomic_inc((atomic_t *)cntPtr);
	return *cntPtr;
}

int32_t zcrt_atom_dec(int32_t * cntPtr)
{
	atomic_dec((atomic_t *)cntPtr);
	return *cntPtr;
}

void zcrt_irq_lock( void )
{
	 local_irq_save( s_irqflags );
}

void zcrt_irq_unlock( void )
{
	local_irq_restore( s_irqflags );
}

ZHANDLE_t zcrt_sem_create( char * name, uint16_t initial )
{
	struct semaphore* sem=zcrt_malloc(sizeof(struct semaphore), s_tag);
	memset(sem, 0, sizeof(struct semaphore));
	ARG_UNUSED(name);
	sema_init(sem, initial);
	
	return (ZHANDLE_t)sem;
}
void zcrt_sem_delete( ZHANDLE_t h )
{
	if (h)
	{
		 zcrt_free(h);
	}
}
EZCRTErr zcrt_sem_take( ZHANDLE_t h,uint32_t timeout )
{
	struct semaphore *sem = (struct semaphore *)h;
	ZCRT_ASSERT( timeout!=ZCRT_WAIT_FOREVER );
	if(down_timeout(sem,timeout) == -EINTR)
	{
		return EZCRTErr_timeout;
	}
	return 0;
}
void zcrt_sem_give( ZHANDLE_t h )
{
	up((struct semaphore *)h);
}

ZHANDLE_t zcrt_mutex_create( char * name )
{
	ZCRTMutex* sem=zcrt_malloc(sizeof(ZCRTMutex), s_tag);
	memset(sem, 0, sizeof(ZCRTMutex));
	sem->owner = 0;
	sem->recurse_count = 0;
	sema_init(&sem->sem, 1);

	ARG_UNUSED(name);

	return (ZHANDLE_t)sem;
}

void zcrt_mutex_delete( ZHANDLE_t h )
{
	if (h!=NULL)
	{
		zcrt_free(h);
	}
}

void zcrt_mutex_lock( ZHANDLE_t h )
{
	ZCRTMutex_t	sem = (ZCRTMutex_t) h;
    uint32_t selfid = current->pid;

    if (sem->owner == selfid) {
		sem->recurse_count++;
		return;
    }

    sem->owner = selfid;
    down(&sem->sem);
}

void zcrt_mutex_unlock( ZHANDLE_t h )
{
	ZCRTMutex_t	sem = (ZCRTMutex_t) h;

    if (sem->recurse_count > 0) {
		sem->recurse_count--;
		return;
    }

    sem->owner = 0;

    up(&sem->sem);
}

EZCRTErr zcrt_mutex_trylock(ZHANDLE_t h)
{
	if (down_trylock(&((ZCRTMutex_t)h)->sem) ==0)
		return EZCRTErr_success;
	return EZCRTErr_timeout;
}

ZHANDLE_t zcrt_thread_create(char *name, int32_t ss, int32_t prio, int (f)(void *), void *arg)
{
	return (ZHANDLE_t)kthread_run(f,arg,name);
}


void zcrt_thread_stop( ZHANDLE_t h )
{
	if (h)
	{
		kthread_stop(( struct task_struct *)h);
	}
}

bool_t zcrt_thread_should_stop( ZHANDLE_t h )
{
	ARG_UNUSED(h);
	return kthread_should_stop();
}
#ifndef SLEEP_MILLI_SEC
#define SLEEP_MILLI_SEC(nMilliSec)\
        do { \
                long timeout = (nMilliSec) * HZ / 1000; \
                while(timeout > 0) \
                { \
                timeout = schedule_timeout(timeout); \
                } \
        }while(0);
#endif
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/delay.h>
void zcrt_sleep( int32_t msec )
{
	msleep(msec);
/*	SLEEP_MILLI_SEC(msec);*/
}

#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/jiffies.h> 

uint32_t zcrt_get_time_freq(void)
{
	return 1000000;
}
void zcrt_get_time64(int64_t* ts)
{
	struct timex  txc;
	do_gettimeofday(&(txc.time));
	*ts = txc.time.tv_sec*1000000+txc.time.tv_usec;
}

uint32_t zcrt_current_time(void)
{
	struct timex  txc;
	do_gettimeofday(&(txc.time));
	return (uint32_t)txc.time.tv_sec; 
}
uint32_t zcrt_cpu_clock(void)
{
	return jiffies;
}
#endif
