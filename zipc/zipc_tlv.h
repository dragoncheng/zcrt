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


#ifndef __zipc_tlv_h__
#define __zipc_tlv_h__

#include "zcrt/zbuf.h"

#ifdef __cplusplus
extern "C"{
#endif

extern uint32_t g_tlv_tag;

/*same with var.lua*/
enum ETlvType
{
	ZTLV_NIL=1,
	ZTLV_FALSE=2,
	ZTLV_TRUE=3,
	ZTLV_INT=4,
	ZTLV_INT64=6,		/*because linux kernel don't support float, so it must a integer*/
	ZTLV_STR=0x11,
	ZTLV_TABLE=0x12,
};

enum ETlvConst
{
	ZTLV_TYPE_LEN=1,
	ZTLV_LEN_LEN=2,
};

#define ZTLV_CHECK_TYPE(ptr, t, ret) \
	if (t != ztlv_get_type((ptr)))\
	{\
		return ret;\
	}

#define ZTLV_CHECK_GET_LEN(ptr, v, ret) if (!ztlv_get_len((ptr),(uint32_t *)&(v))) return ret;

#define ZTLV_CHECK_GET_BOOL(ptr, v, ret) if (!ztlv_get_bool((ptr),&(v))) return ret;
#define ZTLV_CHECK_GET_INT(ptr, v, ret) if (!ztlv_get_int((ptr),(int32_t *)&(v))) return ret;
#define ZTLV_CHECK_GET_STR(ptr, v, ret) if (!ztlv_get_string((ptr),&(v))) return ret;
#define ZTLV_CHECK_GET_INT64(ptr, v, ret) if (!ztlv_get_int64((ptr),&(v))) return ret;

#define ZTLV_CHECK_GET_INT8(ptr, v, ret) \
	{\
		int32_t nv=0;\
		if (!ztlv_get_int((ptr),&nv)) return ret;\
		v=(int8_t)nv;\
	}
#define ZTLV_CHECK_GET_INT16(ptr, v, ret) \
	{\
	int32_t nv=0;\
	if (!ztlv_get_int((ptr),&nv)) return ret;\
	v=(int16_t)nv;\
	}
#define ZTLV_CHECK_GET_UINT8(ptr, v, ret) ZTLV_CHECK_GET_INT8(ptr, v, ret)
#define ZTLV_CHECK_GET_UINT16(ptr, v, ret) ZTLV_CHECK_GET_INT16(ptr, v, ret)
#define ZTLV_CHECK_GET_INT32(ptr, v, ret) ZTLV_CHECK_GET_INT(ptr,v,ret)
#define ZTLV_CHECK_GET_UINT32(ptr, v, ret) ZTLV_CHECK_GET_INT(ptr,v,ret)


uint8_t ztlv_get_type(ZBuf_t buf);
bool_t ztlv_put_type(ZBuf_t buf, uint8_t v);

bool_t ztlv_get_len(ZBuf_t buf, uint32_t* v);
bool_t ztlv_put_len(ZBuf_t buf, uint32_t v);

bool_t ztlv_get_bool(ZBuf_t buf, bool_t* v);
bool_t ztlv_put_bool(ZBuf_t buf, bool_t v);

bool_t ztlv_get_int(ZBuf_t buf, int32_t* v);
bool_t ztlv_put_int(ZBuf_t buf, int32_t v);

bool_t ztlv_get_int64( ZBuf_t buf, int64_t* v );
bool_t ztlv_put_int64( ZBuf_t buf, int64_t v );

bool_t ztlv_get_string(ZBuf_t buf, zstr_t* v);
bool_t ztlv_put_lstring(ZBuf_t buf, const char* v, uint32_t len);

#define ztlv_put_string(buf, v) ztlv_put_lstring(buf, v, (((const char*)(v))==NULL) ? 0:strlen(v));

bool_t ztlv_get_ignore(ZBuf_t buf);


bool_t ztlv_read_var_int(ZBuf_t buf, int32_t* value);
bool_t ztlv_write_var_int(ZBuf_t buf, int32_t value);

bool_t ztlv_get_table_cnt(ZBuf_t buf, uint32_t* cnt);
bool_t ztlv_put_table_cnt(ZBuf_t buf, uint32_t cnt);

#define ztlv_get_uint32(buf,v) ztlv_get_int(buf,(int32_t*)v)
bool_t ztlv_put_uint32(ZBuf_t buf, uint32_t v);

/** the item of array parse*/
typedef bool_t (*ZTLV_TYPE_GET)(ZBuf_t buf, void* v);
typedef bool_t (*ZTLV_TYPE_PUT)(ZBuf_t buf, void* v);
bool_t ztlv_get_array(ZBuf_t buf, ZArrayList arr, ZTLV_TYPE_GET fun);
bool_t ztlv_put_array(ZBuf_t buf, ZArrayList arr, ZTLV_TYPE_PUT fun);

#define ZTLV_GET_ARRAY_BASETYPE(buf, bty, defty, btycall, arrv, ret)\
	{\
		uint32_t cnt=0;\
		uint32_t i=0;\
		int32_t idx=0;\
		defty v=0;\
		if(!ztlv_get_table_cnt(buf, &cnt)) return ret;\
		arrv=zarray_new(cnt, sizeof(bty), 5, g_tlv_tag);\
		if( arrv==NULL) return ret;\
		zarray_setlength(arrv, cnt);\
		for( i=0; i<cnt; i++)\
		{\
			if( !ztlv_get_int(buf, &idx) ) return ret;\
			if( !ztlv_get_##btycall (buf, &v) ) return ret;\
			*(bty*)zarray_get(arrv,i) = (bty)v;\
		}\
	};

#define ZTLV_PUT_ARRAY_BASETYPE(buf, bty, defty, btycall, arrv, ret)\
{\
	uint32_t i=0;\
	defty v=0;\
	ztlv_put_table_cnt(buf, zarray_getlength(arrv));\
	for (i=0; i<zarray_getlength(arrv); i++)\
	{\
		ztlv_put_int(buf, i+1);\
		v=*(bty*)zarray_get(arrv,i);\
		ztlv_put_##btycall(buf,v);\
	}\
};
#define ZTLV_GET_ARRAY_BOOL(buf, arrv, ret) ZTLV_GET_ARRAY_BASETYPE(buf, bool_t, bool_t, bool, arrv, ret)
#define ZTLV_PUT_ARRAY_BOOL(buf, arrv, ret) ZTLV_PUT_ARRAY_BASETYPE(buf, bool_t, bool_t, bool, arrv, ret)

#define ZTLV_GET_ARRAY_INT(buf, arrv, ret, bty) ZTLV_GET_ARRAY_BASETYPE(buf, bty, int32_t, int, arrv, ret)
#define ZTLV_PUT_ARRAY_INT(buf, arrv, ret, bty) ZTLV_PUT_ARRAY_BASETYPE(buf, bty, int32_t, int, arrv, ret)

#define ZTLV_GET_ARRAY_UINT32(buf, arrv, ret) ZTLV_GET_ARRAY_BASETYPE(buf, uint32_t, uint32_t, uint32, arrv, ret)
#define ZTLV_PUT_ARRAY_UINT32(buf, arrv, ret) ZTLV_PUT_ARRAY_BASETYPE(buf, uint32_t, uint32_t, uint32, arrv, ret)

#define ZTLV_GET_ARRAY_INT64(buf, arrv, ret) ZTLV_GET_ARRAY_BASETYPE(buf, int64_t, int64_t, int64_t, arrv, ret)
#define ZTLV_PUT_ARRAY_INT64(buf, arrv, ret) ZTLV_PUT_ARRAY_BASETYPE(buf, int64_t, int64_t, int64_t, arrv, ret)

#define ZTLV_GET_ARRAY_STR(buf, arrv, ret) ZTLV_GET_ARRAY_BASETYPE(buf, str_t, str_t, string, arrv, ret)
#define ZTLV_PUT_ARRAY_STR(buf, arrv, ret) ZTLV_PUT_ARRAY_BASETYPE(buf, str_t, str_t, string, arrv, ret)

#ifdef __cplusplus
};
#endif

#endif
