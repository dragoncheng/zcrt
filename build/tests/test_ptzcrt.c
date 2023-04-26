#include "zcrt/zcrt.h"
#include "test_api_decode.h"

#ifdef WIN32
#include <winsock2.h>
#include "windows.h"
#include <shellapi.h>
//#include <vld.h>
#else
#ifndef __KERNEL__
#include<signal.h> 
#endif
#endif

int g_idx=0;
ZModule_t g_module;
bool_t brun=true;
ZHANDLE_t sch = NULL;
ZHANDLE_t sch_one = NULL;
ZIPCComp_t testcomp = NULL;
ZHANDLE_t s_testmutex=NULL;
int s_mutexcnt=0;		//测试同一个thread可以多次lock同一个mutex

#ifdef __KERNEL__
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> 
#define PRINT(fmt) printk(fmt)
#else
#include <stdio.h>
#define PRINT(fmt) printf(fmt)
#endif

static void test_event(uint32_t evtid, void* evtdata, void* p1, void* p2)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(evtid);
	ARG_UNUSED(p2);
	PRINT(("test event %s \n",(char*)evtdata));
}

static void test_job1(void* p1, void* p2)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ZCRT_ASSERT(g_idx==*(int*)p1);
	PRINT(("test job1\n"));
}
static void test_job2(void* p1, void* p2)
{
	static int ii=0;
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ZCRT_ASSERT(g_idx==*(int*)p1);
	PRINT(("test job2......\n"));
	ii++;
	if ((ii%3)==0)
	{
		zcrt_event_send(g_module,1, (void*)"aaaaaaaaaaa");
	}
}
int g_sch_pri=0;
static void test_sch_high(void* p1, void* p2)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ZCRT_ASSERT(g_sch_pri==0);
	g_sch_pri++;
	PRINT(("test sch high......\n"));
}
static void test_sch_one(void* p1, void* p2)
{
	static int ii=0;
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ZCRT_ASSERT(g_sch_pri==1);
	g_sch_pri++;
	PRINT(("test one......\n"));
	ii++;
}
static void test_sch_pri(void* p1, void* p2)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	PRINT(("test sch pri......\n"));
	zcrt_schedule_sendjob(sch_one, (ZCRT_CB)test_sch_one, (void*)&g_idx, NULL);
	ZCRT_ASSERT( zcrt_schedule_sendjob(sch_one, (ZCRT_CB)test_sch_one, (void*)&g_idx, NULL)!=0);
	zcrt_schedule_sendjob(sch, (ZCRT_CB)test_sch_high, (void*)&g_idx, NULL);
}

static void test_timer1(void* p1, void* p2)
{
	ZHANDLE_t sch = (ZHANDLE_t)p1;
	int idx=(int)p2;
	if (!brun) return;
	ZCRT_ASSERT(idx==1 || idx==0);

	g_idx++;
	if (idx==0)
	{
		zcrt_schedule_sendjob(sch, (ZCRT_CB)test_job1, (void*)&g_idx, NULL);
	}
	else
	{
		zcrt_schedule_sendjob(sch, (ZCRT_CB)test_job2,(void*) &g_idx, NULL);
	}
	zcrt_timer_add(g_module, 1000,(ZCRT_CB)test_timer1, (void*)sch, (void*)(1-idx));
}

static void test_thread_mutex_unlock_first(ZHANDLE_t mutex)
{
	zcrt_mutex_unlock(mutex);
	ZCRT_ASSERT(s_mutexcnt==3);
}

static void test_thread_mutex_lock_second(ZHANDLE_t mutex)
{
	zcrt_mutex_lock(mutex);
	s_mutexcnt++;
	ZCRT_ASSERT(s_mutexcnt==3);
	zcrt_mutex_unlock(mutex);
	zcrt_schedule_sendjob(sch, (ZCRT_CB)test_thread_mutex_unlock_first,(void*) mutex, NULL);
}

