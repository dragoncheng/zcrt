#include "zcrt/zcrt.h"
#include "test_api_def.h"
#include "test_api_decode_def.h"

#include "test_api_decode.h"

bool_t ipc_struct_testStuct1_decode(ZBuf_t ibuf, testStuct1* st)
{
	uint32_t cnt=0;
	uint32_t i=0;
	uint32_t paramcnt=0;
	zstr_t key;
	memset(st, 0, sizeof(testStuct1));

	if (!ztlv_get_table_cnt(ibuf,&cnt)) return false;
	for (i=0; i<cnt; i++)
	{
		ZTLV_CHECK_GET_STR(ibuf, key, false);
		if (zstr_buf_cmp(&key, "p0")==0)
		{
			ZTLV_CHECK_GET_BOOL(ibuf,st->p0,false);
			paramcnt++;
			continue;
		}
		if (zstr_buf_cmp(&key, "p1")==0)
		{
			ZTLV_CHECK_GET_UINT8(ibuf,st->p1,false);
			paramcnt++;
			continue;
		}
		if (zstr_buf_cmp(&key, "p2")==0)
		{
			ZTLV_CHECK_GET_UINT32(ibuf,st->p2,false);
			paramcnt++;
			continue;
		}
		if (zstr_buf_cmp(&key, "p3")==0)
		{
			ZTLV_CHECK_GET_INT8(ibuf,st->p3,false);
			paramcnt++;
			continue;
		}
		if (zstr_buf_cmp(&key, "p4")==0)
		{
			ZTLV_CHECK_GET_INT32(ibuf,st->p4,false);
			paramcnt++;
			continue;
		}
		if (zstr_buf_cmp(&key, "p5")==0)
		{
			ZTLV_CHECK_GET_STR(ibuf,st->p5,false);
			paramcnt++;
			continue;
		}
		ztlv_get_ignore(ibuf);
	}
	if (paramcnt<6) return false;
	return true;
}

void ipc_struct_testStuct1_encode(ZBuf_t obuf, testStuct1* st)
{
	ztlv_put_table_cnt(obuf, 6);
	ztlv_put_string(obuf, "p0");
	ztlv_put_bool(obuf, st->p0);
	ztlv_put_string(obuf, "p1");
	ztlv_put_int(obuf, st->p1);
	ztlv_put_string(obuf, "p2");
	ztlv_put_uint32(obuf, st->p2);
	ztlv_put_string(obuf, "p3");
	ztlv_put_int(obuf, st->p3);
	ztlv_put_string(obuf, "p4");
	ztlv_put_int(obuf, st->p4);
	ztlv_put_string(obuf, "p5");
	ztlv_put_lstring(obuf, st->p5.ptr, st->p5.len);
}
bool_t ipc_struct_testStuct1_array_decode(ZBuf_t ibuf, ZArrayList* v)
{
	uint32_t _cnt=0;
	uint32_t i=0;
	int32_t _idx=0;
	ZArrayList _arr=NULL;
	if(!ztlv_get_table_cnt(ibuf, &_cnt)) return false;
	_arr = zarray_new(_cnt, sizeof(testStuct1), 5, g_tlv_tag);
	if( _arr==NULL) return false;
	*v = _arr;
	zarray_setlength(_arr, _cnt);
	for( i=0; i<_cnt; i++)
	{
		if( !ztlv_get_int(ibuf, &_idx) ) return false;
		if( !ipc_struct_testStuct1_decode(ibuf, (testStuct1*)zarray_get(_arr,i)) ) return false;
	}
	return true;
}

bool_t ipc_struct_testStuct1_array_encode(ZBuf_t obuf, ZArrayList v)
{
	uint32_t i=0;
	if (v==NULL)
	{
		ztlv_put_table_cnt(obuf, 0);
		return true;
	}
	ztlv_put_table_cnt(obuf, zarray_getlength(v));
	for (i=0; i<zarray_getlength(v); i++)
	{
		ztlv_put_int(obuf, i+1);
		ipc_struct_testStuct1_encode(obuf, (testStuct1*)zarray_get(v,i));
	}
	return true;
}

void ipc_struct_testStuct1_destructor(testStuct1* st, bool_t isInput)
{
	ARG_UNUSED(st);
	ARG_UNUSED(isInput);
	if(!isInput)
	{
		zstr_delete(&(st->p5));

	}
}

