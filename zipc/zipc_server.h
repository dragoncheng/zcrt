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

#ifndef __zcrt_ipc_server_h__
#define __zcrt_ipc_server_h__

#include "zcrt/ztype.h"
#include "zcrt/zerrcode.h"
#include "zcrt/zbuf.h"

#ifdef __cplusplus
extern "C"{
#endif

#define ZIPC_BUF_LEN (1024*8)

/** 事件封装函数，如果返回非0，表示封装失败 */
typedef int32_t (*ZCRT_EVENT_ENCODE_FN)(uint32_t evtid, void* data, ZBuf_t buf);

typedef struct tagZIPC
{
	ZHANDLE_t thr;
	ZHANDLE_t sch;
	ZHANDLE_t respondSch;	
	ZHANDLE_t evtSch;
	ZHANDLE_t socket;
	ZModule_t module;
	ZHANDLE_t mutex;
	ZCRT_EVENT_ENCODE_FN evt_encode;
	int16_t port;
	bool_t run;
	ZHash_t modlist;	/*ZIPCComp list*/
	char name[ZCRT_NAME_LEN];
	ZBuf_t freebuf;
}ZIPCModule, *ZIPCModule_t;

struct tagZIPCComp;
typedef EZCRTErr (*ZIPC_CALL)(struct tagZIPCComp* comp, ZBuf_t ibuf, ZBuf_t obuf);
typedef struct tagZIPCComp
{
	char name[ZCRT_NAME_LEN];
	ZModule_t module;
	ZIPC_CALL call;
	ZHash_t apilist;	/*ZIPCComp list*/
	void* arg1;
	void* arg2;
	void* cbs;			/* callback functions,it's a struct */
}ZIPCComp, *ZIPCComp_t;

ZHANDLE_t zipc_server_create(ZModule_t module, uint16_t port, const char* name);
void zipc_server_delete(ZHANDLE_t ipc);

/**
* 设置事件的封装函数
*  @param module zcrt module
*  @param fn 封装函数，一般是自动生成
*/
void zipc_server_set_event_encode(ZModule_t module, ZCRT_EVENT_ENCODE_FN fn);
ZCRT_EVENT_ENCODE_FN zipc_server_get_event_encode(ZModule_t module);

/**
* 注册IPC模块
*  @param module zcrt module
*  @param comp IPC实现模块
*/
EZCRTErr zipc_server_register(ZModule_t module, ZIPCComp_t comp);
/**
* 注销IPC
*  @param module zcrt module
*  @param comp IPC实现模块
*/
void zipc_server_unregister(ZModule_t module, ZIPCComp_t comp);

#ifdef __cplusplus
};
#endif

#endif
