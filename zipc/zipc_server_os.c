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


#if defined(WIN32) || defined(LINUX_OS)

#include "zcrt/zcrt.h"
#include "zipc/zipc_parse.h"

#ifdef WIN32
#include <Windows.h>
#endif

#ifdef LINUX_OS
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <errno.h>
#include <unistd.h>

#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define WSAGetLastError() errno
#define GetLastError() errno
#define closesocket(fd) close(fd)
#endif

typedef struct tagZIPCClient
{
	ZIPCModule_t ipc;
	SOCKET sock;
	ZHANDLE_t thr;
	bool_t isEventHooked;	/*本链接是否被注册过事件关注 */
	int32_t jobcnt;		/*发送给job队列没执行的job数*/
}_zstZIPCClient,*_lpzstZIPCClient;

static uint32_t s_tag=ZCRT_MAKE_FOURCC('Z','I','P','C');

static bool_t _zipc_client_recv_hdr(SOCKET clientSocket, ZIPCMsgHdr_t hdr)
{
	uint32_t len=0;
	if((len=recv(clientSocket,(char*)hdr,ZIPC_HEAD_LEN,0))==SOCKET_ERROR)
	{ 
		ZLOG_ERROR("zcrt ipc client headr receive error:%d",WSAGetLastError());
		return false;
	}
	if (len!=ZIPC_HEAD_LEN || !zipc_parse_check_hdr(hdr))
	{
		ZLOG_ERROR("zcrt ipc client headr parse error: len:%d err:0x%x",len, WSAGetLastError());
		return false;
	}
	hdr->appType = ntohs(hdr->appType);
	hdr->payloadSize = ntohl(hdr->payloadSize);
	if (hdr->payloadSize>=ZIPC_BUF_LEN)
	{
		ZLOG_ERROR("zcrt ipc msg payload size is illegal:%d",hdr->payloadSize);
		return false;
	}
	return true;
}
static bool_t _zipc_client_recv_payload(SOCKET clientSocket, uint32_t payloadSize, int8_t* buf)
{
	uint32_t len=0;
	if((len=recv(clientSocket,buf,payloadSize,0))==SOCKET_ERROR)
	{ 
		ZLOG_ERROR("zcrt ipc client payload receive error:%d",WSAGetLastError());
		return false;
	} 
	if (len!=payloadSize)
	{
		ZLOG_ERROR("zcrt ipc client payload receive error len:%d err:0x%x",len, WSAGetLastError());
		return false;
	}
	return true;
}

static void _zipc_buf_send_job(void* p1, void*p2)
{
	_lpzstZIPCClient client = (_lpzstZIPCClient)p1;
	ZBuf_t buf   = (ZBuf_t)p2;
	if (client && buf)
	{
		send(client->sock, buf->buf, buf->len, 0);
		zipc_buf_free(client->ipc, buf);
	}
}
/**/
static void _zipc_hookevent_cb(uint32_t evtid, void* evtdata, _lpzstZIPCClient client, void* p2)
{
	ZBuf_t buf = NULL;
	static bool_t notprint=true;
	ARG_UNUSED(p2);
	buf = zipc_buf_malloc(client->ipc);
	if (buf==NULL)
	{
		return;
	}
	if (zipc_encode_event(client->ipc->module, evtid, evtdata, buf)==0)
	{
		if (zcrt_schedule_sendjob(client->ipc->evtSch, (ZCRT_CB)_zipc_buf_send_job, client, buf) != 0)
		{
			zipc_buf_free(client->ipc, buf);
			if (notprint)
			{
				ZLOG_ERROR("zipc evtSch schedule was full:evtid=%d\n",evtid);
				notprint = false;
			}
		}
		else
		{
			if ( notprint==false )
			{
				ZLOG_ERROR("zipc evtSch schedule was full finished:evtid=%d\n",evtid);
				notprint = true;
			}
		}

	}
	else
	{
		zipc_buf_free(client->ipc, buf);
	}
}