bool_t ipc_struct_testStuct2_decode(ZBuf_t ibuf, testStuct2* st)
{
	uint32_t cnt=0;
	uint32_t i=0;
	uint32_t paramcnt=0;
	zstr_t key;
	memset(st, 0, sizeof(testStuct2));

	if (!ztlv_get_table_cnt(ibuf,&cnt)) return false;
	for (i=0; i<cnt; i++)
	{
		ZTLV_CHECK_GET_STR(ibuf, key, false);
		if (zstr_buf_cmp(&key, "p0")==0)
		{
			ZTLV_CHECK_GET_BOOL(ibuf,st->p0,false);
			paramcnt++;
			continue;
		}
		if (zstr_buf_cmp(&key, "p1")==0)
		{
			ZTLV_CHECK_GET_UINT8(ibuf,st->p1,false);
			paramcnt++;
			continue;
		}
		ztlv_get_ignore(ibuf);
	}
	if (paramcnt<2) return false;
	return true;
}

void ipc_struct_testStuct2_encode(ZBuf_t obuf, testStuct2* st)
{
	ztlv_put_table_cnt(obuf, 2);
	ztlv_put_string(obuf, "p0");
	ztlv_put_bool(obuf, st->p0);
	ztlv_put_string(obuf, "p1");
	ztlv_put_int(obuf, st->p1);
}
bool_t ipc_struct_testStuct2_array_decode(ZBuf_t ibuf, ZArrayList* v)
{
	uint32_t _cnt=0;
	uint32_t i=0;
	int32_t _idx=0;
	ZArrayList _arr=NULL;
	if(!ztlv_get_table_cnt(ibuf, &_cnt)) return false;
	_arr = zarray_new(_cnt, sizeof(testStuct2), 5, g_tlv_tag);
	if( _arr==NULL) return false;
	*v = _arr;
	zarray_setlength(_arr, _cnt);
	for( i=0; i<_cnt; i++)
	{
		if( !ztlv_get_int(ibuf, &_idx) ) return false;
		if( !ipc_struct_testStuct2_decode(ibuf, (testStuct2*)zarray_get(_arr,i)) ) return false;
	}
	return true;
}

bool_t ipc_struct_testStuct2_array_encode(ZBuf_t obuf, ZArrayList v)
{
	uint32_t i=0;
	if (v==NULL)
	{
		ztlv_put_table_cnt(obuf, 0);
		return true;
	}
	ztlv_put_table_cnt(obuf, zarray_getlength(v));
	for (i=0; i<zarray_getlength(v); i++)
	{
		ztlv_put_int(obuf, i+1);
		ipc_struct_testStuct2_encode(obuf, (testStuct2*)zarray_get(v,i));
	}
	return true;
}


bool_t ipc_struct_testStuct3_decode(ZBuf_t ibuf, testStuct3* st)
{
	uint32_t cnt=0;
	uint32_t i=0;
	uint32_t paramcnt=0;
	zstr_t key;
	memset(st, 0, sizeof(testStuct3));

	if (!ztlv_get_table_cnt(ibuf,&cnt)) return false;
	for (i=0; i<cnt; i++)
	{
		ZTLV_CHECK_GET_STR(ibuf, key, false);
		if (zstr_buf_cmp(&key, "p0")==0)
		{
			ZTLV_CHECK_GET_BOOL(ibuf,st->p0,false);
			paramcnt++;
			continue;
		}
		if (zstr_buf_cmp(&key, "p1")==0)
		{
			if (!ipc_struct_testStuct2_decode(ibuf,&(st->p1))) return false;
			paramcnt++;
			continue;
		}
		ztlv_get_ignore(ibuf);
	}
	if (paramcnt<2) return false;
	return true;
}

void ipc_struct_testStuct3_encode(ZBuf_t obuf, testStuct3* st)
{
	ztlv_put_table_cnt(obuf, 2);
	ztlv_put_string(obuf, "p0");
	ztlv_put_bool(obuf, st->p0);
	ztlv_put_string(obuf, "p1");
	ipc_struct_testStuct2_encode(obuf,&st->p1);
}
bool_t ipc_struct_testStuct3_array_decode(ZBuf_t ibuf, ZArrayList* v)
{
	uint32_t _cnt=0;
	uint32_t i=0;
	int32_t _idx=0;
	ZArrayList _arr=NULL;
	if(!ztlv_get_table_cnt(ibuf, &_cnt)) return false;
	_arr = zarray_new(_cnt, sizeof(testStuct3), 5, g_tlv_tag);
	if( _arr==NULL) return false;
	*v = _arr;
	zarray_setlength(_arr, _cnt);
	for( i=0; i<_cnt; i++)
	{
		if( !ztlv_get_int(ibuf, &_idx) ) return false;
		if( !ipc_struct_testStuct3_decode(ibuf, (testStuct3*)zarray_get(_arr,i)) ) return false;
	}
	return true;
}

