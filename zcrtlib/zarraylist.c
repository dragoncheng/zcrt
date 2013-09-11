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

ZArrayList zarray_new(uint32_t size, uint32_t unitsize, uint32_t step, uint32_t tag)
{
	ZArrayList v = zcrt_malloc(sizeof (_ZArrayList), tag);

	/* allocate at least one slot */
	if (size == 0) size = 1;

	v->alloced = size;
	v->unitsize = unitsize;
	v->len = 0;
	v->memtag = tag;
	v->step=step;
	v->data = zcrt_malloc(v->unitsize * size, tag);
	return v;
}
void zarray_delete( ZArrayList v )
{
	if (v)
	{
		zcrt_free(v->data);
		zcrt_free(v);
	}
}

void zarray_setlength (ZArrayList v, uint32_t len)
{
	uint32_t nlen=len;
	v->len = len;
	ZCRT_ASSERT(len < 0xfffff);
	if (v->alloced > len)
	{
		return;
	}
	if (len-v->alloced < v->step)
	{
		nlen = v->alloced + v->step;
	}

	v->data = zcrt_realloc(v->data, v->unitsize * nlen, v->memtag);
	memset (v->data + v->alloced*v->unitsize, 0, v->unitsize * (nlen - v->alloced) );
	v->alloced = nlen;
}

uint32_t zarray_getlength( ZArrayList v )
{
	if (v==NULL)
	{
		return 0;
	}
	return v->len;
}
void* zarray_get( ZArrayList v, uint32_t idx )
{
	if (idx >= v->len)
	{
		return NULL;
	}
	return (void*)(v->data + idx*v->unitsize);
}

void* zarray_increase( ZArrayList v )
{
	uint32_t len = zarray_getlength(v);
	zarray_setlength(v, len+1);
	return zarray_get(v, len);
}
