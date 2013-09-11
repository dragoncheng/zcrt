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
#include "zipc/zipc_parse.h"
#include "zipc/zipc_tlv.h"

#ifdef WIN32
	#include <windows.h>
#else
	#ifndef __KERNEL__
	#include <netinet/in.h>
	#endif
#endif

static uint32_t s_tag=ZCRT_MAKE_FOURCC('Z','I','P','C');

static uint16_t s_zipc_sig=('G'<<8)+'L';
#define ZIPC_MSG_HDR_CONNST "GL00"

uint32_t zipc_component_hash_key(void* data)
{
	uint32_t key=0;
	char* name=((ZIPCComp_t)data)->name;
	for (;*name; ++name, key+=*name);
	return key;
}

int32_t zipc_component_hash_cmp(const void* data1,const void* data2)
{
	return strcmp(((ZIPCComp_t)data1)->name, ((ZIPCComp_t)data2)->name);
}

bool_t zipc_parse_check_hdr( ZIPCMsgHdr_t hdr )
{
	if (ntohs(hdr->signature) != s_zipc_sig)
	{
		return false;
	}
	return true;
}

bool_t zipc_parse_common_hdr( ZBuf_t ibuf, ZIPCMsgCH_t ch )
{
	uint32_t cnt=0;
	uint32_t i=0;
	zstr_t key;
	memset(ch, 0, sizeof(ZIPCMsgCH));
	ZTLV_CHECK_GET_LEN(ibuf, cnt, false);

	for (i=0; i<cnt; i++)
	{
		ZTLV_CHECK_GET_STR(ibuf, key, false);
		if (zstr_buf_cmp(&key, "sn")==0)
		{
			ZTLV_CHECK_GET_INT(ibuf, ch->sn, false);
			continue;
		}
		if (zstr_buf_cmp(&key, "req")==0)
		{
			ZTLV_CHECK_GET_BOOL(ibuf, ch->req, false);
			continue;
		}
		if (zstr_buf_cmp(&key, "noti")==0)
		{
			ZTLV_CHECK_GET_BOOL(ibuf, ch->noti, false);
			continue;
		}
		ztlv_get_ignore(ibuf);
	}
	return true;
}

EZCRTErr zipc_comp_call(ZModule_t module, ZIPCComp_t comp, zstr_t* action, ZBuf_t ibuf, ZBuf_t obuf)
{
	ZIPCComp_t api = NULL;
	ARG_UNUSED(module);
	if (comp==NULL || action==NULL)
	{
		return EZCRTErr_paramErr;
	}
	api = zipc_parase_lookup_comp(comp->apilist, action->ptr, action->len);
	if (api==NULL || api->call==NULL)
	{
		return EZCRTErr_ipcNoAct;
	}
	return api->call(comp, ibuf, obuf);
}

EZCRTErr zipc_parse_body_info( ZBuf_t ibuf, zstr_t* compname, zstr_t* action ) 
{
	uint32_t idx=0;
	/*table count*/
	ZTLV_CHECK_GET_LEN(ibuf, idx, EZCRTErr_ipcBodyErr);
	if (idx<3)
	{
		return EZCRTErr_ipcBodyErr;
	}

	/*the first is nodeid*/
	ZTLV_CHECK_GET_INT(ibuf, idx, EZCRTErr_ipcBodyErr);
	/* Component name */
	if (!ztlv_get_string(ibuf, compname) || compname->len>=ZCRT_NAME_LEN)
	{
		return EZCRTErr_ipcBodyErr;
	}

	/*the second is action*/
	ZTLV_CHECK_GET_INT(ibuf, idx, EZCRTErr_ipcBodyErr);
	/* action */
	if (!ztlv_get_string(ibuf, action) || action->len>=ZCRT_NAME_LEN)
	{
		return EZCRTErr_ipcBodyErr;
	}
	return 0;
}
/*require format: {1='nodeid',2='action', 3={...}*/
EZCRTErr zipc_parse_request(ZModule_t module, ZBuf_t ibuf, ZIPCMsgCH_t ch, ZBuf_t obuf)
{
	zstr_t compname;
	zstr_t action;
	ZIPCModule_t ipc = (ZIPCModule_t)module->zipc;
	ZIPCComp_t comp = NULL;
	EZCRTErr ret=0;
	uint32_t idx=0;
	ARG_UNUSED(ch);

	ret = zipc_parse_body_info(ibuf, &compname, &action);
	if (ret != 0)
	{
		return ret;
	}

	comp = zipc_parase_lookup_comp(ipc->modlist, compname.ptr, compname.len);

	if (comp==NULL)
	{
		return EZCRTErr_ipcNoModule;
	}
	/*the third is args, and the args is a table*/
	ZTLV_CHECK_GET_INT(ibuf, idx, EZCRTErr_ipcBodyErr)
	if (ztlv_get_type(ibuf) != ZTLV_TABLE)
	{
		return EZCRTErr_ipcArgsErr;
	}
	return zipc_comp_call(module, comp, &action, ibuf, obuf);
}

