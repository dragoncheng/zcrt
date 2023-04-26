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

ZHANDLE_t zcrt_timer_manage_init(void);
void zcrt_timer_manage_delete(ZHANDLE_t);

ZHANDLE_t zcrt_event_manage_init(void);
void zcrt_event_manage_delete(ZHANDLE_t);

ZHANDLE_t zcrt_schedule_module_init(void);
void zcrt_schedule_module_delete(ZModule_t);

static uint32_t s_tag=ZCRT_MAKE_FOURCC('Z','M','O','D');
static ZModule_t s_global_module=NULL;

extern uint32_t g_zcrt_starttime;
extern uint32_t g_zcrt_start_cpu_clock;

void zcrt_set_global_module(ZModule_t m)
{
	s_global_module = m;
}

ZModule_t zcrt_get_global_module( void )
{
	return s_global_module;
}

ZModule_t zcrt_module_new(uint16_t port)
{
	ZModule_t m=zcrt_malloc(sizeof(ZCRTModule), s_tag);
	memset(m, 0, sizeof(ZCRTModule));
	m->crt_mutex = zcrt_mutex_create("ZModule");
	m->timer_mng = zcrt_timer_manage_init();
	m->evt_mng   = zcrt_event_manage_init();
	m->zcrtthr   = zcrt_schedule_module_init();
	m->zipc		 = zipc_server_create(m, port, "ZIPC");
	g_zcrt_starttime = zcrt_current_time();
	g_zcrt_start_cpu_clock = zcrt_cpu_clock();
	return m;
}
void zcrt_module_delete( ZModule_t m )
{
	zipc_server_delete(m->zipc);
	zcrt_event_manage_delete(m->evt_mng);
	zcrt_timer_manage_delete(m->timer_mng);
	zcrt_schedule_module_delete(m);

	zcrt_sleep(200);

	zcrt_mutex_delete(m->crt_mutex);
	zcrt_free(m);
}

void zcrt_module_lock( ZModule_t m )
{
	if (m && m->crt_mutex)
	{
		zcrt_mutex_lock(m->crt_mutex);
	}
}

void zcrt_module_unlock( ZModule_t m )
{
	if (m && m->crt_mutex)
	{
		zcrt_mutex_unlock(m->crt_mutex);
	}
}

