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
#include "zipc/zipc_tlv.h"

#ifdef WIN32
	#include <windows.h>
#else
	#ifndef __KERNEL__
	#include <netinet/in.h>
	#endif
#endif

uint32_t g_tlv_tag=ZCRT_MAKE_FOURCC('Z','T','L','V');

uint32_t _zcrt_ZigZagEncode32(int32_t n)
{
	return (n << 1) ^ (n >> 31);
}
int32_t _zcrt_ZigZagDecode32(uint32_t n)
{
	return(n >> 1) ^ -(int)(n & 1);
}

uint64_t zcdr_Encode64(int64_t n)
{
	return (n << 1) ^ (n >> 63);
}

int64_t zcdr_Decode64(uint64_t n)
{
	return(n >> 1) ^ -(uint64_t)(n & 1);
}

char* _zcrt_tlv_read_len(char* buf, int limit, uint32_t* value)
{
	char* ptr = buf;
	uint32_t b;
	uint32_t result;
	*value = 0;
	if (0 > limit--)
		return NULL;
	b = *(ptr++);
	result = (b & 0x7F);
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return NULL;
	b = *(ptr++);
	result |= (b & 0x7F) << 7;
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return NULL;
	b = *(ptr++);
	result |= (b & 0x7F) << 14;
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return NULL;
	b = *(ptr++);
	result |= (b & 0x7F) << 21;
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return NULL;
	b = *(ptr++);
	result |= b << 28;
	if (!(b & 0x80))
		goto done;
	return NULL;

done: *value = result;
	return ptr;
}
bool_t ztlv_read_var_int(ZBuf_t buf, int32_t* value)
{
	int32_t v;
	char* off = _zcrt_tlv_read_len(buf->buf+buf->offset, buf->len-buf->offset, (uint32_t*)&v);
	if (off)
	{
		*value = _zcrt_ZigZagDecode32(v);
		buf->offset = off-buf->buf;
		return true;
	}
	return false;
}

int zcdr_ReadLEN64(unsigned char* buf, int limit, uint64_t* value)
{
	unsigned char* ptr = buf;
	unsigned int b;
	unsigned int result0,result1=0,result2=0;
	if (0 > limit--)
		return -1;
	b = *(ptr++);
	result0 = (b & 0x7F);
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return -1;
	b = *(ptr++);
	result0 |= (b & 0x7F) << 7;
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return -1;
	b = *(ptr++);
	result0 |= (b & 0x7F) << 14;
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return -1;
	b = *(ptr++);
	result0 |= (b & 0x7F) << 21;
	if (!(b & 0x80))
		goto done;

	if (0 > limit--)
		return -1;
	b = *(ptr++);
	result1 = (b & 0x7F);
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return -1;
	b = *(ptr++);
	result1 |= (b & 0x7F) << 7;
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return -1;
	b = *(ptr++);
	result1 |= (b & 0x7F) << 14;
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return -1;
	b = *(ptr++);
	result1 |= (b & 0x7F) << 21;
	if (!(b & 0x80))
		goto done;

	b = *(ptr++);
	result2 = (b & 0x7F);
	if (!(b & 0x80))
		goto done;
	if (0 > limit--)
		return -1;
	b = *(ptr++);
	result2 |= (b & 0x7F) << 7;
	if (!(b & 0x80))
		goto done;

	return -1;

done:
	*value = ((uint64_t)(result0)) | (((uint64_t)(result1)) << 28) |	(((uint64_t)(result2)) << 56);
	return ptr-buf;
}

