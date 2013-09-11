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


#ifndef __zcrt_hash_h__
#define __zcrt_hash_h__

#ifdef __cplusplus
extern "C"{
#endif

typedef struct tagZHashNode
{
	uint32_t key;
	void* data;
	struct tagZHashNode* next;
}ZHashNode,*ZHashNode_t;

typedef uint32_t (*ZHASH_KEY)(void*);
typedef int32_t (*ZHASH_CMP)(const void*,const void*);
typedef struct ragZHash
{
	uint32_t size;	/* the size of hash array */
	uint32_t count; /* the total count of hash node */

	ZHashNode_t* index;	/* the ptr of the hash array */
	ZHASH_KEY hash_key; /* the callback to calculate the key of node */
	ZHASH_CMP hash_cmp; /* to compare with two hash node */
}ZHash,*ZHash_t;

#define ZCRT_HASH_KEY(ht) ((ht)->hash_key(data) % (ht)->size)

/**
* 创建一个hash数组
*  @param size 数组初始大小
*  @param hash_key 生成hash key函数
*  @param hash_cmp 比较两个hash node是否相等
*  @return hash数组指针
*/
ZHash_t zcrt_hash_new(uint32_t size, ZHASH_KEY hash_key, ZHASH_CMP hash_cmp);

/**
* 释放hash数组
*  @param hashtable hash数组指针
*  @note  需要先释放hashnode的空间，可以用zcrt_hash_enumerate先释放空间
*/
void zcrt_hash_delete(ZHash_t hashtable);

/**
* 增加一个hash node
*  @param hashtable 数组指针
*  @param data  数据指针，如果需要分配空间，需要自己先分配好内存
*/
void* zcrt_hash_add(ZHash_t hashtable, void* data);

/**
* 删除一个hash node
*  @param hashtable 数组指针
*  @param data  数据指针，需要自己释放空间
*/
void zcrt_hash_remove(ZHash_t hashtable, void* data);

/**
* 查找hash node
*  @param hashtable 数组指针
*  @param data  数据指针，此数据可以只包含key数据
*/
void* zcrt_hash_lookup(ZHash_t hashtable, void* data);
typedef void (*ZIPC_HASH_CB)(void* data);

/**
* 枚举所有hash node
*  @param hashtable 数组指针
*  @param cb 枚举回调函数
*/
void  zcrt_hash_enumerate(ZHash_t hashtable, ZIPC_HASH_CB cb);

#ifdef __cplusplus
};
#endif

#endif
