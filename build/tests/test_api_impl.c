#include "zcrt/zcrt.h"
#include "test_api_decode_def.h"
#include "test_api_decode.h"

bool_t _test_pb0=false;
bool_t _test_pb1=true;
int _test_pp1=0x12;
int _test_pp2=0xf1ff;
int _test_pp3=-131234;
int _test_pp4=0xfe;
int _test_pp5=0xfffe;
int _test_pp6=0xf2131235;
int pr0=50;


#define ZCRT_STR_NEW(v, str, tag)\
{\
	v=zcrt_malloc(strlen(str)+1, tag);\
	strcpy(v, str);\
}

void testModule_ipc_test_call( int8_t p1, int8_t p2, int8_t* p3)
{
	ZCRT_ASSERT(p1==(int8_t)_test_pp1);
	ZCRT_ASSERT(p2==(int8_t)_test_pp2);
	*p3=(int8_t)_test_pp3;
}
int8_t testModule_ipc_test_basetype( bool_t p0 , int8_t p1 , int16_t p2 , int32_t p3 , int8_t* p4 , int16_t* p5 , int32_t* p6 , bool_t* p7 )
{
	ZCRT_ASSERT(p0==_test_pb0);
	ZCRT_ASSERT(p1==(int8_t)_test_pp1);
	ZCRT_ASSERT(p2==(int16_t)_test_pp2);
	ZCRT_ASSERT(p3==(int32_t)_test_pp3);
	*p4=(int8_t)_test_pp4;
	*p5=(int16_t)_test_pp5;
	*p6=(int32_t)_test_pp6;
	*p7=_test_pb1;
	return (int8_t)pr0;
}

uint8_t testModule_ipc_test_unsignedtype( uint8_t p1 , uint16_t p2 , uint32_t p3 , uint8_t* p4 , uint16_t* p5 , uint32_t* p6 )
{
	ZCRT_ASSERT(p1==(uint8_t)_test_pp1);
	ZCRT_ASSERT(p2==(uint16_t)_test_pp2);
	/*printf("unsigned long p3 is 0x%x\n",p3);*/
	ZCRT_ASSERT(p3==(uint32_t)0xf2131234);
	*p4=(uint8_t)_test_pp4;
	*p5=(uint16_t)_test_pp5;
	*p6=(uint32_t)_test_pp6;
	return (uint8_t)_test_pp1;
}

int32_t pf1=(int32_t)12345;
int32_t pf2=(int32_t)-567;

int64_t pd1=1234567898765;
int64_t pd2=98765432342;

int64_t pii1=1234567890123;
int64_t pii2=9876543210123;

#define ZCRT_NUMBER_EQUEAL(p1,p2) (p1==p2)
int32_t testModule_ipc_test_number( int32_t p1 , int64_t p2 , int64_t p3 , int32_t* p4 , int64_t* p5 , int64_t* p6 )
{
	ZCRT_ASSERT(ZCRT_NUMBER_EQUEAL(p1,pf1));
	ZCRT_ASSERT(ZCRT_NUMBER_EQUEAL(p2,pd1));
	ZCRT_ASSERT(ZCRT_NUMBER_EQUEAL(pd1,p2));
	ZCRT_ASSERT(ZCRT_NUMBER_EQUEAL(pii1,p3));

	//ZCRT_ASSERT(p3==pii1);
	*p4=pf2;
	*p5=pd2;
	*p6=(int64_t)pii2;
	return pf1;
}

void testModule_ipc_test_string( zstr_t* p1 , zstr_t* p2 )
{
	ZCRT_ASSERT(zstr_buf_cmp(p1,"test string")==0);
	zstr_new(p2, "params string");
}

void testModule_ipc_test_stringnull( zstr_t* p1 , zstr_t* p2 )
{
	ZCRT_ASSERT(zstr_buf_cmp(p1,"")==0);
	p2->len = 0;
	p2->ptr = NULL;
}
void testModule_ipc_test_array_int( ZArrayList p1 , ZArrayList p2, ZArrayList* p3 )
{
	uint32_t i=0;
	uint8_t v1;
	uint32_t v2;
	ZArrayList r=NULL;
	
	for(i=0; i<zarray_getlength(p1); i++)
	{
		v1=*(int8_t*)zarray_get(p1, i);
		ZCRT_ASSERT(v1==i+1);
	}
	for(i=0; i<zarray_getlength(p2); i++)
	{
		v2=*(int32_t*)zarray_get(p2, i);
		ZCRT_ASSERT(v2==1000001+i);
	}
	r=zarray_new(10, sizeof(uint32_t), 5, 0);
	zarray_setlength(r, 10);
	for (i=0; i<10; i++)
	{
		*(uint32_t*)zarray_get(r,i) = i+1;
	}
	*p3 = r;
}