static void test_thread_mutex_lock_inner(ZHANDLE_t mutex)
{
	zcrt_mutex_lock(mutex);
	s_mutexcnt++;
	ZCRT_ASSERT(s_mutexcnt==2);
	zcrt_mutex_unlock(mutex);
}
static void test_thread_mutex_lock_first(ZHANDLE_t mutex)
{
	zcrt_mutex_lock(mutex);
	s_mutexcnt++;
	test_thread_mutex_lock_inner(mutex);
	zcrt_schedule_sendjob(sch, (ZCRT_CB)test_thread_mutex_lock_second,(void*) mutex, NULL);
}

static void test_thread_mutex_check()
{
	PRINT("check thread mutex.....");
	ZCRT_ASSERT(s_mutexcnt==3);	/* thread mutex*/
	PRINT("ok\n");
}

ZIPCComp_t ipc_pkg_test_create(ZModule_t g_module, const char* modname, void* arg1, void* arg2);

int32_t ipc_event_encode(uint32_t evtid, void* data, ZBuf_t buf)
{
	switch(evtid)
	{
	case 2:
		ztlv_put_int(buf, *(int32_t*)data);
		return 0;

	}
	return -1;
}

int s=1000;
void test_ipc_register(void)
{
	testcomp = ipc_pkg_testModule_create(g_module,"testapi", NULL,(void*)g_module, (void*)&s);
	zipc_server_register(g_module, testcomp);
	zipc_server_set_event_encode(g_module, ipc_event_encode);
}

void test_init(void)
{
	g_module = zcrt_module_new(ZIPC_SVR_DEFAULT_PORT);
	test_ipc_register();
	sch = zcrt_schedule_create(g_module, "test", EZCRTSchType_zcrtThread, EZCRTSchPri_priNormal, 100);
	sch_one = zcrt_schedule_create(g_module, "testone", EZCRTSchType_zcrtThread, EZCRTSchPri_priLow, 1);
	zcrt_event_register(g_module, 0, 1, test_event, 0,0);
	zcrt_timer_add(g_module, 1000, (ZCRT_CB)test_timer1, (void*)sch, (void*)0);
	zcrt_schedule_sendjob(sch, (ZCRT_CB)test_sch_pri, (void*)&g_idx, NULL);
	s_testmutex = zcrt_mutex_create("test");
	ZCRT_ASSERT(s_testmutex);
	zcrt_schedule_sendjob(sch, (ZCRT_CB)test_thread_mutex_lock_first, (void*)s_testmutex, NULL);
	zcrt_timer_add(g_module, 2000, (ZCRT_CB)test_thread_mutex_check, (void*)0, (void*)0);

}

void test_stop(void)
{
	zcrt_mutex_delete(s_testmutex);
	zcrt_event_unregister(g_module, 1, test_event,0,0);
	zcrt_schedule_delete(sch);
	zcrt_schedule_delete(sch_one);

	zipc_server_unregister(g_module, testcomp);
	ipc_pkg_testModule_delete(testcomp);

#ifdef WIN32
	WSACleanup();
#endif
	zcrt_module_delete(g_module);
}

#ifndef __KERNEL__
#ifndef WIN32
void singal_fun(int signal)
{
	printf("app crashed！\n");
	zcrt_module_delete(g_module);
	g_module = NULL;
}
#endif

int main(void)
{
	int loop=0;
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return -1;
	}
#else
	signal(SIGINT, singal_fun);
	signal(SIGABRT, singal_fun);
	signal(SIGFPE, singal_fun);
	signal(SIGSEGV, singal_fun);
#endif
	test_init();

	while (loop<2000)
	{
		zcrt_sleep(10);
		loop++;
	}

	brun=false;
	loop=0;
	while (loop<1000)
	{
		zcrt_sleep(10);
		loop++;
	}

	test_stop();

	zcrt_sleep(2000);
	return 0;
}
#else
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> 

MODULE_LICENSE("GPL");

static int __init testzcrt_init(void){
	printk(KERN_ALERT "Hello World enter...\n");
	test_init();
	printk(KERN_ALERT "Hello World entered\n");
	return 0;
}

static void __exit testzcrt_exit(void) {
	printk(KERN_ALERT "Hello World exit...\n");
	test_stop();
	printk(KERN_ALERT "Hello World existed\n");
}

module_init(testzcrt_init);
module_exit(testzcrt_exit);

#endif