EZCRTErr zipc_parse_app_default(ZModule_t module, ZBuf_t ibuf, ZBuf_t obuf)
{
	ZIPCMsgCH ch;
	EZCRTErr ret=0;

	/*common header*/
	if (!zipc_parse_common_hdr(ibuf, &ch))
	{
		return EZCRTErr_ipcCommonHdrErr;
	}

	if (ch.req)
	{
		zipc_parse_init_buf(obuf, ZEIPC_TYPE_DEFAULT);
		zipc_parse_put_reply_commonhdr(obuf, &ch, true);
		ret = zipc_parse_request(module, ibuf, &ch, obuf);
		if( ret!=EZCRTErr_success)
		{
			zipc_parse_put_reply_errcode(obuf, ret);
		}
		return ret;
	}
	return EZCRTErr_ipcNotSupport;
}

EZCRTErr zipc_parse_payload( ZModule_t module, uint16_t appType, ZBuf_t ibuf, ZBuf_t obuf )
{
	EZCRTErr r=EZCRTErr_ipcNotSupport;
	switch (appType)
	{
	case ZEIPC_TYPE_DEFAULT:
		r = zipc_parse_app_default(module, ibuf, obuf);
		break;
	}
	return r;
}

static EZCRTErr _zipc_link_parase_eventid(ZBuf_t ibuf, uint32_t* evtid)
{
	uint32_t idx=0;
	/*the args is a table*/
	ZTLV_CHECK_GET_INT(ibuf, idx, EZCRTErr_ipcBodyErr);
	if (ztlv_get_type(ibuf) != ZTLV_TABLE) return EZCRTErr_ipcArgsErr;
	if (!ztlv_get_len(ibuf, &idx)) return EZCRTErr_ipcTLVErr;
	if( !ztlv_get_int(ibuf, (int32_t*)&idx) ) return EZCRTErr_ipcTLVErr;
	if( !ztlv_get_uint32(ibuf, evtid) ) return EZCRTErr_ipcTLVErr;
	return 0;
}

EZCRTErr zipc_parse_app_link(ZModule_t module, ZBuf_t ibuf, ZBuf_t obuf, ZCRT_EVT_CB cb, void* p1, void* p2)
{
	ZIPCMsgCH ch;
	zstr_t compname;
	zstr_t action;
	EZCRTErr ret=0;
	uint32_t idx=0;

	/*common header*/
	if (!zipc_parse_common_hdr(ibuf, &ch))
	{
		return EZCRTErr_ipcCommonHdrErr;
	}

	do{
		ret = zipc_parse_body_info(ibuf, &compname, &action);
		if (ret!=0) break;
		zipc_parse_init_buf(obuf, ZEIPC_TYPE_LINK);
		zipc_parse_put_reply_commonhdr(obuf, &ch, true);

		if (zstr_buf_cmp(&action,"_hookevt") == 0)
		{
			uint32_t evtid=0;
			if (NULL == zipc_server_get_event_encode(module))
			{
				ret=EZCRTErr_ipcNotSupport;
				break;
			}
			ret = _zipc_link_parase_eventid(ibuf, &evtid);
			if(ret !=0 || evtid==0) break;
			zcrt_event_register(module, 10, evtid, cb, p1, p2);
		}
		else if (zstr_buf_cmp(&action,"_unhookevt") == 0)
		{
			uint32_t evtid=0;
			ret = _zipc_link_parase_eventid(ibuf, &evtid);
			if(ret !=0 || evtid==0) break;
			zcrt_event_unregister(module, evtid, cb, p1, p2);
		}
		idx=0;
	}while(idx!=0);

	if( ret!=EZCRTErr_success)
	{
		zipc_parse_put_reply_errcode(obuf, ret);
	}
	else
	{
		zipc_parse_put_reply_req_result(obuf, 1);
		ztlv_put_int(obuf, 1);
		ztlv_put_uint32(obuf, 1);
	}
	return ret;
}

void zipc_component_setlname( ZIPCComp_t comp, const char* name, uint32_t len)
{
	memset(comp->name, 0, ZCRT_NAME_LEN);
	memcpy(comp->name, name, len>=ZCRT_NAME_LEN?ZCRT_NAME_LEN-1:len);
}

ZIPCComp_t zipc_parase_lookup_comp( ZHash_t complist, const char* compname,uint32_t complen)
{
	ZIPCComp   src;
	if (complist==NULL || compname==NULL)
	{
		return NULL;
	}
	zipc_component_setlname(&src, compname, complen);
	return zcrt_hash_lookup(complist, (void*)&src);
}

ZIPCComp_t zipc_component_new( ZModule_t module, const char* name, ZIPC_CALL call, void* cbs, void* arg1, void* arg2 )
{
	ZIPCComp_t comp=zcrt_malloc(sizeof(ZIPCComp), s_tag);
	memset(comp, 0, sizeof(ZIPCComp));
	memcpy(comp->name, name, strlen(name)>=ZCRT_NAME_LEN?ZCRT_NAME_LEN-1:strlen(name));
	comp->module = module;
	comp->apilist = zcrt_hash_new(32,zipc_component_hash_key, zipc_component_hash_cmp);
	comp->arg1 = arg1;
	comp->arg2 = arg2;
	comp->call = call;
	comp->cbs = cbs;
	return comp;
}

