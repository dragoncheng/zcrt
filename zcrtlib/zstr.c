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

static uint32_t s_tag=ZCRT_MAKE_FOURCC('Z','S','T','R');

int32_t zstr_cmp( zstr_t* s1, zstr_t* s2 )
{
	if (s1->len != s2->len)
	{
		return (int32_t)(s1->len-s2->len);
	}
	return strncmp(s1->ptr, s2->ptr, s1->len);
}

int32_t zstr_buf_cmp( zstr_t* s1, const char* buf )
{
	uint32_t len = strlen(buf);
	if (s1->len != len)
	{
		return (int32_t)(s1->len - len);
	}
	return strncmp(s1->ptr, buf, s1->len);
}

void zstr_new(zstr_t* s1, const char* buf)
{
	s1->len = strlen(buf);
	s1->ptr = zcrt_malloc(s1->len +1, s_tag);
	memset(s1->ptr, 0, s1->len+1);
	memcpy(s1->ptr, buf, s1->len);
}

void zstr_new_len(zstr_t* s1, const char* buf, uint32_t len)
{
	s1->len = len;
	s1->ptr = zcrt_malloc(s1->len +1, s_tag);
	memset(s1->ptr, 0, s1->len+1);
	memcpy(s1->ptr, buf, s1->len);
}

void zstr_delete( zstr_t* s1 )
{
	if (s1!=NULL && s1->ptr!=NULL)
	{
		zcrt_free(s1->ptr);
	}
}