bool_t ztlv_read_var_int64(ZBuf_t buf, int64_t* value)
{
	int64_t v;
	int r = zcdr_ReadLEN64((unsigned char*)(buf->buf+buf->offset), buf->len-buf->offset, (uint64_t*)&v);
	if (r>0)
	{
		*value = zcdr_Decode64(v);
		buf->offset += r;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
//buf len must>=5
uint32_t _zcrt_tlv_write_len(char* buf, uint32_t value)
{
	char* target = (char*) buf;
	target[0] = (unsigned char) (value | 0x80);
	if (value >= (1 << 7))
	{
		target[1] = (unsigned char) ((value >> 7) | 0x80);
		if (value >= (1 << 14))
		{
			target[2] = (unsigned char) ((value >> 14) | 0x80);
			if (value >= (1 << 21))
			{
				target[3] = (unsigned char) ((value >> 21) | 0x80);
				if (value >= (1 << 28))
				{
					target[4] = (unsigned char) (value >> 28);
					return 5;
				}
				else
				{
					target[3] &= 0x7F;
					return 4;
				}
			}
			else
			{
				target[2] &= 0x7F;
				return 3;
			}
		}
		else
		{
			target[1] &= 0x7F;
			return 2;
		}
	}
	else
	{
		target[0] &= 0x7F;
		return 1;
	}
}
bool_t ztlv_write_var_int(ZBuf_t buf, int32_t value)
{
	if( buf==NULL )
	{
		return false;
	}
	zcrt_buf_realloc(buf, buf->len+5);
	buf->len += _zcrt_tlv_write_len(buf->buf+buf->len, _zcrt_ZigZagEncode32(value));
	return true;
}


//////////////////////////////////////////////////////////////////////////
//buf len must>=9
int zcdr_WriteLEN64(unsigned char* buf, uint64_t value)
{
	unsigned char* target = (unsigned char*) buf;
	unsigned int part0 = (unsigned int)(value      );
	unsigned int part1 = (unsigned int)(value >> 28);
	unsigned int part2 = (unsigned int)(value >> 56);

	int size;

	// Here we can't really optimize for small numbers, since the value is
	// split into three parts.  Cheking for numbers < 128, for instance,
	// would require three comparisons, since you'd have to make sure part1
	// and part2 are zero.  However, if the caller is using 64-bit integers,
	// it is likely that they expect the numbers to often be very large, so
	// we probably don't want to optimize for small numbers anyway.  Thus,
	// we end up with a hardcoded binary search tree...
	if (part2 == 0) {
		if (part1 == 0) {
			if (part0 < (1 << 14)) {
				if (part0 < (1 << 7)) {
					size = 1; goto size1;
				} else {
					size = 2; goto size2;
				}
			} else {
				if (part0 < (1 << 21)) {
					size = 3; goto size3;
				} else {
					size = 4; goto size4;
				}
			}
		} else {
			if (part1 < (1 << 14)) {
				if (part1 < (1 << 7)) {
					size = 5; goto size5;
				} else {
					size = 6; goto size6;
				}
			} else {
				if (part1 < (1 << 21)) {
					size = 7; goto size7;
				} else {
					size = 8; goto size8;
				}
			}
		}
	} else {
		if (part2 < (1 << 7)) {
			size = 9; goto size9;
		} else {
			size = 10; goto size10;
		}
	}


size10: target[9] = (unsigned char)((part2 >>  7) | 0x80);
size9 : target[8] = (unsigned char)((part2      ) | 0x80);
size8 : target[7] = (unsigned char)((part1 >> 21) | 0x80);
size7 : target[6] = (unsigned char)((part1 >> 14) | 0x80);
size6 : target[5] = (unsigned char)((part1 >>  7) | 0x80);
size5 : target[4] = (unsigned char)((part1      ) | 0x80);
size4 : target[3] = (unsigned char)((part0 >> 21) | 0x80);
size3 : target[2] = (unsigned char)((part0 >> 14) | 0x80);
size2 : target[1] = (unsigned char)((part0 >>  7) | 0x80);
size1 : target[0] = (unsigned char)((part0      ) | 0x80);

	target[size-1] &= 0x7F;
	return size;
}
bool_t ztlv_write_var_int64(ZBuf_t buf, int64_t value)
{
	if( buf==NULL )
	{
		return false;
	}
	zcrt_buf_realloc(buf, buf->len+10);
	buf->len += zcdr_WriteLEN64((unsigned char*)(buf->buf+buf->len), zcdr_Encode64(value));
	return true;
}

#if 0
/*network endian is big-endian*/
static int zcrt_is_network_endian(void)
{
	int x = 1;
	int e = *(char*) &x;
	return e;
}
#define __ZTLV_INT64_SWAP  (zcrt_is_network_endian()!=0)
#endif

#if 0
static void * doswap(bool_t swap, void *p, size_t n)
{
	if (swap)
	{
		char *a = p;
		int i, j;
		for (i = 0, j = n - 1, n = n / 2; n--; i++, j--)
		{
			char t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
	}
	return p;
}
#endif

uint8_t ztlv_get_type( ZBuf_t buf )
{
	void* data=zcrt_buf_get(buf, 1);
	if(data)
	{
		return *(uint8_t*)data;
	}
	return 0;
}

bool_t ztlv_put_type( ZBuf_t buf, uint8_t v )
{
	zcrt_buf_append(buf, (int8_t*)&v, 1);
	return true;
}

bool_t ztlv_get_bool( ZBuf_t buf, bool_t* v )
{
	uint32_t ty = ztlv_get_type(buf);
	if (v==NULL)
	{
		return false;
	}
	switch (ty)
	{
	case ZTLV_TRUE:
		*v = true;
		break;

	case ZTLV_FALSE:
		*v=false;
		break;

	default:
		return false;
	}
	return true;
}

bool_t ztlv_get_int( ZBuf_t buf, int32_t* v )
{
	uint8_t ty = ztlv_get_type(buf);
	switch (ty)
	{
	case ZTLV_INT:
		return ztlv_read_var_int(buf, v);

	case ZTLV_INT64:
		{
			int64_t  nv=0;
			if( ztlv_read_var_int64(buf, &nv) == true )
			{
				*v=(int32_t)nv;
				return true;
			}
			return false;
		}
	}
	return false;
}


bool_t ztlv_get_string( ZBuf_t buf, zstr_t* v )
{
	ZTLV_CHECK_TYPE(buf, ZTLV_STR, false);
	ztlv_get_len(buf, &(v->len));
	return zcrt_buf_getstr(buf, v, v->len);
}

bool_t ztlv_get_ignore( ZBuf_t buf )
{
	int32_t vi=0;
	zstr_t vs;
	bool_t vb=false;
	vs.len = 0;
	switch (ztlv_get_type(buf))
	{
	case ZTLV_FALSE:
	case ZTLV_TRUE:
		ztlv_get_bool(buf, &vb);
		break;

	case ZTLV_INT:
		ztlv_get_int(buf, &vi);
		break;

	case ZTLV_STR:
		ztlv_get_string(buf, &vs);
		break;

	default:
		return false;
	}
	return true;
}

bool_t ztlv_put_bool( ZBuf_t buf, bool_t v )
{
	uint8_t ty = v ? ZTLV_TRUE:ZTLV_FALSE;
	return ztlv_put_type(buf, ty);
}

bool_t ztlv_put_int( ZBuf_t buf, int32_t v )
{
	if (!ztlv_put_type(buf, ZTLV_INT))
	{
		return false;
	}
	return ztlv_write_var_int(buf, v);
}

bool_t ztlv_put_lstring(ZBuf_t buf, const char* v, uint32_t len)
{
	//type
	if (v==NULL)
	{
		return ztlv_put_type(buf, ZTLV_NIL);
	}

	if (!ztlv_put_type(buf, ZTLV_STR))
	{
		return false;
	}
	ztlv_put_len(buf, len);
	zcrt_buf_append(buf, v, len);
	return true;
}

bool_t ztlv_get_len( ZBuf_t buf, uint32_t* v )
{
	char* off = _zcrt_tlv_read_len(buf->buf+buf->offset, buf->len-buf->offset, v);
	if (off)
	{
		buf->offset = off-buf->buf;
		return true;
	}
	return false;

}

bool_t ztlv_put_len( ZBuf_t buf, uint32_t v )
{
	if( buf==NULL )
	{
		return false;
	}
	zcrt_buf_realloc(buf, buf->len+5);
	buf->len += _zcrt_tlv_write_len(buf->buf+buf->len, v);
	return true;

}

bool_t ztlv_get_table_cnt( ZBuf_t buf, uint32_t* cnt )
{
	ZTLV_CHECK_TYPE(buf, ZTLV_TABLE, false);
	return ztlv_get_len(buf, cnt);
}

bool_t ztlv_put_table_cnt( ZBuf_t buf, uint32_t cnt )
{
	ztlv_put_type(buf, ZTLV_TABLE);
	ztlv_put_len(buf, cnt);
	return true;
}

#define _ZTLV_VAL_IS_INT32(v) ((int64_t)v<=((int64_t)2147483647) && (int64_t)v>=((int64_t)(-2147483647)))
bool_t ztlv_put_uint32( ZBuf_t buf, uint32_t v )
{
	int64_t nv=(int64_t)v;
	if(_ZTLV_VAL_IS_INT32(nv))
	{
		if (!ztlv_put_type(buf, ZTLV_INT))
		{
			return false;
		}
		return ztlv_write_var_int(buf, (int32_t)v);
	}

	if (!ztlv_put_type(buf, ZTLV_INT64))
	{
		return false;
	}
	return ztlv_write_var_int64(buf, v);
}

bool_t ztlv_get_array( ZBuf_t buf, ZArrayList arr, ZTLV_TYPE_GET fun)
{
	uint32_t cnt=0;
	uint32_t i=0;
	int32_t idx=0;
	bool_t ret=true;
	if (fun==NULL || arr==NULL)
	{
		return false;
	}
	if(!ztlv_get_table_cnt(buf, &cnt))
	{
		return false;
	}
	zarray_setlength(arr, cnt);
	for( i=0; i<cnt; i++)
	{
		if( !ztlv_get_int(buf, &idx) )
		{
			ret = false;
			break;
		}
		if( !fun(buf, zarray_get(arr,i)) )
		{
			ret = false;
			break;
		}
	}
	return ret;
}

bool_t ztlv_put_array( ZBuf_t buf, ZArrayList arr, ZTLV_TYPE_PUT fun )
{
	uint32_t i=0;
	if (fun==NULL)
	{
		return false;
	}
	ztlv_put_table_cnt(buf, zarray_getlength(arr));
	for (i=0; i<zarray_getlength(arr); i++)
	{
		ztlv_put_int(buf, i+1);
		if( !fun(buf, zarray_get(arr,i)) )
		{
			return false;
		}
	}
	return true;
}

bool_t ztlv_get_int64( ZBuf_t buf, int64_t* v )
{
	switch (ztlv_get_type(buf))
	{
	case ZTLV_INT:
		{
			int32_t nv=0;
			if (ztlv_read_var_int(buf, &nv) )
			{
				*v = nv;
				return true;
			}
			return false;
		}

	case ZTLV_INT64:
		{
			if (ztlv_read_var_int64(buf, v) )
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

bool_t ztlv_put_int64( ZBuf_t buf, int64_t v )
{
	if (_ZTLV_VAL_IS_INT32(v))
	{
		if (!ztlv_put_type(buf, ZTLV_INT))
		{
			return false;
		}
		return ztlv_write_var_int(buf, (int32_t)v);
	}
	if (!ztlv_put_type(buf, ZTLV_INT64))
	{
		return false;
	}
	return ztlv_write_var_int64(buf, v);
}