bool_t ipc_struct_testStuct3_array_encode(ZBuf_t obuf, ZArrayList v)
{
	uint32_t i=0;
	if (v==NULL)
	{
		ztlv_put_table_cnt(obuf, 0);
		return true;
	}
	ztlv_put_table_cnt(obuf, zarray_getlength(v));
	for (i=0; i<zarray_getlength(v); i++)
	{
		ztlv_put_int(obuf, i+1);
		ipc_struct_testStuct3_encode(obuf, (testStuct3*)zarray_get(v,i));
	}
	return true;
}

void ipc_struct_testStuct3_destructor(testStuct3* st, bool_t isInput)
{
	ARG_UNUSED(st);
	ARG_UNUSED(isInput);
	ipc_struct_testStuct2_destructor(&st->p1, isInput);
}

bool_t ipc_struct_testStuctIsArray_decode(ZBuf_t ibuf, testStuctIsArray* st)
{
	uint32_t cnt=0;
	uint32_t i=0;
	uint32_t paramcnt=0;
	zstr_t key;
	memset(st, 0, sizeof(testStuctIsArray));

	if (!ztlv_get_table_cnt(ibuf,&cnt)) return false;
	for (i=0; i<cnt; i++)
	{
		ZTLV_CHECK_GET_STR(ibuf, key, false);
		if (zstr_buf_cmp(&key, "p0")==0)
		{
			ZTLV_CHECK_GET_BOOL(ibuf,st->p0,false);
			paramcnt++;
			continue;
		}
		if (zstr_buf_cmp(&key, "pa1")==0)
		{
			if (!ipc_struct_testStuct2_array_decode(ibuf,&(st->pa1))) return false;
			paramcnt++;
			continue;
		}
		ztlv_get_ignore(ibuf);
	}
	if (paramcnt<2) return false;
	return true;
}

void ipc_struct_testStuctIsArray_encode(ZBuf_t obuf, testStuctIsArray* st)
{
	ztlv_put_table_cnt(obuf, 2);
	ztlv_put_string(obuf, "p0");
	ztlv_put_bool(obuf, st->p0);
	ztlv_put_string(obuf, "pa1");
	ipc_struct_testStuct2_array_encode(obuf, st->pa1);
}
bool_t ipc_struct_testStuctIsArray_array_decode(ZBuf_t ibuf, ZArrayList* v)
{
	uint32_t _cnt=0;
	uint32_t i=0;
	int32_t _idx=0;
	ZArrayList _arr=NULL;
	if(!ztlv_get_table_cnt(ibuf, &_cnt)) return false;
	_arr = zarray_new(_cnt, sizeof(testStuctIsArray), 5, g_tlv_tag);
	if( _arr==NULL) return false;
	*v = _arr;
	zarray_setlength(_arr, _cnt);
	for( i=0; i<_cnt; i++)
	{
		if( !ztlv_get_int(ibuf, &_idx) ) return false;
		if( !ipc_struct_testStuctIsArray_decode(ibuf, (testStuctIsArray*)zarray_get(_arr,i)) ) return false;
	}
	return true;
}

bool_t ipc_struct_testStuctIsArray_array_encode(ZBuf_t obuf, ZArrayList v)
{
	uint32_t i=0;
	if (v==NULL)
	{
		ztlv_put_table_cnt(obuf, 0);
		return true;
	}
	ztlv_put_table_cnt(obuf, zarray_getlength(v));
	for (i=0; i<zarray_getlength(v); i++)
	{
		ztlv_put_int(obuf, i+1);
		ipc_struct_testStuctIsArray_encode(obuf, (testStuctIsArray*)zarray_get(v,i));
	}
	return true;
}

void ipc_struct_testStuctIsArray_destructor(testStuctIsArray* st, bool_t isInput)
{
	ARG_UNUSED(st);
	ARG_UNUSED(isInput);
	if (st->pa1 != NULL)
		zarray_delete(st->pa1);
}

