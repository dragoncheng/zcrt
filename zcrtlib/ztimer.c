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


#include "zcrt/zcrt.h"
#include <stdarg.h>
#include "zcmm.h"

#define ZTIMER_CONTAINER_MAX 1000

#define ZTIMER_IS_VALID_CONTAINER_ID(cid) \
	(((cid)>=0 && (cid)<ZTIMER_CONTAINER_MAX \
		&& (cid)!=ZTIMER_INVALID_CONTAINER) \
	|| (cid)==ZTIMER_CONTAINER_CONSTID )
/*
定时器管理可以优化
可以使用多个链表，将即将到期的定时器（比如说<500ms)放到每次轮询链表中
将较大链表放到另一个链表中，同时在一个较大周期中轮询
*/
typedef struct tagZTimerCB
{
	uint32_t left;
	ZCRT_CB fn;
	uint32_t argn;
	uint16_t timecon;
	void* args[ZCRT_CB_MAX_ARGS];
	struct tagZTimerCB* next;
}_zstTimerCB,*_lpzstTimerCB;

typedef struct tagZTimer
{
	ZHANDLE_t thr;
	ZHANDLE_t mutex;
	_lpzstTimerCB job;
	_lpzstTimerCB tail;
	_lpzstTimerCB freelist;
	int32_t freesize;
	uint16_t conlastid;
	uint8_t container[ZTIMER_CONTAINER_MAX];
	bool_t run;
}_zstTimer,*_lpzstTimer;

static uint32_t s_timertag=ZCRT_MAKE_FOURCC('Z','T','M','R');

static _lpzstTimerCB _zcrt_timer_malloc_cb( _lpzstTimer mng, uint32_t period, ZCRT_CB fn)
{
	_lpzstTimerCB job = NULL;

	zcrt_mutex_lock(mng->mutex);
	if (mng->freelist)
	{
		job=mng->freelist;
		mng->freelist=job->next;
		mng->freesize--;
		zcrt_mutex_unlock(mng->mutex);
	}
	else
	{
		zcrt_mutex_unlock(mng->mutex);
		job=zcrt_malloc(sizeof(_zstTimerCB), s_timertag);
		if (job==NULL)
		{
			return NULL;
		}
	}
	memset(job, 0, sizeof(_zstTimerCB));
	job->left = period;
	job->fn = fn;
	return job;
}

static void _zcrt_timer_free_cb( _lpzstTimer mng, _lpzstTimerCB job)
{
	zcrt_mutex_lock(mng->mutex);
	job->next = mng->freelist;
	mng->freelist = job;
	job->fn=NULL;
	job->timecon = ZTIMER_INVALID_CONTAINER;
	mng->freesize++;
	zcrt_mutex_unlock(mng->mutex);
}

static void _zcrt_t_free_jobs(_lpzstTimerCB jobs)
{
	_lpzstTimerCB n=NULL;
	while (jobs)
	{
		n=jobs->next;
		zcrt_free(jobs);
		jobs=n;
	}
}

#define TIMER_TICK 50
static int _zcrt_timer_proc(void* arg)
{
	_lpzstTimer mng=(_lpzstTimer)arg;
	_lpzstTimerCB job=NULL;
	_lpzstTimerCB prev=NULL;

	while (mng->run && !zcrt_thread_should_stop(mng->thr))
	{
		job=mng->job;
		prev=job;
		while(job)
		{
			if (job->left<TIMER_TICK)
			{
				zcrt_mutex_lock(mng->mutex);
				if (mng->job == job)
				{
					mng->job = job->next;
					prev = mng->job;
				}
				else
				{
					prev->next = job->next;
				}
				if (mng->tail==job)
				{
					ZCRT_ASSERT(job->next == NULL);
					mng->tail = prev;
				}
				zcrt_mutex_unlock(mng->mutex);
				ZCRT_ASSERT(job->fn);
				/*zcrt_cmm_call_cb(job->fn, job->argn, job->args);*/
				if (ZTIMER_IS_VALID_CONTAINER_ID(job->timecon)
					&& (job->timecon == ZTIMER_CONTAINER_CONSTID
						|| mng->container[job->timecon] != ZTIMER_INVALID_CONTAINER) )
				{
					job->fn(job->args[0],job->args[1],job->args[2],job->args[3],job->args[4],job->args[5]);
				}
				_zcrt_timer_free_cb(mng, job);
				job = mng->job;
			}
			else
			{
				prev = job;
				job->left -= TIMER_TICK;
				job = job->next;
			}
		}
		zcrt_sleep(TIMER_TICK);
	}
	zcrt_mutex_delete(mng->mutex);
	_zcrt_t_free_jobs(mng->job);
	_zcrt_t_free_jobs(mng->freelist);
	zcrt_free(mng);
	return 0;
}

