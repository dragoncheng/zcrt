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

typedef struct tagZEventCB
{
	ZCRT_EVT_CB fn;
	uint8_t pri;
	void* p1;
	void* p2;
	struct tagZEventCB* next;
}_zstEventCB,*_lpzstEventCB;

typedef struct tagZEvent
{
	ZHANDLE_t mutex;
	_lpzstEventCB freelist;
	int32_t freesize;
	uint32_t lastEvtid;
	_lpzstEventCB evtlist[ZCRT_MAX_EVT];
}_zstEvent,*_lpzstEvent;

static uint32_t s_evttag=ZCRT_MAKE_FOURCC('Z','E','V','T');

static _lpzstEventCB _zcrt_Event_malloc_cb( _lpzstEvent mng, uint8_t priority, ZCRT_EVT_CB fn, void* p1, void* p2 )
{
	_lpzstEventCB job = NULL;

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
		job=zcrt_malloc(sizeof(_zstEventCB), s_evttag);
		if (job==NULL)
		{
			return NULL;
		}
	}
	memset(job, 0, sizeof(_zstEventCB));
	job->fn = fn;
	job->p1 = p1;
	job->p2 = p2;
	job->pri = priority;
	return job;
}

static void _zcrt_Event_free_cb_no_protected( _lpzstEvent mng, _lpzstEventCB job)
{
	job->next = mng->freelist;
	mng->freelist = job;
	job->fn=NULL;
	mng->freesize++;
}
static void _zcrt_e_free_cbs(_lpzstEventCB cbs)
{
	_lpzstEventCB n=NULL;
	while (cbs)
	{
		n=cbs->next;
		zcrt_free(cbs);
		cbs=n;
	}
}
ZHANDLE_t zcrt_event_manage_init(void)
{
	_lpzstEvent mng=zcrt_malloc(sizeof(_zstEvent), s_evttag);
	if (mng==NULL)
	{
		return NULL;
	}
	memset(mng,0,sizeof(_zstEvent));
	mng->mutex = zcrt_mutex_create("ZEvent");

	return mng;
}
void zcrt_event_manage_delete(ZHANDLE_t h)
{
	_lpzstEvent mng=(_lpzstEvent)h;

	uint32_t i=0;
	if (mng==NULL)
	{
		return;
	}
	zcrt_mutex_delete(mng->mutex);
	for (i=0; i<ZCRT_MAX_EVT; i++)
	{
		ZCRT_ASSERT(mng->evtlist[i]==NULL);
		_zcrt_e_free_cbs(mng->evtlist[i]);
	}
	_zcrt_e_free_cbs(mng->freelist);
	zcrt_free(mng);

	return;
}

EZCRTErr zcrt_event_register( ZModule_t module, uint8_t priority, uint32_t evtid, ZCRT_EVT_CB cb, void* p1, void* p2 )
{
	_lpzstEvent mng = NULL;
	_lpzstEventCB job = NULL;

	if (module==NULL || module->evt_mng==NULL)
	{
		return EZCRTErr_invalidCrtModule;
	}

	if (evtid >= ZCRT_MAX_EVT || cb==NULL)
	{
		return EZCRTErr_paramErr;
	}
	mng = module->evt_mng;
	job = _zcrt_Event_malloc_cb(mng, priority, cb, p1, p2);

	zcrt_mutex_lock(mng->mutex);
	if (mng->evtlist[evtid] == NULL)
	{
		mng->evtlist[evtid] = job;
	}
	else
	{
		_lpzstEventCB cur=mng->evtlist[evtid];
		_lpzstEventCB prev=NULL;
		while (cur && cur->pri >= priority)
		{
			prev = cur;
			cur = cur->next;
		}
		if (prev==NULL)
		{
			job->next = mng->evtlist[evtid];
			mng->evtlist[evtid] = job;
		}
		else
		{
			job->next = prev->next;
			prev->next = job;
		}
	}
	zcrt_mutex_unlock(mng->mutex);
	return 0;
}

void zcrt_event_unregister( ZModule_t module, uint32_t evtid, ZCRT_EVT_CB cb, void* p1, void* p2 )
{
	_lpzstEvent mng = NULL;
	if (module==NULL || module->evt_mng==NULL || evtid >= ZCRT_MAX_EVT || cb==NULL)
	{
		return;
	}

	mng = module->evt_mng;
	ZCRT_ASSERT(mng->lastEvtid != evtid );
	zcrt_mutex_lock(mng->mutex);
	if (mng->evtlist[evtid] != NULL)
	{
		_lpzstEventCB cur=mng->evtlist[evtid];
		_lpzstEventCB prev=NULL;
		while (cur && cur->fn !=cb && cur->p1!=p1 && cur->p2!=p2)
		{
			prev = cur;
			cur = cur->next;
		}
		if (cur)
		{
			if (cur != mng->evtlist[evtid])
			{
				prev = cur->next;
			}
			else
			{
				mng->evtlist[evtid] = cur->next;
			}
			_zcrt_Event_free_cb_no_protected(mng, cur);
		}
	}
	zcrt_mutex_unlock(mng->mutex);
}
void zcrt_event_send( ZModule_t module, uint32_t evtid, void* evtdata )
{
	_lpzstEvent mng = NULL;
	if (module==NULL || module->evt_mng==NULL || evtid >= ZCRT_MAX_EVT)
	{
		return;
	}

	mng = module->evt_mng;
	mng->lastEvtid = evtid;
	zcrt_mutex_lock(mng->mutex);
	if (mng->evtlist[evtid] != NULL)
	{
		_lpzstEventCB cur=mng->evtlist[evtid];
		while (cur)
		{
			cur->fn(evtid, evtdata, cur->p1, cur->p2);
			cur = cur->next;
		}
	}
	zcrt_mutex_unlock(mng->mutex);
	mng->lastEvtid = 0;
}