bool_t ipc_struct_testStructDefault_decode(ZBuf_t ibuf, testStructDefault* st)
{
	uint32_t cnt=0;
	uint32_t i=0;
	uint32_t paramcnt=0;
	zstr_t key;
	memset(st, 0, sizeof(testStructDefault));
	st->p2 = true;
	st->p3 = 305419896;
	st->p4.ptr = "test";
	st->p4.len = strlen("test");
	st->p5 = 12345;

	if (!ztlv_get_table_cnt(ibuf,&cnt)) return false;
	for (i=0; i<cnt; i++)
	{
		ZTLV_CHECK_GET_STR(ibuf, key, false);
		if (zstr_buf_cmp(&key, "p1")==0)
		{
			ZTLV_CHECK_GET_INT8(ibuf,st->p1,false);
			paramcnt++;
			continue;
		}
		if (zstr_buf_cmp(&key, "p2")==0)
		{
			ZTLV_CHECK_GET_BOOL(ibuf,st->p2,false);
			continue;
		}
		if (zstr_buf_cmp(&key, "p3")==0)
		{
			ZTLV_CHECK_GET_INT32(ibuf,st->p3,false);
			continue;
		}
		if (zstr_buf_cmp(&key, "p4")==0)
		{
			ZTLV_CHECK_GET_STR(ibuf,st->p4,false);
			continue;
		}
		if (zstr_buf_cmp(&key, "p5")==0)
		{
			ZTLV_CHECK_GET_UINT32(ibuf,st->p5,false);
			continue;
		}
		ztlv_get_ignore(ibuf);
	}
	if (paramcnt<1) return false;
	return true;
}

void ipc_struct_testStructDefault_encode(ZBuf_t obuf, testStructDefault* st)
{
	ztlv_put_table_cnt(obuf, 5);
	ztlv_put_string(obuf, "p1");
	ztlv_put_int(obuf, st->p1);
	ztlv_put_string(obuf, "p2");
	ztlv_put_bool(obuf, st->p2);
	ztlv_put_string(obuf, "p3");
	ztlv_put_int(obuf, st->p3);
	ztlv_put_string(obuf, "p4");
	ztlv_put_lstring(obuf, st->p4.ptr, st->p4.len);
	ztlv_put_string(obuf, "p5");
	ztlv_put_uint32(obuf, st->p5);
}
bool_t ipc_struct_testStructDefault_array_decode(ZBuf_t ibuf, ZArrayList* v)
{
	uint32_t _cnt=0;
	uint32_t i=0;
	int32_t _idx=0;
	ZArrayList _arr=NULL;
	if(!ztlv_get_table_cnt(ibuf, &_cnt)) return false;
	_arr = zarray_new(_cnt, sizeof(testStructDefault), 5, g_tlv_tag);
	if( _arr==NULL) return false;
	*v = _arr;
	zarray_setlength(_arr, _cnt);
	for( i=0; i<_cnt; i++)
	{
		if( !ztlv_get_int(ibuf, &_idx) ) return false;
		if( !ipc_struct_testStructDefault_decode(ibuf, (testStructDefault*)zarray_get(_arr,i)) ) return false;
	}
	return true;
}

bool_t ipc_struct_testStructDefault_array_encode(ZBuf_t obuf, ZArrayList v)
{
	uint32_t i=0;
	if (v==NULL)
	{
		ztlv_put_table_cnt(obuf, 0);
		return true;
	}
	ztlv_put_table_cnt(obuf, zarray_getlength(v));
	for (i=0; i<zarray_getlength(v); i++)
	{
		ztlv_put_int(obuf, i+1);
		ipc_struct_testStructDefault_encode(obuf, (testStructDefault*)zarray_get(v,i));
	}
	return true;
}

void ipc_struct_testStructDefault_destructor(testStructDefault* st, bool_t isInput)
{
	ARG_UNUSED(st);
	ARG_UNUSED(isInput);
	if(!isInput)
	{
		zstr_delete(&(st->p4));

	}
}

bool_t ipc_struct_testStructEnum_decode(ZBuf_t ibuf, testStructEnum* st)
{
	uint32_t cnt=0;
	uint32_t i=0;
	uint32_t paramcnt=0;
	zstr_t key;
	memset(st, 0, sizeof(testStructEnum));

	if (!ztlv_get_table_cnt(ibuf,&cnt)) return false;
	for (i=0; i<cnt; i++)
	{
		ZTLV_CHECK_GET_STR(ibuf, key, false);
		if (zstr_buf_cmp(&key, "p1")==0)
		{
			ZTLV_CHECK_GET_INT32(ibuf,st->p1,false);
			paramcnt++;
			continue;
		}
		ztlv_get_ignore(ibuf);
	}
	if (paramcnt<1) return false;
	return true;
}

