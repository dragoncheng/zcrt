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
#include "zcmm.h"
#include <stdarg.h>

struct tagZSchedule;
struct tagZSchThr;
typedef struct tagZScheduleJob
{
	ZCRT_CB fn;
	uint32_t argn;
	void* args[ZCRT_CB_MAX_ARGS];
	struct tagZSchedule* sch;
	struct tagZScheduleJob* next;
}_zstScheduleJob,*_lpzstScheduleJob;

#define SCH_NAME_LEN 20
typedef struct tagZSchPriList
{
	_lpzstScheduleJob jobs;
	_lpzstScheduleJob tail;
}_zstSchPriList,*_lpzstSchPriList;

typedef struct tagZSchedule
{
	char name[SCH_NAME_LEN];
	struct tagZSchThr* thr;

	int32_t maxsize;
	EZCRTPriority pri;
	EZCRTSchType type;

	int32_t cursize;
	int32_t maxhold;
	ZModule_t module;
	struct tagZSchedule* next;
}_zstSchedule,*_lpzstSchedule;

typedef struct tagZSchThr
{
	char name[SCH_NAME_LEN];
	ZHANDLE_t thr;
	ZHANDLE_t sem;
	ZHANDLE_t mutex;

	_zstSchPriList que[EZCRTSchPri_num];

	_lpzstScheduleJob freejobs;

	bool_t run;
	int32_t refCnt;		/* 创建schedule的引用个数 */
	ZModule_t module;
	
	_lpzstSchedule sch_list;
	struct tagZSchThr* next;

	/* debugs */
	uint32_t  joballoced;
	uint32_t  jobfreed;
	uint32_t  jobadded;
}_zstSchThr,*_lpzstSchThr;

static uint32_t s_schtag=ZCRT_MAKE_FOURCC('Z','S','C','H');

/* schedule thread*/
static void _zcrt_s_thr_remove_from_module( _lpzstSchThr thr )
{
	ZModule_t m = thr->module;
	_lpzstSchThr prev = NULL;
	if (m==NULL) 
	{
		return;
	}
	if (thr==m->schthr_list)
	{
		m->schthr_list = thr->next;
	}
	else
	{
		prev = (_lpzstSchThr)m->schthr_list;
		while (prev && prev->next!=thr)
		{
			prev=prev->next;
		}
		if (prev)
		{
			prev->next = thr->next;
		}
	}
}
static _lpzstScheduleJob _zcrt_s_thr_getjob(_lpzstSchPriList schpri)
{
	_lpzstScheduleJob job=schpri->jobs;
	if (job == NULL)
	{
		return NULL;
	}
	schpri->jobs = job->next;
	if(schpri->tail == job)
	{
		schpri->tail = NULL;
	}
	return job;
}

static _lpzstScheduleJob _zcrt_s_job_new( _lpzstSchThr thr, ZCRT_CB cb )
{
	_lpzstScheduleJob job = NULL;
	ZCRT_ASSERT(thr != NULL );

	zcrt_mutex_lock(thr->mutex);
	thr->jobadded++;
	if (thr->freejobs)
	{
		job=thr->freejobs;
		thr->freejobs=job->next;
		zcrt_mutex_unlock(thr->mutex);
	}
	else
	{
		thr->joballoced++;
		zcrt_mutex_unlock(thr->mutex);
		job=zcrt_malloc(sizeof(_zstScheduleJob), s_schtag);
		if (job==NULL)
		{
			return NULL;
		}
	}
	memset(job, 0, sizeof(_zstScheduleJob));
	job->fn = cb;
	return job;
}

static void _zcrt_s_job_delete_has_mutex( _lpzstSchThr thr, _lpzstScheduleJob job)
{
	_lpzstSchedule sch=job->sch;
	/*zcrt_mutex_lock(thr->mutex);*/
	job->next = thr->freejobs;
	thr->freejobs = job;
	thr->jobfreed++;
	job->fn=NULL;
	job->sch=NULL;
	if (sch != NULL)
	{
		sch->cursize--;
	}
	/*zcrt_mutex_unlock(thr->mutex);*/
}

