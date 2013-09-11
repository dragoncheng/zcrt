#ifndef __zmdk_test_api_decode_h__
#define __zmdk_test_api_decode_h__
#include "zcrt/zcrt.h"
#include "test_api_decode_def.h"

void    ipc_struct_testStuct1_encode(ZBuf_t obuf, testStuct1* st);
bool_t  ipc_struct_testStuct1_decode(ZBuf_t ibuf, testStuct1* st);
bool_t  ipc_struct_testStuct1_array_encode(ZBuf_t obuf, ZArrayList v);
bool_t  ipc_struct_testStuct1_array_decode(ZBuf_t ibuf, ZArrayList* v);
#define ipc_struct_testStuct1_init(st) memset(&st, 0, sizeof(testStuct1))
void    ipc_struct_testStuct1_destructor(testStuct1* st, bool_t isInput);

void    ipc_struct_testStuct2_encode(ZBuf_t obuf, testStuct2* st);
bool_t  ipc_struct_testStuct2_decode(ZBuf_t ibuf, testStuct2* st);
bool_t  ipc_struct_testStuct2_array_encode(ZBuf_t obuf, ZArrayList v);
bool_t  ipc_struct_testStuct2_array_decode(ZBuf_t ibuf, ZArrayList* v);
#define ipc_struct_testStuct2_init(st) memset(&st, 0, sizeof(testStuct2))
#define ipc_struct_testStuct2_destructor(a,b)

void    ipc_struct_testStuct3_encode(ZBuf_t obuf, testStuct3* st);
bool_t  ipc_struct_testStuct3_decode(ZBuf_t ibuf, testStuct3* st);
bool_t  ipc_struct_testStuct3_array_encode(ZBuf_t obuf, ZArrayList v);
bool_t  ipc_struct_testStuct3_array_decode(ZBuf_t ibuf, ZArrayList* v);
#define ipc_struct_testStuct3_init(st) memset(&st, 0, sizeof(testStuct3))
void    ipc_struct_testStuct3_destructor(testStuct3* st, bool_t isInput);

void    ipc_struct_testStuctIsArray_encode(ZBuf_t obuf, testStuctIsArray* st);
bool_t  ipc_struct_testStuctIsArray_decode(ZBuf_t ibuf, testStuctIsArray* st);
bool_t  ipc_struct_testStuctIsArray_array_encode(ZBuf_t obuf, ZArrayList v);
bool_t  ipc_struct_testStuctIsArray_array_decode(ZBuf_t ibuf, ZArrayList* v);
#define ipc_struct_testStuctIsArray_init(st) memset(&st, 0, sizeof(testStuctIsArray))
void    ipc_struct_testStuctIsArray_destructor(testStuctIsArray* st, bool_t isInput);

void    ipc_struct_testStructDefault_encode(ZBuf_t obuf, testStructDefault* st);
bool_t  ipc_struct_testStructDefault_decode(ZBuf_t ibuf, testStructDefault* st);
bool_t  ipc_struct_testStructDefault_array_encode(ZBuf_t obuf, ZArrayList v);
bool_t  ipc_struct_testStructDefault_array_decode(ZBuf_t ibuf, ZArrayList* v);
#define ipc_struct_testStructDefault_init(st) memset(&st, 0, sizeof(testStructDefault))
void    ipc_struct_testStructDefault_destructor(testStructDefault* st, bool_t isInput);

void    ipc_struct_testStructEnum_encode(ZBuf_t obuf, testStructEnum* st);
bool_t  ipc_struct_testStructEnum_decode(ZBuf_t ibuf, testStructEnum* st);
bool_t  ipc_struct_testStructEnum_array_encode(ZBuf_t obuf, ZArrayList v);
bool_t  ipc_struct_testStructEnum_array_decode(ZBuf_t ibuf, ZArrayList* v);
#define ipc_struct_testStructEnum_init(st) memset(&st, 0, sizeof(testStructEnum))
#define ipc_struct_testStructEnum_destructor(a,b)


#endif