ZHANDLE_t zcrt_timer_manage_init(void)
{
	_lpzstTimer mng=zcrt_malloc(sizeof(_zstTimer), s_timertag);
	if (mng==NULL)
	{
		return NULL;
	}
	memset(mng,0,sizeof(_zstTimer));

	mng->mutex = zcrt_mutex_create("ZTimer");
	mng->run = true;
	mng->thr = zcrt_thread_create("ZTimer", 0, ZCRT_THREAD_PRI_NOMAL, _zcrt_timer_proc, mng);
	return (ZHANDLE_t)mng;
}

void zcrt_timer_manage_delete(ZHANDLE_t h)
{
	if (h!=NULL)
	{
		_lpzstTimer mng=(_lpzstTimer)h;
		mng->run = false;
	}
}

void _zcrt_timer_insert_cb_has_mutex( _lpzstTimer mng, _lpzstTimerCB job )
{
//	zcrt_mutex_lock(mng->mutex);
	if (mng->job)
	{
		ZCRT_ASSERT(mng->tail);
		mng->tail->next=job;
		mng->tail=job;
	}
	else
	{
		ZCRT_ASSERT(mng->tail==NULL);
		mng->job  = job;
		mng->tail = job;
	}
//	zcrt_mutex_unlock(mng->mutex);
}
uint16_t zcrt_timer_container_new( ZModule_t module)
{
	_lpzstTimer mng = NULL;
	uint16_t i = 0;
	uint16_t idx = 0;
	uint16_t cid = ZTIMER_INVALID_CONTAINER;
	if (module==NULL || module->timer_mng==NULL)
	{
		return ZTIMER_INVALID_CONTAINER;
	}
	mng = module->timer_mng;
	zcrt_mutex_lock(mng->mutex);
	for(i=mng->conlastid; i<mng->conlastid+ZTIMER_CONTAINER_MAX; i++)
	{
		idx = (i%ZTIMER_CONTAINER_MAX);
		if (idx != ZTIMER_INVALID_CONTAINER
			&& mng->container[idx] == 0)
		{
			mng->container[idx] = 1;
			mng->conlastid = i;
			cid = idx;
			break;
		}
	}
	zcrt_mutex_unlock(mng->mutex);
	return cid;
}

void zcrt_timer_container_delete( ZModule_t module, uint16_t container_id)
{
	_lpzstTimer mng = NULL;
	if (module==NULL
		|| module->timer_mng==NULL
		|| !ZTIMER_IS_VALID_CONTAINER_ID(container_id)
		|| container_id == ZTIMER_CONTAINER_CONSTID)
	{
		return;
	}
	mng = module->timer_mng;

	zcrt_mutex_lock(mng->mutex);
	mng->container[container_id] = 0;
	zcrt_mutex_unlock(mng->mutex);
	return;
}