void ipc_struct_testStructEnum_encode(ZBuf_t obuf, testStructEnum* st)
{
	ztlv_put_table_cnt(obuf, 1);
	ztlv_put_string(obuf, "p1");
	ztlv_put_int(obuf, st->p1);
}
bool_t ipc_struct_testStructEnum_array_decode(ZBuf_t ibuf, ZArrayList* v)
{
	uint32_t _cnt=0;
	uint32_t i=0;
	int32_t _idx=0;
	ZArrayList _arr=NULL;
	if(!ztlv_get_table_cnt(ibuf, &_cnt)) return false;
	_arr = zarray_new(_cnt, sizeof(testStructEnum), 5, g_tlv_tag);
	if( _arr==NULL) return false;
	*v = _arr;
	zarray_setlength(_arr, _cnt);
	for( i=0; i<_cnt; i++)
	{
		if( !ztlv_get_int(ibuf, &_idx) ) return false;
		if( !ipc_struct_testStructEnum_decode(ibuf, (testStructEnum*)zarray_get(_arr,i)) ) return false;
	}
	return true;
}

bool_t ipc_struct_testStructEnum_array_encode(ZBuf_t obuf, ZArrayList v)
{
	uint32_t i=0;
	if (v==NULL)
	{
		ztlv_put_table_cnt(obuf, 0);
		return true;
	}
	ztlv_put_table_cnt(obuf, zarray_getlength(v));
	for (i=0; i<zarray_getlength(v); i++)
	{
		ztlv_put_int(obuf, i+1);
		ipc_struct_testStructEnum_encode(obuf, (testStructEnum*)zarray_get(v,i));
	}
	return true;
}

void testModule_ipc_test_call( int8_t, int8_t, int8_t*);
EZCRTErr ipc_api_testModule_ipc_test_call_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	int32_t param1 = 0;
	int32_t param2 = 0;
	int8_t param3 = 0;
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &param1) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &param2) ) {_err=EZCRTErr_ipcTLVErr; break;};
	testModule_ipc_test_call( (int8_t)param1, (int8_t)param2, &param3 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ztlv_put_int(obuf, param3);
	}while(_cont);
	
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

int8_t testModule_ipc_test_basetype( bool_t, int8_t, int16_t, int32_t, int8_t*, int16_t*, int32_t*, bool_t*);
EZCRTErr ipc_api_testModule_ipc_test_basetype_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	bool_t param0 = 0;
	int32_t param1 = 0;
	int32_t param2 = 0;
	int32_t param3 = 0;
	int8_t param4 = 0;
	int16_t param5 = 0;
	int32_t param6 = 0;
	bool_t param7 = 0;
	int8_t ret = 0;
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_bool(ibuf, &param0) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &param1) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &param2) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &param3) ) {_err=EZCRTErr_ipcTLVErr; break;};
	ret = testModule_ipc_test_basetype( (bool_t)param0, (int8_t)param1, (int16_t)param2, (int32_t)param3, &param4, &param5, &param6, &param7 );
	zipc_parse_put_reply_req_result(obuf, 5);
	ztlv_put_int(obuf, 1);
	ztlv_put_int(obuf, ret);
	ztlv_put_int(obuf, 2);
	ztlv_put_int(obuf, param4);
	ztlv_put_int(obuf, 3);
	ztlv_put_int(obuf, param5);
	ztlv_put_int(obuf, 4);
	ztlv_put_int(obuf, param6);
	ztlv_put_int(obuf, 5);
	ztlv_put_bool(obuf, param7);
	}while(_cont);
	
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

uint8_t testModule_ipc_test_unsignedtype( uint8_t, uint16_t, uint32_t, uint8_t*, uint16_t*, uint32_t*);
EZCRTErr ipc_api_testModule_ipc_test_unsignedtype_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	int32_t p1 = 0;
	int32_t p2 = 0;
	uint32_t p3 = 0;
	uint8_t p4 = 0;
	uint16_t p5 = 0;
	uint32_t p6 = 0;
	uint8_t ret = 0;
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &p1) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &p2) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_uint32(ibuf, &p3) ) {_err=EZCRTErr_ipcTLVErr; break;};
	ret = testModule_ipc_test_unsignedtype( (uint8_t)p1, (uint16_t)p2, (uint32_t)p3, &p4, &p5, &p6 );
	zipc_parse_put_reply_req_result(obuf, 4);
	ztlv_put_int(obuf, 1);
	ztlv_put_int(obuf, ret);
	ztlv_put_int(obuf, 2);
	ztlv_put_int(obuf, p4);
	ztlv_put_int(obuf, 3);
	ztlv_put_int(obuf, p5);
	ztlv_put_int(obuf, 4);
	ztlv_put_uint32(obuf, p6);
	}while(_cont);
	
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

