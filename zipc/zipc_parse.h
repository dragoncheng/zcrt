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


#ifndef __zipc_parase_h__
#define __zipc_parase_h__

#include "zcrt/ztype.h"
#include "zcrt/zerrcode.h"
#include "zipc/zipc_server.h"
#include "zcrt/zbuf.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef enum{
	ZEIPC_TYPE_DEFAULT=0,
	ZEIPC_TYPE_LINK,
}ZEIPC_TYPE;

#pragma pack(push, 1) 
typedef struct tagZIPCMsgHdr
{
	uint16_t signature;		/* GL*/
	uint16_t version;
	uint16_t appType;
	uint32_t payloadSize;	/* 后面所有信息长度*/
}ZIPCMsgHdr, *ZIPCMsgHdr_t;

typedef struct tagZIPCMsgCommonHeader
{
	uint32_t sn;
	bool_t req;
	bool_t noti;
}ZIPCMsgCH, *ZIPCMsgCH_t;
#pragma pack(pop) 

#define ZIPC_HEAD_LEN sizeof(ZIPCMsgHdr)

bool_t zipc_parse_check_hdr(ZIPCMsgHdr_t hdr);

EZCRTErr zipc_parse_payload(ZModule_t module, uint16_t appType, ZBuf_t ibuf, ZBuf_t obuf);
EZCRTErr zipc_parse_app_default(ZModule_t module, ZBuf_t ibuf, ZBuf_t obuf);
EZCRTErr zipc_parse_app_link(ZModule_t module, ZBuf_t ibuf, ZBuf_t obuf, ZCRT_EVT_CB cb, void* p1, void* p2);
bool_t zipc_parse_common_hdr(ZBuf_t ibuf, ZIPCMsgCH_t ch);
EZCRTErr zipc_parse_request(ZModule_t module, ZBuf_t ibuf, ZIPCMsgCH_t ch, ZBuf_t obuf);
EZCRTErr zipc_parse_body_info( ZBuf_t ibuf, zstr_t* compname, zstr_t* action );

void zipc_parse_init_buf(ZBuf_t buf, uint16_t appType);
void zipc_parse_put_reply_commonhdr(ZBuf_t buf, ZIPCMsgCH_t ch, bool_t req);
void zipc_parse_put_reply_errcode(ZBuf_t buf, EZCRTErr err);
void zipc_parse_put_reply_req_result(ZBuf_t buf, int32_t cnt);

ZIPCComp_t zipc_parase_lookup_comp( ZHash_t complist, const char* compname,uint32_t complen);

void zipc_component_setlname( ZIPCComp_t comp, const char* name, uint32_t len);

ZIPCComp_t zipc_component_new(ZModule_t module, const char* name, ZIPC_CALL call, void* cbs, void* arg1, void* arg2);
void zipc_component_delete(ZIPCComp_t comp);
EZCRTErr zipc_component_register(ZIPCComp_t comp, ZIPCComp_t sub);

uint32_t zipc_component_hash_key(void* data);
int32_t  zipc_component_hash_cmp(const void* data1,const void* data2);

ZBuf_t zipc_buf_malloc(ZIPCModule_t zipc);
void   zipc_buf_free(ZIPCModule_t zipc, ZBuf_t buf);


/* encode */
void zipc_encode_msg_hdr(ZBuf_t buf, uint16_t appType);
void zipc_encode_common_hdr(ZBuf_t buf, bool_t req, bool_t noti);
EZCRTErr zipc_encode_event(ZModule_t module, uint32_t evtid, void* evtdata, ZBuf_t buf) ;
#ifdef __cplusplus
};
#endif

#endif