void _zipc_componet_hash_free(void* data)
{
	zipc_component_delete((ZIPCComp_t)data);
}
void zipc_component_delete( ZIPCComp_t comp )
{
	if (comp)
	{
		zcrt_hash_enumerate(comp->apilist, _zipc_componet_hash_free);
		zcrt_hash_delete(comp->apilist);
		comp->apilist = NULL;
		zcrt_free(comp);
	}
}

EZCRTErr zipc_component_register( ZIPCComp_t comp, ZIPCComp_t sub )
{
	if (comp==NULL || sub==NULL)
	{
		return EZCRTErr_paramErr;
	}
	if( zipc_parase_lookup_comp(comp->apilist, sub->name, strlen(sub->name) ) != NULL)
	{
		return EZCRTErr_existed;

	}
	zcrt_hash_add( comp->apilist, (void*)sub);
	return 0;
}

void zipc_parse_init_buf( ZBuf_t buf, uint16_t appType)
{
	ZIPCMsgHdr hdr;
	buf->offset = 0;
	memset(&hdr, 0, sizeof(ZIPCMsgHdr));
	hdr.signature=htons(s_zipc_sig);
	hdr.appType=htons(appType);
	zcrt_buf_append(buf, (int8_t*)&hdr, sizeof(ZIPCMsgHdr));
}
/*
reply common hdr: {sn}
*/
void zipc_parse_put_reply_commonhdr( ZBuf_t buf, ZIPCMsgCH_t ch, bool_t req )
{
	ztlv_put_len(buf, 2);
	ztlv_put_string(buf, "sn");
	ztlv_put_uint32(buf, ch->sn);
	ztlv_put_string(buf, "req");
	ztlv_put_bool(buf, req);
}

void zipc_parse_put_reply_errcode( ZBuf_t buf, EZCRTErr err )
{
	ztlv_put_len(buf, 1);
	ztlv_put_string(buf, "err");
	ztlv_put_int(buf, err);
}

/*reply ret fmt: {ret={...}}*/
void zipc_parse_put_reply_req_result( ZBuf_t buf, int32_t cnt )
{
	ztlv_put_len(buf, 1);

	/*fmt=ret={...}*/
	ztlv_put_string(buf, "ret");
	ztlv_put_table_cnt(buf, cnt);
}

ZBuf_t zipc_buf_malloc(ZIPCModule_t zipc)
{
	ZBuf_t buf=NULL;
	if (zipc==NULL)
	{
		return NULL;
	}

	zcrt_mutex_lock(zipc->mutex);
	if (zipc->freebuf)
	{
		buf = zipc->freebuf;
		zipc->freebuf=buf->next;
	}
	zcrt_mutex_unlock(zipc->mutex);

	if (buf==NULL)
	{
		buf=zcrt_malloc(sizeof(ZBuf), s_tag);
		if (buf==NULL)
		{
			return NULL;
		}
		zcrt_buf_init(buf, ZIPC_BUF_LEN, s_tag);
	}
	zcrt_buf_clear(buf);
	return buf;

}

void zipc_buf_free(ZIPCModule_t zipc, ZBuf_t buf)
{
	if (zipc==NULL || buf == NULL)
	{
		ZCRT_ASSERT(buf&&zipc);
		return;
	}
	zcrt_mutex_lock(zipc->mutex);
	buf->next = zipc->freebuf;
	zipc->freebuf = buf;
	zcrt_mutex_unlock(zipc->mutex);
}

void zipc_encode_common_hdr( ZBuf_t buf, bool_t req, bool_t noti )
{
	ztlv_put_len(buf, 3);
	ztlv_put_string(buf, "sn");
	ztlv_put_uint32(buf, zcrt_unique_id());
	ztlv_put_string(buf, "req");
	ztlv_put_bool(buf, req);
	ztlv_put_string(buf, "noti");
	ztlv_put_bool(buf, noti);
}

EZCRTErr zipc_encode_event(ZModule_t module, uint32_t evtid, void* evtdata, ZBuf_t buf) 
{
	ZCRT_EVENT_ENCODE_FN fn=zipc_server_get_event_encode(module);

	if (fn==NULL)
	{
		return EZCRTErr_ipcNotSupport;
	}

	zipc_parse_init_buf(buf, ZEIPC_TYPE_LINK);
	zipc_encode_common_hdr(buf, false, true);
	/* body {eventid, data}*/
	ztlv_put_len(buf, 2);

	/*the first idx*/
	ztlv_put_int(buf, 1);
	ztlv_put_uint32(buf, evtid);

	/*the second idx*/
	ztlv_put_int(buf, 2);

	/*put data*/
	if (fn(evtid, evtdata, buf)==0 )
	{
		ZIPCMsgHdr_t rhdr = (ZIPCMsgHdr_t)buf->buf;
		rhdr->payloadSize = htonl(buf->len-ZIPC_HEAD_LEN);
		return 0;
	}
	return EZCRTErr_ipcNotSupport;
}
