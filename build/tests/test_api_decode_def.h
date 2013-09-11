#include "zcrt/zcrt.h"

#ifndef __zmdk_testModule_h__
#define __zmdk_testModule_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "test_api_def.h"
/**  */
typedef struct
{
	bool_t	p0;		/* bool param */
	uint8_t	p1;		/* uint8 param */
	uint32_t	p2;		/* uint32 param */
	int8_t	p3;		/* int8 param */
	int32_t	p4;		/* int32 param */
	zstr_t	p5;		/* str param */
}testStuct1;

/**  */
typedef struct
{
	bool_t	p0;		/* bool param */
	uint8_t	p1;		/* uint8 param */
}testStuct2;

/**  */
typedef struct
{
	bool_t	p0;		/* bool param */
	testStuct2	p1;		/* testStuct2 param */
}testStuct3;

/**  */
typedef struct
{
	bool_t	p0;		/* bool param */
	ZArrayList	pa1;		/* (Array[testStuct2]) testStuct2 array */
}testStuctIsArray;

/**  */
typedef struct
{
	int8_t	p1;		/*  */
	bool_t	p2;		/*  */
	int32_t	p3;		/*  */
	zstr_t	p4;		/*  */
	uint32_t	p5;		/*  */
}testStructDefault;

/**  */
typedef struct
{
	ETest	p1;		/*  */
}testStructEnum;


/**
* test base call
*  @param param1:  int8 type
*  @param param2:  第二个
*  @param param3:  
*/
void testModule_ipc_test_call( int8_t param1 , int8_t param2 , int8_t* param3 );

/**
* test base type, no include unsinged
*  @param param0:  
*  @param param1:  
*  @param param2:  
*  @param param3:  
*  @param param4:  
*  @param param5:  
*  @param param6:  
*  @param param7:  
*  @return 
*/
int8_t testModule_ipc_test_basetype( bool_t param0 , int8_t param1 , int16_t param2 , int32_t param3 , int8_t* param4 , int16_t* param5 , int32_t* param6 , bool_t* param7 );

/**
* 测试unsigned类型
*  @param p1:  
*  @param p2:  
*  @param p3:  
*  @param p4:  
*  @param p5:  
*  @param p6:  
*  @return 
*/
uint8_t testModule_ipc_test_unsignedtype( uint8_t p1 , uint16_t p2 , uint32_t p3 , uint8_t* p4 , uint16_t* p5 , uint32_t* p6 );

/**
* 测试int64
*  @param p1:  
*  @param p2:  
*  @param p3:  
*  @param p4:  
*  @param p5:  
*  @param p6:  
*  @return 
*/
int32_t testModule_ipc_test_number( int32_t p1 , int64_t p2 , int64_t p3 , int32_t* p4 , int64_t* p5 , int64_t* p6 );

/**
* 测试string
*  @param p1:  
*  @param p2:  
*/
void testModule_ipc_test_string( zstr_t* p1 , zstr_t* p2 );

/**
* 测试string space and null
*  @param p1:  
*  @param p2:  
*/
void testModule_ipc_test_stringnull( zstr_t* p1 , zstr_t* p2 );

/**
* test array
*  @param p1:  (Array[int8_t]) 
*  @param p2:  (Array[int32_t]) 
*  @param p3:  (Array[int32_t]) 
*/
void testModule_ipc_test_array_int( ZArrayList p1 , ZArrayList p2 , ZArrayList* p3 );

/**
* test array
*  @param p1:  (Array[bool_t]) 
*  @param p2:  (Array[uint32_t]) 
*  @param p3:  (Array[int32_t]) 
*  @param p4:  (Array[bool_t]) 
*  @param p5:  (Array[uint32_t]) 
*  @param p6:  (Array[int32_t]) 
*/
void testModule_ipc_test_array_mix( ZArrayList p1 , ZArrayList p2 , ZArrayList p3 , ZArrayList* p4 , ZArrayList* p5 , ZArrayList* p6 );

/**
* testStuct1
*  @param p1:  
*  @param p2:  
*/
void testModule_ipc_test_struct_testStuct1( testStuct1* p1 , testStuct1* p2 );

/**
* testStuctAndStruct
*  @param p1:  
*  @param p2:  
*/
void testModule_ipc_test_struct_testStuctAndStruct( testStuct3* p1 , testStuct3* p2 );

/**
* testStuctArray
*  @param p1:  (Array[testStuct2]) 
*  @param p2:  (Array[testStuct2]) 
*/
void testModule_ipc_test_struct_testStuctArray( ZArrayList p1 , ZArrayList* p2 );

/**
* testStuctIsArray
*  @param p1:  
*  @param p2:  
*/
void testModule_ipc_test_struct_testStuctIsArray( testStuctIsArray* p1 , testStuctIsArray* p2 );

/**
* test args
*  @param module:  
*  @param arg1:  
*  @param p1:  
*  @param p2:  
*/
void testModule_ipc_test_args( ZModule_t module, int32_t* arg1, int8_t p1 , int8_t* p2 );

/**
* test struct default
*  @param p1:  
*/
void testModule_ipc_test_strcut_default( testStructDefault* p1 );

/**
* test enum and struct enum
*  @param p1:  
*  @param p2:  
*  @param p3:  
*  @return 
*/
ETest testModule_ipc_test_strcut_enum( ETest p1 , testStructEnum* p2 , ETest* p3 );

/**
* test event
*  @param module:  
*  @param p1:  
*/
void testModule_ipc_test_event( ZModule_t module, int8_t* p1 );

ZIPCComp_t ipc_pkg_testModule_create(ZModule_t module, const char* modname, void* cbs, void* arg1, void* arg2);
#define ipc_pkg_testModule_delete(comp) zipc_component_delete(comp)

#ifdef __cplusplus
};
#endif
#endif