int32_t testModule_ipc_test_number( int32_t, int64_t, int64_t, int32_t*, int64_t*, int64_t*);
EZCRTErr ipc_api_testModule_ipc_test_number_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	int32_t p1 = 0;
	int64_t p2 = 0;
	int64_t p3 = 0;
	int32_t p4 = 0;
	int64_t p5 = 0;
	int64_t p6 = 0;
	int32_t ret = 0;
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &p1) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int64(ibuf, &p2) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int64(ibuf, &p3) ) {_err=EZCRTErr_ipcTLVErr; break;};
	ret = testModule_ipc_test_number( (int32_t)p1, (int64_t)p2, (int64_t)p3, &p4, &p5, &p6 );
	zipc_parse_put_reply_req_result(obuf, 4);
	ztlv_put_int(obuf, 1);
	ztlv_put_int(obuf, ret);
	ztlv_put_int(obuf, 2);
	ztlv_put_int(obuf, p4);
	ztlv_put_int(obuf, 3);
	ztlv_put_int64(obuf, p5);
	ztlv_put_int(obuf, 4);
	ztlv_put_int64(obuf, p6);
	}while(_cont);
	
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_string( zstr_t*, zstr_t*);
EZCRTErr ipc_api_testModule_ipc_test_string_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	zstr_t p1;
	zstr_t p2;
	zstr_init(p1);
	zstr_init(p2);
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_string(ibuf, &(p1)) ) {_err=EZCRTErr_ipcTLVErr; break;};
	testModule_ipc_test_string( &p1, &p2 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ztlv_put_lstring(obuf, p2.ptr, p2.len);
	}while(_cont);
	
	zstr_delete(&(p2));
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_stringnull( zstr_t*, zstr_t*);
EZCRTErr ipc_api_testModule_ipc_test_stringnull_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	zstr_t p1;
	zstr_t p2;
	zstr_init(p1);
	zstr_init(p2);
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_string(ibuf, &(p1)) ) {_err=EZCRTErr_ipcTLVErr; break;};
	testModule_ipc_test_stringnull( &p1, &p2 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ztlv_put_lstring(obuf, p2.ptr, p2.len);
	}while(_cont);
	
	zstr_delete(&(p2));
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_array_int( ZArrayList, ZArrayList, ZArrayList*);
EZCRTErr ipc_api_testModule_ipc_test_array_int_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	ZArrayList p1 = NULL;
	ZArrayList p2 = NULL;
	ZArrayList p3 = NULL;
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	ZTLV_GET_ARRAY_INT(ibuf, p1, EZCRTErr_ipcTLVErr, int8_t);
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	ZTLV_GET_ARRAY_INT(ibuf, p2, EZCRTErr_ipcTLVErr, int32_t);
	testModule_ipc_test_array_int( (ZArrayList)p1, (ZArrayList)p2, &p3 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ZTLV_PUT_ARRAY_INT(obuf, p3, EZCRTErr_ipcTLVErr, int32_t);
	}while(_cont);
	
	if (p1 != NULL)
		zarray_delete(p1);
	if (p2 != NULL)
		zarray_delete(p2);
	if (p3 != NULL)
		zarray_delete(p3);
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_array_mix( ZArrayList, ZArrayList, ZArrayList, ZArrayList*, ZArrayList*, ZArrayList*);
EZCRTErr ipc_api_testModule_ipc_test_array_mix_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	ZArrayList p1 = NULL;
	ZArrayList p2 = NULL;
	ZArrayList p3 = NULL;
	ZArrayList p4 = NULL;
	ZArrayList p5 = NULL;
	ZArrayList p6 = NULL;
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	ZTLV_GET_ARRAY_BOOL(ibuf, p1, EZCRTErr_ipcTLVErr);
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	ZTLV_GET_ARRAY_UINT32(ibuf, p2, EZCRTErr_ipcTLVErr);
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	ZTLV_GET_ARRAY_INT(ibuf, p3, EZCRTErr_ipcTLVErr, int32_t);
	testModule_ipc_test_array_mix( (ZArrayList)p1, (ZArrayList)p2, (ZArrayList)p3, &p4, &p5, &p6 );
	zipc_parse_put_reply_req_result(obuf, 3);
	ztlv_put_int(obuf, 1);
	ZTLV_PUT_ARRAY_BOOL(obuf, p4, EZCRTErr_ipcTLVErr);
	ztlv_put_int(obuf, 2);
	ZTLV_PUT_ARRAY_UINT32(obuf, p5, EZCRTErr_ipcTLVErr);
	ztlv_put_int(obuf, 3);
	ZTLV_PUT_ARRAY_INT(obuf, p6, EZCRTErr_ipcTLVErr, int32_t);
	}while(_cont);
	
	if (p1 != NULL)
		zarray_delete(p1);
	if (p2 != NULL)
		zarray_delete(p2);
	if (p3 != NULL)
		zarray_delete(p3);
	if (p4 != NULL)
		zarray_delete(p4);
	if (p5 != NULL)
		zarray_delete(p5);
	if (p6 != NULL)
		zarray_delete(p6);
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_struct_testStuct1( testStuct1*, testStuct1*);
EZCRTErr ipc_api_testModule_ipc_test_struct_testStuct1_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	testStuct1 p1;
	testStuct1 p2;
	ipc_struct_testStuct1_init(p1);
	ipc_struct_testStuct1_init(p2);
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if(!ipc_struct_testStuct1_decode(ibuf, &p1)) {_err=EZCRTErr_ipcTLVErr; break;};
	testModule_ipc_test_struct_testStuct1( &p1, &p2 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ipc_struct_testStuct1_encode(obuf, &p2);
	}while(_cont);
	
	ipc_struct_testStuct1_destructor(&p1, true);
	ipc_struct_testStuct1_destructor(&p2, false);
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_struct_testStuctAndStruct( testStuct3*, testStuct3*);
EZCRTErr ipc_api_testModule_ipc_test_struct_testStuctAndStruct_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	testStuct3 p1;
	testStuct3 p2;
	ipc_struct_testStuct3_init(p1);
	ipc_struct_testStuct3_init(p2);
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if(!ipc_struct_testStuct3_decode(ibuf, &p1)) {_err=EZCRTErr_ipcTLVErr; break;};
	testModule_ipc_test_struct_testStuctAndStruct( &p1, &p2 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ipc_struct_testStuct3_encode(obuf, &p2);
	}while(_cont);
	
	ipc_struct_testStuct3_destructor(&p1, true);
	ipc_struct_testStuct3_destructor(&p2, false);
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_struct_testStuctArray( ZArrayList, ZArrayList*);
EZCRTErr ipc_api_testModule_ipc_test_struct_testStuctArray_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	ZArrayList p1 = NULL;
	ZArrayList p2 = NULL;
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ipc_struct_testStuct2_array_decode(ibuf, &p1) ) {_err=EZCRTErr_ipcTLVErr; break;};
	testModule_ipc_test_struct_testStuctArray( (ZArrayList)p1, &p2 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ipc_struct_testStuct2_array_encode(obuf, p2);
	}while(_cont);
	
	if (p1 != NULL)
		zarray_delete(p1);
	if (p2 != NULL)
		zarray_delete(p2);
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_struct_testStuctIsArray( testStuctIsArray*, testStuctIsArray*);
EZCRTErr ipc_api_testModule_ipc_test_struct_testStuctIsArray_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	testStuctIsArray p1;
	testStuctIsArray p2;
	ipc_struct_testStuctIsArray_init(p1);
	ipc_struct_testStuctIsArray_init(p2);
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if(!ipc_struct_testStuctIsArray_decode(ibuf, &p1)) {_err=EZCRTErr_ipcTLVErr; break;};
	testModule_ipc_test_struct_testStuctIsArray( &p1, &p2 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ipc_struct_testStuctIsArray_encode(obuf, &p2);
	}while(_cont);
	
	ipc_struct_testStuctIsArray_destructor(&p1, true);
	ipc_struct_testStuctIsArray_destructor(&p2, false);
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_args( ZModule_t, int32_t*, int8_t, int8_t*);
EZCRTErr ipc_api_testModule_ipc_test_args_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	int32_t p1 = 0;
	int8_t p2 = 0;
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &p1) ) {_err=EZCRTErr_ipcTLVErr; break;};
	testModule_ipc_test_args( (ZModule_t)comp->arg1, (int32_t*)comp->arg2, (int8_t)p1, &p2 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ztlv_put_int(obuf, p2);
	}while(_cont);
	
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_strcut_default( testStructDefault*);
EZCRTErr ipc_api_testModule_ipc_test_strcut_default_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	testStructDefault p1;
	ipc_struct_testStructDefault_init(p1);
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if(!ipc_struct_testStructDefault_decode(ibuf, &p1)) {_err=EZCRTErr_ipcTLVErr; break;};
	testModule_ipc_test_strcut_default( &p1 );
	zipc_parse_put_reply_req_result(obuf, 0);
	}while(_cont);
	
	ipc_struct_testStructDefault_destructor(&p1, true);
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

