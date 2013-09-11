
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"
#include "zcrt/zcrt.h"
#include "test_api_decode.h"
#ifndef WIN32
#include <netinet/in.h>
#else
#include <windows.h>
#endif

static SimpleString* buffer;

static bool_t pb0;
static bool_t pb1;
static int pp1;
static int pp2;
static int pp3;
static int pp4;
static int pp5;
static int pp6;
static int pr0;

TEST_GROUP(TestIpcDecode)
{ 
	ZBuf buf;
	ZBuf obuf;
	ZCRTModule module;
	void setup()
	{
		zcrt_buf_init(&buf, 1024, 100);
		zcrt_buf_init(&obuf, 1024, 100);
		memset(&module, 0, sizeof(ZCRTModule));
	}
	void teardown()
	{
		zcrt_buf_destory(&buf);
		zcrt_buf_destory(&obuf);
	}
};

extern "C" 
{
	int ipc_api_ipc_test_call_decode(ZIPCModule_t m, ZBuf_t ibuf, ZBuf_t obuf);
	int ipc_api_ipc_test_basetype_decode(ZIPCModule_t m, ZBuf_t ibuf, ZBuf_t obuf);

};
static void _init_ipc_hdr(ZBuf_t buf)
{
	/**/
	ZIPCMsgHdr hdr;
	memset( &hdr, 0, sizeof(ZIPCMsgHdr) );
	hdr.signature=htons(('G'<<8) + 'L');
	zcrt_buf_append(buf, (int8_t*)&hdr, sizeof(ZIPCMsgHdr));
}
static void _init_ipc_ch(ZBuf_t buf, const char* nodeid, const char* action)
{
	//common header
	ztlv_put_len(buf, 2);

	ztlv_put_string(buf, "sn");
	ztlv_put_int(buf, 2);

	ztlv_put_string(buf, "req");
	ztlv_put_bool(buf, true);

	//body
	ztlv_put_len(buf, 3);
	
	ztlv_put_int(buf, 1);
	ztlv_put_string(buf, (const str_t)nodeid);
	
	ztlv_put_int(buf, 2);
	ztlv_put_string(buf, (const str_t)action);
}

static void _init_ipc_buf(ZBuf_t buf, const char* nodeid, const char* action)
{
	_init_ipc_ch(buf, nodeid, action);
}

static void _init_ipc_args(ZBuf_t buf, int cnt)
{
	ztlv_put_int(buf, 3);
	ztlv_put_type(buf, ZTLV_TABLE);
	ztlv_put_len(buf, cnt);
}
#if 0
TEST(TestIpcDecode, int_1byte)
{
	pp1=1;
	pp2=2;
	pp3=3;
	int argid=1;
	ztlv_put_len(&buf, 2);
	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp1);
	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp2);

	CHECK_EQUAL(ipc_api_ipc_test_call_decode(NULL, &buf, &obuf), 0);
	int32_t p3=0;
	CHECK_EQUAL(ztlv_get_int(&obuf, &p3), true);
	CHECK_EQUAL(p3, pp3);
}

TEST(TestIpcDecode, basttype)
{
	pb0=false;
	pp1=0x12;
	pp2=0xf1ff;
	pp3=0xf2131234;
	pp4=0xfe;
	pp5=0xfffe;
	pp6=0xfffffff0;
	pr0=50;

	int argid=1;
	ztlv_put_len(&buf, 4);
	ztlv_put_int(&buf, argid++);
	ztlv_put_bool(&buf, pb0);
	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp1);
	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp2);
	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp3);

	CHECK_EQUAL(ipc_api_ipc_test_basetype_decode(NULL, &buf, &obuf), 0);
	int32_t r0;
	int32_t pr=0;
	CHECK_EQUAL(ztlv_get_int(&obuf, &r0), true);
	CHECK_EQUAL(r0, pr0);


	CHECK_EQUAL(ztlv_get_int(&obuf, &pr), true);
	CHECK_EQUAL(pr, (int8_t)pp4);
	CHECK_EQUAL(ztlv_get_int(&obuf, &pr), true);
	CHECK_EQUAL((int16_t)pr, (int16_t)pp5);
	CHECK_EQUAL(ztlv_get_int(&obuf, &pr), true);
	CHECK_EQUAL((int32_t)pr, (int32_t)pp6);

	bool_t rb;
	CHECK_EQUAL(ztlv_get_bool(&obuf, &rb), true);
	CHECK_EQUAL(rb, pb1);
}