/**
* test array
*  @param p1:  (Array[bool_t]) 
*  @param p2:  (Array[uint32_t]) 
*  @param p3:  (Array[int32_t]) 
*  @param p4:  (Array[bool_t]) 
*  @param p5:  (Array[uint32_t]) 
*  @param p6:  (Array[int32_t]) 
*/
void testModule_ipc_test_array_mix( ZArrayList p1 , ZArrayList p2 , ZArrayList p3 , ZArrayList* p4 , ZArrayList* p5 , ZArrayList* p6 )
{
	uint32_t i=0;
	bool_t v1;
	uint32_t v2;
	int32_t v3;
	for(i=0; i<zarray_getlength(p1); i++)
	{
		v1=*(bool_t*)zarray_get(p1, i);
		ZCRT_ASSERT(v1==((i%2)==0));
	}
	for(i=0; i<zarray_getlength(p2); i++)
	{
		v2=*(uint32_t*)zarray_get(p2, i);
		ZCRT_ASSERT(v2==1000001+i);
	}
	for(i=0; i<zarray_getlength(p3); i++)
	{
		v3=*(int32_t*)zarray_get(p3, i);
		ZCRT_ASSERT(ZCRT_NUMBER_EQUEAL((uint32_t)v3, 12345+i));
	}
	*p4=zarray_new(3, sizeof(bool_t), 5, 0);
	for (i=0; i<3; i++)
	{
		ZARRAY_APPEND(*p4, bool_t, ((i%2)!=0));
	}

	*p5=zarray_new(3, sizeof(uint32_t), 5, 0);
	for (i=0; i<4; i++)
	{
		ZARRAY_APPEND(*p5, uint32_t, _test_pp2+i);
	}

	*p6=zarray_new(3, sizeof(int32_t), 5, 0);
	for (i=0; i<4; i++)
	{
		ZARRAY_APPEND(*p6, int32_t, pf2+i);
	}
}

void testModule_ipc_test_struct_testStuct1( testStuct1* p1 , testStuct1* p2 )
{
	memset(p2, 0, sizeof(testStuct1));
	ZCRT_ASSERT(p1->p0==false);
	ZCRT_ASSERT(p1->p1==30);
	ZCRT_ASSERT(p1->p2==0x12345678);
	ZCRT_ASSERT(p1->p3==-12);
	ZCRT_ASSERT(p1->p4==-123456);
	ZCRT_ASSERT(zstr_buf_cmp(&(p1->p5),"test")==0);

	p2->p0 = true;
	p2->p1 = 0xee;
	p2->p2 = 0x87654321;
	p2->p3 = -12;
	p2->p4 = -87654321;
	zstr_new(&(p2->p5), "qwerty");
}

void testModule_ipc_test_struct_testStuctAndStruct( testStuct3* p1 , testStuct3* p2 )
{
	memset(p2, 0, sizeof(testStuct3));
	ZCRT_ASSERT(p1->p0==false);
	ZCRT_ASSERT(p1->p1.p0==true);
	ZCRT_ASSERT(p1->p1.p1==12);
	p2->p0=true;
	p2->p1.p0 = false;
	p2->p1.p1 = 200;
}

void testModule_ipc_test_struct_testStuctArray( ZArrayList p1 , ZArrayList* p2 )
{
	uint32_t i=0;
	testStuct2 *v=NULL;
	for (i=0; i<zarray_getlength(p1); i++)
	{
		v=(testStuct2*)zarray_get(p1,i);
		ZCRT_ASSERT(v->p0==true);
		ZCRT_ASSERT(v->p1==i+1);
	}
	*p2=zarray_new(10, sizeof(testStuct2), 10, 0);
	zarray_setlength(*p2, 10);
	for (i=0; i<10; i++)
	{
		v=(testStuct2*)zarray_get(*p2,i);
		v->p0=false;
		v->p1=(uint8_t)(100+i);
	}
}

void testModule_ipc_test_struct_testStuctIsArray( testStuctIsArray* p1 , testStuctIsArray* p2 )
{
	testStuct2* v=NULL;
	uint32_t i = 0;
	ZCRT_ASSERT(p1->p0==true);
	for (i=0; i<zarray_getlength(p1->pa1); i++)
	{
		v=(testStuct2*)zarray_get(p1->pa1,i);
		ZCRT_ASSERT(v->p0==true);
		ZCRT_ASSERT(v->p1==i+1);
	}
	p2->p0 = true;
	p2->pa1 = zarray_new(10, sizeof(testStuct2), 10, 0);
	zarray_setlength(p2->pa1, 10);
	for (i=0; i<10; i++)
	{
		v=(testStuct2*)zarray_get(p2->pa1,i);
		v->p0=false;
		v->p1=(uint8_t)(100+i);
	}
}

extern ZModule_t g_module;
void testModule_ipc_test_args( ZModule_t module, int32_t* arg1, int8_t p1 , int8_t* p2 )
{
	ZCRT_ASSERT(g_module==module);
	ZCRT_ASSERT(*arg1==1000);
	ZCRT_ASSERT(p1==100);
	*p2=10;
}
void testModule_ipc_test_strcut_default( testStructDefault* p1 )
{
	ZCRT_ASSERT(p1->p1==100);
	ZCRT_ASSERT(p1->p2==true);
	ZCRT_ASSERT(p1->p3==0x12345678);
	ZCRT_ASSERT(zstr_buf_cmp(&(p1->p4),"test")==0);
	ZCRT_ASSERT(p1->p5==1000);
}

ETest testModule_ipc_test_strcut_enum( ETest p1 , testStructEnum* p2 , ETest* p3 )
{
	ZCRT_ASSERT(p1==ETest_two);
	ZCRT_ASSERT(p2->p1==ETest_one);
	*p3=ETest_two;
	return ETest_one;
}

void testModule_ipc_test_event( ZModule_t module, int8_t* p1 )
{
	int32_t evt=111;
	ZCRT_ASSERT(g_module==module);
	zcrt_event_send(module, 2, &evt);
	*p1=200;
}
