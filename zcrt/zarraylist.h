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

#ifndef __zcrt_array_list__
#define __zcrt_array_list__

#include "ztype.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagZArrayList
{
	uint32_t len;			/* number of active elem */
	uint32_t unitsize;		/* the sizeof the elem */
	uint32_t alloced;		/* number of allocated elem */
	uint32_t memtag;		/* memory tag*/
	uint32_t step;			/* realloc for step */
	int8_t *data;			/* data */
}_ZArrayList,*ZArrayList;

/**
* ����һ������
*  @param size �����ʼ��С
*  @param unitsize ÿ������Ԫ�ش�С
*  @param step ���·��������ڴ�ʱ���ڴ���������
*  @param p1 memtag ��������ʱģ��tag
*  @return ����ָ��
*/
ZArrayList zarray_new(uint32_t size, uint32_t unitsize, uint32_t step, uint32_t memtag);

/**
* �ͷ�����
*  @param v ����ָ��
*/
void zarray_delete(ZArrayList v);

/**
* �������鳤��
*  @param v ����ָ��
*  @param len �����µĳ���
*/
void zarray_setlength(ZArrayList v, uint32_t len);

/**
* ��ȡ���鳤��
*  @param v ����ָ��
*  @return ���鳤��
*/
uint32_t zarray_getlength(ZArrayList v);

/**
* ��ȡ�����ĳ����Ԫֵ
*  @param v ����ָ��
*  @param idx ��������
*  @return ָ��������buffer
*/
void* zarray_get(ZArrayList v, uint32_t idx);

/**
*  ���鳤������һ����ͬʱ�������һ��buffer
*  @param v ����ָ��
*  @return ���һ��buffer
*/
void* zarray_increase(ZArrayList v);

/**
*  ���鳤�ȼ���һ��
*  @param v ����ָ��
*/
#define zarray_decrease(v) zarray_setlength(v, zarray_getlength(v)-1)

/**
* array����һ��ֵ
*  @param arr ����ָ��
*  @param T  ��������
*  @param val ֵ
*/#define ZARRAY_APPEND(arr, T, val)\
	{\
		uint32_t idx=zarray_getlength(arr);\
		zarray_setlength(arr, idx+1);\
		*(T*)zarray_get(arr,idx) = val;\
	};

#ifdef __cplusplus
};
#endif

#endif
