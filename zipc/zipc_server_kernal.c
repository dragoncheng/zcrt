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


#if defined(__KERNEL__)
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <linux/version.h>

#include "zcrt/zcrt.h"
#include "zipc/zipc_parse.h"

#define NETLINK_ZCRT_IPC 21

static ZIPCModule_t s_kipc = NULL;
static uint32_t s_tag=ZCRT_MAKE_FOURCC('Z','I','P','C');
static u32 s_pid=0;

static bool_t _zipc_client_recv_hdr(char* data, int datalen, ZIPCMsgHdr_t hdr)
{
	memcpy(hdr, data, ZIPC_HEAD_LEN);
	if (!zipc_parse_check_hdr(hdr))
	{
		ZLOG_ERROR("zcrt ipc client headr parse error: len:%d",datalen);
		return false;
	}
	hdr->appType = ntohs(hdr->appType);
	hdr->payloadSize = ntohl(hdr->payloadSize);
	if (hdr->payloadSize>=ZIPC_BUF_LEN)
	{
		ZLOG_ERROR("zcrt ipc msg payload size is illegal:%d",hdr->payloadSize);
		return false;
	}
	return datalen>=(hdr->payloadSize+ZIPC_HEAD_LEN);
}

static void _zipc_netlink_send(ZIPCModule_t ipc, ZBuf_t buf, u32 pid)
{
	struct sock *nl_sk = (struct sock *)ipc->socket;
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlh = NULL;
	skb = nlmsg_new(buf->len+NLMSG_LENGTH(0), 0);
	if (!skb){
		printk(KERN_ERR "net_link: allocate failed.\n");
		zipc_buf_free(ipc, buf);
		return;
    }
	ZCRT_ASSERT(buf->len<64000);	/*znetlink客户端接收的每个报文不能超过64k长度*/
	nlh=nlmsg_put(skb,0,0,NLMSG_DONE,buf->len,0); 
#if LINUX_VERSION_CODE< KERNEL_VERSION(3,1,0)
	NETLINK_CB(skb).pid = 0; /* from kernel */
#endif
	memcpy(NLMSG_DATA(nlh), buf->buf, buf->len);
	netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT);

	zipc_buf_free(ipc, buf);
}

static void _zipc_hookevent_cb(uint32_t evtid, void* evtdata, ZIPCModule_t ipc, u32 pid)
{
	ZBuf_t buf = NULL;
	buf = zipc_buf_malloc(ipc);
	if (buf==NULL)
	{
		return;
	}
	if (zipc_encode_event(ipc->module, evtid, evtdata, buf)==0)
	{
		/* _zipc_netlink_send( (struct sock *)ipc->socket, buf, pid); */
		if (zcrt_schedule_sendjob_argn(ipc->evtSch, (ZCRT_CB)_zipc_netlink_send, 3, ipc, buf, pid) !=0 )
		{
			zipc_buf_free(ipc, buf);
			ZLOG_ERROR("zipc evtSch schedule was full:evtid=%d\n",evtid);
		}
	}
}

static void _zipc_msg_proc_job(ZIPCModule_t ipc, ZBuf_t ibuf, uint16_t appType, u32 pid)
{
	ZBuf_t obuf = zipc_buf_malloc(ipc);
	ZIPCMsgHdr_t rhdr=NULL;
	if (obuf==NULL)
	{
		ZLOG_ERROR("zbuf no memory");
		zipc_buf_free(ipc, ibuf);
		return;
	}
	if (appType == ZEIPC_TYPE_LINK)
	{
		zipc_parse_app_link(ipc->module, ibuf, obuf, (ZCRT_EVT_CB)_zipc_hookevent_cb, (void*)ipc, (void*)pid);
	}
	else
	{
		zipc_parse_payload(ipc->module, appType, ibuf, obuf);
	}
	if (obuf->len !=0)
	{
		rhdr = (ZIPCMsgHdr_t)obuf->buf;
		rhdr->payloadSize = htonl(obuf->len-ZIPC_HEAD_LEN);
		//_zipc_netlink_send( (struct sock *)ipc->socket, obuf, pid);
		if( zcrt_schedule_sendjob_argn(ipc->respondSch, (ZCRT_CB)_zipc_netlink_send, 3, ipc, obuf, pid) !=0 )
		{
			zipc_buf_free(ipc, obuf);
			ZLOG_ERROR("zipc respondSch schedule was full");
		}
	}
	zipc_buf_free(ipc, ibuf);
	/* zipc_buf_free(ipc, obuf); */
}