static void _zipc_msg_proc_job(void* p1, void* p2, void* p3)
{
	_lpzstZIPCClient client=(_lpzstZIPCClient)p1;
	ZBuf_t ibuf = (ZBuf_t)p2;
	uint16_t appType=(uint16_t)((uint32_t)p3);

	//SOCKET clientSocket=client->sock;
	ZBuf_t obuf = zipc_buf_malloc(client->ipc);
	ZIPCMsgHdr_t rhdr=NULL;

	if (obuf==NULL)
	{
		ZLOG_ERROR("zbuf no memory");
		zipc_buf_free(client->ipc, ibuf);
		return;
	}
	if (appType == ZEIPC_TYPE_LINK)
	{
		client->isEventHooked=true;
		zipc_parse_app_link(client->ipc->module, ibuf, obuf, (ZCRT_EVT_CB)_zipc_hookevent_cb, (void*)client, NULL);
	}
	else
	{
		zipc_parse_payload(client->ipc->module, appType, ibuf, obuf);
	}

	if (obuf->len !=0)
	{
		rhdr = (ZIPCMsgHdr_t)obuf->buf;
		rhdr->payloadSize = htonl(obuf->len-ZIPC_HEAD_LEN);
		/* send(clientSocket, obuf->buf, obuf->len, 0); */
		if (zcrt_schedule_sendjob(client->ipc->respondSch, (ZCRT_CB)_zipc_buf_send_job, client, obuf) != 0)
		{
			zipc_buf_free(client->ipc, obuf);
			ZLOG_ERROR("zipc respondSch schedule was full");
		}
	}
	else
	{
		zipc_buf_free(client->ipc, obuf);
	}
	zipc_buf_free(client->ipc, ibuf);
	zcrt_atom_dec(&(client->jobcnt));
}

static int _zipc_client_thread(void* arg)
{
	_lpzstZIPCClient client=(_lpzstZIPCClient)arg;
	SOCKET clientSocket=client->sock;
	ZIPCModule_t ipc = client->ipc;
	ZIPCMsgHdr hdr;
	int32_t appType = 0;

	ZBuf_t buf = zipc_buf_malloc(ipc);
	while(client->ipc->run && !zcrt_thread_should_stop(client->thr))
	{
		if (!_zipc_client_recv_hdr(clientSocket, &hdr))
		{
			break;
		}
		zcrt_buf_realloc(buf, hdr.payloadSize);
		if (!_zipc_client_recv_payload(clientSocket, hdr.payloadSize, buf->buf))
		{
			break;
		}
		buf->len = hdr.payloadSize;
		buf->offset = 0;
		appType = hdr.appType;
		
		zcrt_atom_inc(&(client->jobcnt));
		if (zcrt_schedule_sendjob_argn(ipc->sch, (ZCRT_CB)_zipc_msg_proc_job, 3, client, buf, (void*)appType) !=0 )
		{
			zcrt_atom_dec(&(client->jobcnt));
			zipc_buf_free(ipc,buf);
			ZLOG_ERROR("zipc sch schedule was full");
		}
		buf = zipc_buf_malloc(ipc);
		if (buf ==NULL)
		{
			ZLOG_ERROR("zbuf no memory");
			break;
		}
	}
	while(client->jobcnt !=0)
	{
		zcrt_sleep(10);
	}
	if (client->isEventHooked)
	{
		int i=0;
		for (i=1; i<ZCRT_MAX_EVT; i++)
		{
			zcrt_event_unregister(client->ipc->module, i, (ZCRT_EVT_CB)_zipc_hookevent_cb, (void*)client, NULL);
		}
	}
	if (buf)
	{
		zipc_buf_free(ipc, buf);
	}

	closesocket(clientSocket);
	zcrt_free(client);
	return 0;
}
static int _zipc_server_thread(void* arg)
{
	ZIPCModule_t ipc=(ZIPCModule_t)arg;
	struct sockaddr_in localAddr;
	struct sockaddr_in clientAddr;
	SOCKET  clientSocket = 0;
	int addrLen = 0;
	SOCKET sock =socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int sock_opt = 1;
	if (sock==(uint32_t)(~0))
	{
		ZLOG_ERROR("zcrt ipc socket init error%d\n",GetLastError());
		return -1;
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&sock_opt, sizeof(sock_opt) ) == -1){
		return -1;
	}

	ipc->socket = (ZHANDLE_t)sock;
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(ipc->port);
	memset(localAddr.sin_zero, 0x00, 8);

	if( bind(sock, (struct sockaddr*)&localAddr, sizeof(localAddr)) !=0 )
	{
		ZLOG_ERROR("zcrt ipc socket bind error:%d\n",GetLastError());
		return -1;
	}
	if (listen(sock, 10)!=0)
	{
		ZLOG_ERROR("zcrt ipc socket listen error:%d\n",GetLastError());
		return -1;
	}

	while (ipc->run && !zcrt_thread_should_stop(ipc->thr))
	{
		_lpzstZIPCClient client=NULL;
		addrLen = sizeof(clientAddr);
		clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &addrLen);
		if ( clientSocket == INVALID_SOCKET )
		{
			ZLOG_ERROR("zcrt ipc socket accept error:%d\n",GetLastError());
			break;
		}
		client = (_lpzstZIPCClient)zcrt_malloc(sizeof(_zstZIPCClient), s_tag);
		memset(client, 0, sizeof(_zstZIPCClient) );
		client->ipc = ipc;
		client->sock = clientSocket;

		client->thr = zcrt_thread_create("ZIPCCLI", 0, ZCRT_THREAD_PRI_NOMAL, _zipc_client_thread, (void*)client);
	}
	closesocket(sock);
	return 0;
}

