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
* ����һ��hash����
*  @param size �����ʼ��С
*  @param hash_key ����hash key����
*  @param hash_cmp �Ƚ�����hash node�Ƿ����
*  @return hash����ָ��
*/
ZHash_t zcrt_hash_new(uint32_t size, ZHASH_KEY hash_key, ZHASH_CMP hash_cmp);

/**
* �ͷ�hash����
*  @param hashtable hash����ָ��
*  @note  ��Ҫ���ͷ�hashnode�Ŀռ䣬������zcrt_hash_enumerate���ͷſռ�
*/
void zcrt_hash_delete(ZHash_t hashtable);

/**
* ����һ��hash node
*  @param hashtable ����ָ��
*  @param data  ����ָ�룬�����Ҫ����ռ䣬��Ҫ�Լ��ȷ�����ڴ�
*/
void* zcrt_hash_add(ZHash_t hashtable, void* data);

/**
* ɾ��һ��hash node
*  @param hashtable ����ָ��
*  @param data  ����ָ�룬��Ҫ�Լ��ͷſռ�
*/
void zcrt_hash_remove(ZHash_t hashtable, void* data);

/**
* ����hash node
*  @param hashtable ����ָ��
*  @param data  ����ָ�룬�����ݿ���ֻ����key����
*/
void* zcrt_hash_lookup(ZHash_t hashtable, void* data);
typedef void (*ZIPC_HASH_CB)(void* data);

/**
* ö������hash node
*  @param hashtable ����ָ��
*  @param cb ö�ٻص�����
*/
void  zcrt_hash_enumerate(ZHash_t hashtable, ZIPC_HASH_CB cb);

#ifdef __cplusplus
};
#endif

#endif