void nl_data_recv (struct sk_buff *__skb)
{
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlh = NULL;
	char* data=NULL;
	ZIPCMsgHdr hdr;
	int datalen=0;
	u32 pid=0;
	int32_t appType = 0;
	ZBuf_t buf = zipc_buf_malloc(s_kipc);
	skb = skb_get(__skb);

	if (buf==NULL)
	{
		kfree_skb(skb);
		printk("IPC no memory \n");
		return;
	}

	if (skb->len < NLMSG_SPACE(ZIPC_HEAD_LEN) || s_kipc==NULL)
	{
		kfree_skb(skb);
		printk("recv error %d 0x%x\n", skb->len, (unsigned int)s_kipc);  
		return;
	}
	nlh = nlmsg_hdr(skb);
	data = (char *)NLMSG_DATA(nlh);
	datalen = nlh->nlmsg_len - NLMSG_LENGTH(0);
	pid = nlh->nlmsg_pid; /*pid of sending process */
	s_pid=pid;
	if (!_zipc_client_recv_hdr(data, datalen, &hdr))
	{
		kfree_skb(skb);
		printk("recv hdr error\n");  
		return;
	}
	
	buf->len = 0;
	buf->offset = 0;
	zcrt_buf_append(buf, data+ZIPC_HEAD_LEN, hdr.payloadSize);
	appType = hdr.appType;

	if (zcrt_schedule_sendjob_argn(s_kipc->sch, (ZCRT_CB)_zipc_msg_proc_job, 4, s_kipc, buf, (void*)appType, (void*)pid) !=0 )
	{
		zipc_buf_free(s_kipc, buf);
		//ZLOG_ERROR("zipc sch schedule was full");
	}

	kfree_skb(skb);
	return;
}

ZHANDLE_t zipc_server_create(ZModule_t module, uint16_t port, const char* name)
{
	struct sock * nl_sk = NULL; 
	ZIPCModule_t ipc = NULL;

	ipc = (ZIPCModule_t)zcrt_malloc(sizeof(ZIPCModule), s_tag);
	s_kipc=ipc;

	memset(ipc, 0, sizeof(ZIPCModule));
	memcpy(ipc->name, name, strlen(name)>=ZCRT_NAME_LEN?ZCRT_NAME_LEN-1:strlen(name));

	ipc->module = module;
	ipc->port = port;

	ipc->mutex = zcrt_mutex_create("ZIPC");
	ipc->modlist = zcrt_hash_new(128,zipc_component_hash_key, zipc_component_hash_cmp);
	ipc->sch = zcrt_schedule_create(module, "zipc", EZCRTSchType_zcrtThread, EZCRTSchPri_priNormal, 100);
	ipc->respondSch = zcrt_schedule_create(module, "zipc", EZCRTSchType_selfThread, EZCRTSchPri_priNormal, 500);
	ipc->evtSch = zcrt_schedule_create_share(module, ipc->respondSch, "zipc", EZCRTSchPri_priLow, 1500);

#if LINUX_VERSION_CODE< KERNEL_VERSION(3,1,0)
	nl_sk = netlink_kernel_create(&init_net, NETLINK_ZCRT_IPC, 0, nl_data_recv, NULL, THIS_MODULE);
#else
	struct netlink_kernel_cfg cfg = {
    		.input = nl_data_recv,
	};
	nl_sk = netlink_kernel_create(&init_net, NETLINK_ZCRT_IPC, &cfg);
#endif
	if (! nl_sk) {  
		printk("Fail to create netlink socket.\n");  
		return NULL;  
	}

	ipc->socket = (ZHANDLE_t)nl_sk;
	ipc->run = true;
	
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
	struct sock * nl_sk = NULL;
	uint32_t i=0;
	if(h==NULL) return;
	s_kipc=NULL;
	for (i=1; i<ZCRT_MAX_EVT; i++)
	{
		zcrt_event_unregister(ipc->module, i, (ZCRT_EVT_CB)_zipc_hookevent_cb, (void*)ipc,  (void*)s_pid);
	}

	nl_sk = (struct sock *)ipc->socket;
	ipc->run = false;
	sock_release(nl_sk->sk_socket);

	_zipc_comp_buf_freeall(ipc);
	zcrt_hash_delete(ipc->modlist);
	zcrt_schedule_delete(ipc->evtSch);
	zcrt_schedule_delete(ipc->respondSch);
	zcrt_schedule_delete(ipc->sch);
	zcrt_mutex_delete(ipc->mutex);
	zcrt_free(ipc);
}

#endif