ZHANDLE_t zipc_server_create(ZModule_t module, uint16_t port, const char* name)
{
	ZIPCModule_t ipc=(ZIPCModule_t)zcrt_malloc(sizeof(ZIPCModule), s_tag);
	memset(ipc, 0, sizeof(ZIPCModule));
	memcpy(ipc->name, name, strlen(name)>=ZCRT_NAME_LEN?ZCRT_NAME_LEN-1:strlen(name));

	ipc->module = module;
	ipc->port = port;
	ipc->run = true;
	ipc->mutex = zcrt_mutex_create("ZIPC");
	ipc->modlist = zcrt_hash_new(128,zipc_component_hash_key, zipc_component_hash_cmp);
	ipc->sch = zcrt_schedule_create(module, "zipc", EZCRTSchType_zcrtThread, EZCRTSchPri_priNormal, 100);
	ipc->respondSch = zcrt_schedule_create(module, "zipc", EZCRTSchType_selfThread, EZCRTSchPri_priNormal, 500);
	ipc->evtSch = zcrt_schedule_create_share(module, ipc->respondSch, "zipc", EZCRTSchPri_priLow, 500);
	ipc->thr = zcrt_thread_create("ZIPCSVR", 0, ZCRT_THREAD_PRI_NOMAL, _zipc_server_thread, ipc);
	return (ZHANDLE_t)ipc;
}

void _zipc_comp_buf_freeall(ZIPCModule_t ipc)
{
	ZBuf_t buf=ipc->freebuf;
	ZBuf_t next=NULL;
	zcrt_mutex_lock(ipc->mutex);
	while (buf)
	{
		next = buf->next;
		zcrt_buf_destory(buf);
		zcrt_free(buf);
		buf = next;
	}
	zcrt_mutex_unlock(ipc->mutex);
}
void zipc_server_delete( ZHANDLE_t h )
{
	ZIPCModule_t ipc=(ZIPCModule_t)h;
	ipc->run = false;
	closesocket((SOCKET)(ipc->socket));

	_zipc_comp_buf_freeall(ipc);
	zcrt_hash_delete(ipc->modlist);
	zcrt_schedule_delete(ipc->evtSch);
	zcrt_schedule_delete(ipc->respondSch);
	zcrt_schedule_delete(ipc->sch);
	zcrt_mutex_delete(ipc->mutex);
	zcrt_free(ipc);
}

#endif
