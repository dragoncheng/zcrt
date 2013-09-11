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


#ifndef __ptcrt_buffer_h__
#define __ptcrt_buffer_h__
#include "zcrt/ztype.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct tagZBuf 
{
	uint32_t tag;
	uint32_t maxlen;
	uint32_t len;
	uint32_t offset;
	char* buf;
	struct tagZBuf * next;
}ZBuf,*ZBuf_t;

void zcrt_buf_init(ZBuf_t v, uint32_t len, uint32_t memtag);
void zcrt_buf_destory(ZBuf_t v);
void zcrt_buf_clear(ZBuf_t v);

void* zcrt_buf_get(ZBuf_t v, uint32_t len);
bool_t zcrt_buf_getstr( ZBuf_t v, zstr_t* str, uint32_t len);

uint32_t zcrt_buf_realloc(ZBuf_t v, uint32_t len);

uint32_t zcrt_buf_append(ZBuf_t v, const int8_t* buf, uint32_t len);

#ifdef __cplusplus
};
#endif

#endif
