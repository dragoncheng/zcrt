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
* 创建一个数组
*  @param size 数组初始大小
*  @param unitsize 每个数组元素大小
*  @param step 重新分配数组内存时，内存增长步长
*  @param p1 memtag 分配数组时模块tag
*  @return 数组指针
*/
ZArrayList zarray_new(uint32_t size, uint32_t unitsize, uint32_t step, uint32_t memtag);

/**
* 释放数组
*  @param v 数组指针
*/
void zarray_delete(ZArrayList v);

/**
* 设置数组长度
*  @param v 数组指针
*  @param len 数组新的长度
*/
void zarray_setlength(ZArrayList v, uint32_t len);

/**
* 获取数组长度
*  @param v 数组指针
*  @return 数组长度
*/
uint32_t zarray_getlength(ZArrayList v);

/**
* 获取数组的某个单元值
*  @param v 数组指针
*  @param idx 数组索引
*  @return 指定索引的buffer
*/
void* zarray_get(ZArrayList v, uint32_t idx);

/**
*  数组长度增加一个，同时返回最后一个buffer
*  @param v 数组指针
*  @return 最后一个buffer
*/
void* zarray_increase(ZArrayList v);

/**
*  数组长度减少一个
*  @param v 数组指针
*/
#define zarray_decrease(v) zarray_setlength(v, zarray_getlength(v)-1)

/**
* array增加一个值
*  @param arr 数组指针
*  @param T  数据类型
*  @param val 值
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