EZCRTErr zcrt_timercon_add( ZModule_t module, uint16_t container_id, uint32_t period, ZCRT_CB cb, void* p1, void* p2 )
{
	_lpzstTimer mng = NULL;
	_lpzstTimerCB job=NULL;
	
	if (module==NULL
		|| module->timer_mng==NULL
		|| !ZTIMER_IS_VALID_CONTAINER_ID(container_id))
	{
		return EZCRTErr_invalidCrtModule;
	}

	if (cb==NULL) { return EZCRTErr_paramErr; }

	mng = module->timer_mng;
	zcrt_mutex_lock(mng->mutex);

	if (container_id != ZTIMER_CONTAINER_CONSTID
		&& mng->container[container_id] == ZTIMER_INVALID_CONTAINER)
	{
		zcrt_mutex_unlock(mng->mutex);
		return EZCRTErr_badObject;
	}

	job = _zcrt_timer_malloc_cb(mng, period, cb);
	if (job==NULL)
	{
		zcrt_mutex_unlock(mng->mutex);
		return EZCRTErr_noMem;
	}
	job->argn = 2;
	job->args[0] = p1;
	job->args[1] = p2;
	job->timecon = container_id;
	_zcrt_timer_insert_cb_has_mutex(mng, job);

	zcrt_mutex_unlock(mng->mutex);
	return 0;
}

EZCRTErr zcrt_timercon_add_argn( ZModule_t module, uint16_t container_id,
		uint32_t period, ZCRT_CB cb, uint32_t argn, ... )
{
	_lpzstTimer mng = NULL;
	_lpzstTimerCB job=NULL;
	va_list va;
	uint32_t i=0;

	if (module==NULL
		|| module->timer_mng==NULL
		|| !ZTIMER_IS_VALID_CONTAINER_ID(container_id))
	{
		return EZCRTErr_invalidCrtModule;
	}

	if (cb==NULL || argn==0 || argn>ZCRT_CB_MAX_ARGS) { return EZCRTErr_paramErr; }

	mng = module->timer_mng;
	zcrt_mutex_lock(mng->mutex);
	if (container_id != ZTIMER_CONTAINER_CONSTID
		&& mng->container[container_id] == ZTIMER_INVALID_CONTAINER)
	{
		zcrt_mutex_unlock(mng->mutex);
		return EZCRTErr_badObject;
	}

	job = _zcrt_timer_malloc_cb(mng, period, cb);
	if (job==NULL)
	{
		zcrt_mutex_unlock(mng->mutex);
		return EZCRTErr_noMem;
	}

	job->argn = argn;
	va_start (va, argn);
	for (i = 0; i < argn; i++)
	{
		job->args[i] = va_arg (va, void *);
	}
	va_end (va);
	_zcrt_timer_insert_cb_has_mutex(mng, job);

	zcrt_mutex_unlock(mng->mutex);
	return 0;
}

#if 0
EZCRTErr zcrt_timer_add( ZModule_t module, uint32_t period, ZCRT_CB cb, void* p1, void* p2 )
{
	return zcrt_timercon_add( module, ZTIMER_CONTAINER_CONSTID, period, cb, p1, p2 )
}
#endif

EZCRTErr zcrt_timer_delete( ZModule_t module, ZCRT_CB cb, void* p1, void* p2 )
{
	_lpzstTimer mng = NULL;
	_lpzstTimerCB job=NULL;

	if (module==NULL || module->timer_mng==NULL)
	{
		return EZCRTErr_invalidCrtModule;
	}

	if (cb==NULL) { return EZCRTErr_paramErr; }

	mng = module->timer_mng;

	zcrt_mutex_lock(mng->mutex);
	job = mng->job;
	if (job)
	{
		if (job->fn==cb && job->args[0]==p1 && job->args[1]==p2)
		{
			mng->job = job->next;
			if (job->next==NULL)
			{
				mng->tail = NULL;
			}
		}
		else
		{
			_lpzstTimerCB prev=job;
			while (job!=NULL && (job->fn!=cb || job->args[0]!=p1 || job->args[1]!=p2))
			{
				prev = job;
				job = job->next;
			}
			if (job)
			{
				prev->next = job->next;
			}
			if (mng->tail==job)
			{
				ZCRT_ASSERT(job->next == NULL);
				mng->tail = prev;
			}
		}
	}
	zcrt_mutex_unlock(mng->mutex);
	if (job != NULL)
	{
		_zcrt_timer_free_cb(mng, job);
	}
	return 0;

}
