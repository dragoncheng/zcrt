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


#include "zcrt/zcrt.h"

static uint32_t s_tag=ZCRT_MAKE_FOURCC('Z','H','A','S');

ZHash_t zcrt_hash_new( uint32_t size, ZHASH_KEY hash_key, ZHASH_CMP hash_cmp )
{
	ZHash_t ht = NULL;
	if (hash_cmp==NULL || hash_key==NULL)
	{
		return NULL;
	}
	ht = zcrt_malloc(sizeof(ZHash), s_tag);

	memset(ht, 0, sizeof(ZHash));
	ht->index = zcrt_malloc(sizeof(ZHashNode_t)*size, s_tag);
	memset(ht->index, 0, sizeof(ZHashNode_t)*size);
	ht->size = size;
	ht->hash_key = hash_key;
	ht->hash_cmp = hash_cmp;
	return ht;
}

void zcrt_hash_delete( ZHash_t ht )
{
	uint32_t i=0;
	ZHashNode_t node=NULL;
	ZHashNode_t next=NULL;
	if (ht==NULL)
	{
		return;
	}
	for (i=0; i<ht->size; i++)
	{
		node = ht->index[i];
		while(node)
		{
			next = node->next;
			zcrt_free(node);
			node = next;
		}
	}
	zcrt_free(ht->index);
	zcrt_free(ht);
}

ZHashNode_t _zcrt_hash_getnode(ZHash_t ht, void* data)
{
	uint32_t key=0;
	if (ht==NULL || data==NULL)
	{
		return NULL;
	}
	key = ZCRT_HASH_KEY(ht);
	return ht->index[key];
}
void* zcrt_hash_add( ZHash_t ht, void* data )
{
	uint32_t key=0;
	ZHashNode_t nnode=NULL;
	if (ht==NULL || data==NULL)
	{
		return NULL;
	}
	key  = ZCRT_HASH_KEY(ht);
	nnode=zcrt_malloc(sizeof(ZHashNode), s_tag);
	nnode->data = data;
	nnode->next = ht->index[key];
	ht->index[key] = nnode;

	ht->count++;
	return nnode;
}

void zcrt_hash_remove( ZHash_t ht, void* data )
{
	ZHashNode_t node=_zcrt_hash_getnode(ht,data);
	if (node==NULL)
	{
		return;
	}
	if (ht->hash_cmp(node->data,data) == 0)
	{
		uint32_t key  = ZCRT_HASH_KEY(ht);
		ht->index[key] = node->next;
		ht->count--;
		zcrt_free(node);
	}
	else
	{
		ZHashNode_t prev=node;
		while (node && ht->hash_cmp(node->data,data)!=0)
		{
			prev = node;
			node = node->next;
		}
		if (node)
		{
			prev->next = node->next;
			zcrt_free(node);
			ht->count--;
		}
	}
	return;
}

void* zcrt_hash_lookup( ZHash_t ht, void* data )
{
	ZHashNode_t node=_zcrt_hash_getnode(ht,data);
	if (node==NULL)
	{
		return NULL;
	}
	while (node && ht->hash_cmp(node->data,data)!=0)
	{
		node = node->next;
	}
	return node==NULL ? NULL:node->data;
}

void zcrt_hash_enumerate( ZHash_t ht, ZIPC_HASH_CB cb )
{
	uint32_t i=0;
	ZHashNode_t node=NULL;
	if(ht==NULL || cb==NULL)
	{
		return;
	}
	for (i=0; i<ht->size; i++)
	{
		node=ht->index[i];
		while (node)
		{
			cb(node->data);
			node=node->next;
		}
	}
}