TEST(TestIpcDecode, registercomp)
{
	const char* name="test";
	ZIPCModule_t ipc=(ZIPCModule_t)zcrt_malloc(sizeof(ZIPCModule), 0);
	int argid=1;
	memset(ipc, 0, sizeof(ZIPCModule));
	module.zipc = ipc;
	memcpy(ipc->name, name, strlen(name)>=ZCRT_NAME_LEN?ZCRT_NAME_LEN-1:strlen(name));
	ipc->modlist = zcrt_hash_new(128,zipc_component_hash_key, zipc_component_hash_cmp);
	zcrt_hash_add( ipc->modlist, ipc_pkg_test_create(&module,"testapi", NULL, NULL));

	_init_ipc_buf(&buf, "testapi", "ipc_test_call");
	_init_ipc_args(&buf, 2);

	pp1=1;
	pp2=2;
	pp3=3;
	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp1);
	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp2);

	zipc_parse_app_default(&module, &buf, &obuf);

	int32_t p3=0;
	CHECK_EQUAL(ztlv_get_int(&obuf, &p3), true);
	CHECK_EQUAL(p3, pp3);
}

TEST(TestIpcDecode, registercomp_callany)
{
	const char* name="test";
	ZIPCModule_t ipc=(ZIPCModule_t)zcrt_malloc(sizeof(ZIPCModule), 0);
	memset(ipc, 0, sizeof(ZIPCModule));
	module.zipc = ipc;
	memcpy(ipc->name, name, strlen(name)>=ZCRT_NAME_LEN?ZCRT_NAME_LEN-1:strlen(name));
	ipc->modlist = zcrt_hash_new(128,zipc_component_hash_key, zipc_component_hash_cmp);
	zcrt_hash_add( ipc->modlist, ipc_pkg_test_create(&module,"testapi", NULL, NULL));

	pb0=false;
	pp1=0x12;
	pp2=0xf1ff;
	pp3=0xf2131234;
	pp4=0xfe;
	pp5=0xfffe;
	pp6=0xfffffff0;
	pr0=50;
	int argid=0;

	_init_ipc_buf(&buf, "testapi", "ipc_test_basetype");
	_init_ipc_args(&buf, 4);

	ztlv_put_int(&buf, argid++);
	ztlv_put_bool(&buf, pb0);

	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp1);

	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp2);

	ztlv_put_int(&buf, argid++);
	ztlv_put_int(&buf, pp3);

	zipc_parse_app_default(&module, &buf, &obuf);

	int32_t r0;
	int32_t pr=0;
	CHECK_EQUAL(ztlv_get_int(&obuf, &r0), true);
	CHECK_EQUAL(r0, pr0);


	CHECK_EQUAL(ztlv_get_int(&obuf, &pr), true);
	CHECK_EQUAL(pr, (int8_t)pp4);
	CHECK_EQUAL(ztlv_get_int(&obuf, &pr), true);
	CHECK_EQUAL((int16_t)pr, (int16_t)pp5);
	CHECK_EQUAL(ztlv_get_int(&obuf, &pr), true);
	CHECK_EQUAL((int32_t)pr, (int32_t)pp6);

	bool_t rb;
	CHECK_EQUAL(ztlv_get_bool(&obuf, &rb), true);
	CHECK_EQUAL(rb, pb1);
}
#endif
