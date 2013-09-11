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

void zcrt_buf_init( ZBuf_t v, uint32_t len, uint32_t memtag)
{
	if (v==NULL)
	{
		return;
	}
	memset(v, 0, sizeof(ZBuf));
	v->tag=memtag;
	if (len != 0)
	{
		v->buf = zcrt_malloc(len, memtag);
		v->maxlen = len;
	}
}

void zcrt_buf_destory( ZBuf_t v )
{
	if (v==NULL || v->buf==NULL)
	{
		return;
	}
	zcrt_free(v->buf);
	memset(v, 0, sizeof(ZBuf));
}

uint32_t zcrt_buf_append( ZBuf_t v, const int8_t* buf, uint32_t len )
{
	uint32_t newlen = 0;
	if (v==NULL || buf==NULL || len==0)
	{
		return 0;
	}
	newlen = v->len+len;
	zcrt_buf_realloc(v, newlen);

	memcpy(v->buf+v->len, buf, len);
	v->len = newlen;
	return len;
}

void* zcrt_buf_get( ZBuf_t v, uint32_t len )
{
	uint32_t off = 0;
	if (v==NULL || len==0)
	{
		return NULL;
	}
	off = v->offset;
	if (off + len> v->len)
	{
		return NULL;
	}
	v->offset += len;
	return (v->buf+off);

}

uint32_t zcrt_buf_realloc( ZBuf_t v, uint32_t len )
{
	uint32_t cap = 1;
	if (v==NULL || len==0)
	{
		return 0;
	}
	if (v->maxlen<len)
	{
		while (cap <= len + 1) 
		{
			cap *= 2;
		}
		v->buf = zcrt_realloc(v->buf, cap, v->tag);
		v->maxlen = len;
	}
	return len;
}

bool_t zcrt_buf_getstr( ZBuf_t v, zstr_t* str, uint32_t len)
{
	str->ptr = v->buf + v->offset;
	str->len = len;
	v->offset += len;
	if (v->offset > v->len)
	{
		str->len = 0;
		str->ptr = NULL;
		v->offset = v->len;
		return false;
	}
	return true;
}

void zcrt_buf_clear( ZBuf_t v )
{
	if (v)
	{
		v->len = 0;
		v->offset = 0;
		v->next = NULL;
	}
}