static void _zcrt_s_thr_delete( _lpzstSchThr thr )
{
	_lpzstScheduleJob job=thr->freejobs;
	_lpzstScheduleJob p=NULL;
	uint32_t i=0;
	while (job)
	{
		p=job->next;
		zcrt_free(job);
		job=p;
	}
	for (i=0; i<EZCRTSchPri_num; i++)
	{
		job = thr->que[i].jobs;
		while (job)
		{
			p=job->next;
			zcrt_free(job);
			job=p;
		}
	}
	zcrt_free(thr);
}
static int _zcrt_schedule_thread_proc(void* arg)
{
	_lpzstSchThr thr=(_lpzstSchThr)arg;
	_lpzstScheduleJob job=NULL;
	uint32_t i=0;
	ZCRT_CB fn=NULL;
	void* args[ZCRT_CB_MAX_ARGS];

	while (thr->run && !zcrt_thread_should_stop(thr->thr))
	{
		zcrt_sem_take(thr->sem, 100);
		do
		{
			job=NULL;
			fn = NULL;
			zcrt_mutex_lock(thr->mutex);
			for (i=0; i<EZCRTSchPri_num; i++)
			{
				if ((job=_zcrt_s_thr_getjob(&thr->que[i])) != NULL)
				{
					break;
				}
			}
			if (job)
			{
				fn=job->fn;
				memcpy(args, job->args, sizeof(void*)*ZCRT_CB_MAX_ARGS);
				_zcrt_s_job_delete_has_mutex(thr, job);
			}
			zcrt_mutex_unlock(thr->mutex);

			if (job && fn)
			{
				fn(args[0],args[1],args[2],args[3],args[4],args[5]);
			}
		}while(job!=NULL);
	}
	zcrt_sem_delete(thr->sem);
	zcrt_mutex_delete(thr->mutex);
	_zcrt_s_thr_remove_from_module(thr);
	_zcrt_s_thr_delete(thr);
	return 0;
}

static _lpzstSchThr _zcrt_s_thr_new(ZModule_t module, const char* name )
{
	_lpzstSchThr thr=zcrt_malloc(sizeof(_zstSchThr), s_schtag);
	memset(thr, 0, sizeof(_zstSchThr));
	memcpy(thr->name, name, strlen(name)>=SCH_NAME_LEN?SCH_NAME_LEN-1:strlen(name));

	thr->sem = zcrt_sem_create("ZScheduleThr", 0);
	thr->mutex = zcrt_mutex_create("ZScheduleThr");
	thr->module = module;
	thr->run = true;

	if (module)
	{
		zcrt_module_lock(module);
		thr->next = module->schthr_list;
		module->schthr_list = thr;
		zcrt_module_unlock(module);
	}
	thr->thr = zcrt_thread_create("ZScheduleThr", 0, ZCRT_THREAD_PRI_NOMAL, _zcrt_schedule_thread_proc, thr);

	return thr;
}

static void _zcrt_s_thr_add_sch(_lpzstSchThr thr, _lpzstSchedule sch)
{
	zcrt_mutex_lock(thr->mutex);
	if (thr->sch_list)
	{
		sch->next = thr->sch_list;
		thr->sch_list = sch;
	}
	thr->sch_list = sch;
	zcrt_mutex_unlock(thr->mutex);
}

static void _zcrt_s_thr_remove_sch(_lpzstSchThr thr, _lpzstSchedule sch)
{
	zcrt_mutex_lock(thr->mutex);
	/*ZCRT_ASSERT(sch->cursize == 0);*/
	if (sch->cursize > 0)
	{	/* 删除该队列中存在的job */
		_lpzstSchPriList schpri= &(thr->que[sch->pri]);
		_lpzstScheduleJob job  = schpri->jobs;
		_lpzstScheduleJob next = NULL;
		_lpzstScheduleJob prev = job;
		while (job!=NULL && sch->cursize!=0)
		{
			next = job->next;
			if (job->sch==sch)
			{
				_zcrt_s_job_delete_has_mutex(thr, job);
				if (prev == job)
				{
					schpri->jobs = next;
					prev = next;
				}
				else
				{
					prev->next = next;
				}
				if(schpri->tail == job)
				{
					schpri->tail = NULL;
				}
			}
			else
			{
				prev = job;
			}
			job = next;
		}
	}

	if (thr->sch_list == sch)
	{
		thr->sch_list = sch->next;
	}
	else
	{
		_lpzstSchedule prev = thr->sch_list;
		while (prev && prev->next != sch)
		{
			prev = prev->next;
		}
		if(prev)
		{
			prev->next = sch->next;
		}
	}
	zcrt_mutex_unlock(thr->mutex);
}

void _zcrt_s_insert_job( _lpzstSchedule sch, _lpzstScheduleJob job ) 
{
	_lpzstSchThr thr = sch->thr;
	_lpzstSchPriList que=NULL;

	zcrt_mutex_lock(thr->mutex);
	que  = &thr->que[sch->pri];

	if (que->jobs)
	{
		ZCRT_ASSERT(que->tail);
		que->tail->next=job;
		que->tail=job;
	}
	else
	{
		ZCRT_ASSERT(que->tail==NULL);
		que->jobs = job;
		que->tail = job;
	}

	sch->cursize++;
	if (sch->maxhold<sch->cursize)
	{
		sch->maxhold = sch->cursize;
	}
	zcrt_mutex_unlock(thr->mutex);
	zcrt_sem_give(thr->sem);	
}