ETest testModule_ipc_test_strcut_enum( ETest, testStructEnum*, ETest*);
EZCRTErr ipc_api_testModule_ipc_test_strcut_enum_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	int32_t p1 = 0;
	testStructEnum p2;
	ETest p3 = 0;
	ETest ret = 0;
	ipc_struct_testStructEnum_init(p2);
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &p1) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if( !ztlv_get_int(ibuf, &_idx) ) {_err=EZCRTErr_ipcTLVErr; break;};
	if(!ipc_struct_testStructEnum_decode(ibuf, &p2)) {_err=EZCRTErr_ipcTLVErr; break;};
	ret = testModule_ipc_test_strcut_enum( (ETest)p1, &p2, &p3 );
	zipc_parse_put_reply_req_result(obuf, 2);
	ztlv_put_int(obuf, 1);
	ztlv_put_int(obuf, ret);
	ztlv_put_int(obuf, 2);
	ztlv_put_int(obuf, p3);
	}while(_cont);
	
	ipc_struct_testStructEnum_destructor(&p2, true);
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

void testModule_ipc_test_event( ZModule_t, int8_t*);
EZCRTErr ipc_api_testModule_ipc_test_event_decode(ZIPCComp_t comp, ZBuf_t ibuf, ZBuf_t obuf)
{
	uint32_t _len = 0;
	int32_t _idx = 0;
	EZCRTErr _err=0;
	bool_t _cont=false;
	int8_t p1 = 0;
	if (!ztlv_get_len(ibuf, &_len)) return EZCRTErr_ipcTLVErr;
	do
	{
	testModule_ipc_test_event( (ZModule_t)comp->arg1, &p1 );
	zipc_parse_put_reply_req_result(obuf, 1);
	ztlv_put_int(obuf, 1);
	ztlv_put_int(obuf, p1);
	}while(_cont);
	
	ARG_UNUSED(comp);
	ARG_UNUSED(_idx);
	return _err;
}

ZIPCComp_t ipc_pkg_testModule_create(ZModule_t module, const char* modname, void* cbs, void* arg1, void* arg2)
{
	ZIPCComp_t comp = zipc_component_new(module, modname, NULL, (void*)cbs, arg1, arg2);
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_call",ipc_api_testModule_ipc_test_call_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_basetype",ipc_api_testModule_ipc_test_basetype_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_unsignedtype",ipc_api_testModule_ipc_test_unsignedtype_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_number",ipc_api_testModule_ipc_test_number_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_string",ipc_api_testModule_ipc_test_string_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_stringnull",ipc_api_testModule_ipc_test_stringnull_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_array_int",ipc_api_testModule_ipc_test_array_int_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_array_mix",ipc_api_testModule_ipc_test_array_mix_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_struct_testStuct1",ipc_api_testModule_ipc_test_struct_testStuct1_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_struct_testStuctAndStruct",ipc_api_testModule_ipc_test_struct_testStuctAndStruct_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_struct_testStuctArray",ipc_api_testModule_ipc_test_struct_testStuctArray_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_struct_testStuctIsArray",ipc_api_testModule_ipc_test_struct_testStuctIsArray_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_args",ipc_api_testModule_ipc_test_args_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_strcut_default",ipc_api_testModule_ipc_test_strcut_default_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_strcut_enum",ipc_api_testModule_ipc_test_strcut_enum_decode, NULL, NULL, NULL));
	zipc_component_register(comp, zipc_component_new(module, "ipc_test_event",ipc_api_testModule_ipc_test_event_decode, NULL, NULL, NULL));
	return comp;
}