ZHANDLE_t zcrt_schedule_create( ZModule_t module, const char* name, EZCRTSchType type, EZCRTPriority pri, uint16_t maxsize )
{
	_lpzstSchedule sch = zcrt_malloc(sizeof(_zstSchedule), s_schtag);
	if (sch==NULL)
	{
		return NULL;
	}
	memset(sch,0,sizeof(_zstSchedule));
	memcpy(sch->name, name, strlen(name)>=SCH_NAME_LEN?SCH_NAME_LEN-1:strlen(name));

	if (type==EZCRTSchType_selfThread)
	{
		sch->thr = _zcrt_s_thr_new(module, name);
	}
	else
	{
		sch->thr = (_lpzstSchThr)module->zcrtthr;
	}
	sch->maxsize = maxsize;
	sch->pri = pri;
	sch->type = type;
	sch->module = module;
	zcrt_atom_inc(&(sch->thr->refCnt));

	_zcrt_s_thr_add_sch(sch->thr, sch);

	return sch;
}

ZHANDLE_t zcrt_schedule_create_share(ZModule_t module, ZHANDLE_t sch1, const char* name, EZCRTPriority pri, uint16_t maxsize)
{
	_lpzstSchedule sch = zcrt_malloc(sizeof(_zstSchedule), s_schtag);
	_lpzstSchedule shareSch = (_lpzstSchedule)sch1; 
	if (sch==NULL || sch1==NULL)
	{
		return NULL;
	}
	memset(sch,0,sizeof(_zstSchedule));
	memcpy(sch->name, name, strlen(name)>=SCH_NAME_LEN?SCH_NAME_LEN-1:strlen(name));

	if (shareSch->type==EZCRTSchType_selfThread)
	{
		sch->type = EZCRTSchType_shareThread;
		sch->thr = shareSch->thr;
	}
	else
	{
		sch->thr = (_lpzstSchThr)module->zcrtthr;
	}
	zcrt_atom_inc(&(sch->thr->refCnt));

	sch->maxsize = maxsize;
	sch->pri = pri;
	sch->module = module;

	_zcrt_s_thr_add_sch(sch->thr, sch);

	return sch;

}

void zcrt_schedule_delete( ZHANDLE_t h )
{
	_lpzstSchedule sch=(_lpzstSchedule)h;
	uint32_t cnt=0;
	if (h==NULL) return;
	if (sch->pri >= EZCRTSchPri_num)
	{
		ZCRT_ASSERT(sch->pri<EZCRTSchPri_num);
		return;
	}
	_zcrt_s_thr_remove_sch(sch->thr, sch);
	cnt=zcrt_atom_dec(&(sch->thr->refCnt));
	if (sch->type == EZCRTSchType_selfThread && cnt==0)
	{
		sch->thr->run = false;
	}
	sch->thr = NULL;
	zcrt_free(sch);
}

EZCRTErr zcrt_schedule_sendjob( ZHANDLE_t h, ZCRT_CB cb, void* p1, void* p2 )
{
	_lpzstSchedule sch=(_lpzstSchedule)h;
	_lpzstScheduleJob job=NULL;

	if (sch==NULL || cb==NULL ||sch->pri>=EZCRTSchPri_num) { return EZCRTErr_paramErr; }
	if (sch->cursize >= sch->maxsize) { return EZCRTErr_full; }
	ZCRT_ASSERT(sch->thr != NULL );
	if (sch->thr == NULL)
	{
		return EZCRTErr_paramErr;
	}

	job = _zcrt_s_job_new(sch->thr,cb);
	if (job==NULL)
	{
		return EZCRTErr_noMem;
	}

	job->argn = 2;
	job->args[0] = p1;
	job->args[1] = p2;
	job->sch = sch;
	_zcrt_s_insert_job(sch, job);

	return 0;
}

EZCRTErr zcrt_schedule_sendjob_argn( ZHANDLE_t h, ZCRT_CB cb, uint32_t argn, ... )
{
	_lpzstSchedule sch=(_lpzstSchedule)h;
	_lpzstScheduleJob job=NULL;
	va_list va;
	uint32_t i=0;

	if (sch==NULL || cb==NULL ||sch->pri>=EZCRTSchPri_num || argn==0 || argn>ZCRT_CB_MAX_ARGS) { return EZCRTErr_paramErr; }
	if (sch->cursize >= sch->maxsize) { return EZCRTErr_full; }

	job = _zcrt_s_job_new(sch->thr,cb);
	if (job==NULL)
	{
		return EZCRTErr_noMem;
	}

	job->argn = argn;
	job->sch = sch;

	va_start (va, argn);
	for (i = 0; i < argn; i++)
		job->args[i] = va_arg (va, void *);
	va_end (va);

	_zcrt_s_insert_job(sch, job);
	return 0;
}

ZHANDLE_t zcrt_schedule_module_init(void)
{
	return _zcrt_s_thr_new(NULL, "ZCRT_SCH");
}

void zcrt_schedule_module_delete(ZModule_t m)
{
	_lpzstSchThr thr=(_lpzstSchThr)m->zcrtthr;
	if (thr==NULL) return;
	thr->run = false;
	zcrt_sem_give(thr->sem);
	thr = (_lpzstSchThr)m->schthr_list;
	while(thr)
	{
		thr->run = false;
		zcrt_sem_give(thr->sem);
		thr = thr->next;
	}
}
