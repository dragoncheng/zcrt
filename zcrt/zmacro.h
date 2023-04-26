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


#ifndef __zcrt_macro_h__
#define __zcrt_macro_h__

/** Make四元组 */
#define ZCRT_MAKE_FOURCC(a,b,c,d)\
	((uint32_t)(uint8_t)(a) | ((uint32_t)(uint8_t)(b) << 8) |   \
	((uint32_t)(uint8_t)(c) << 16) | ((uint32_t)(uint8_t)(d) << 24 ))

/** 地址字节对齐 */
#define ZCRT_ULONG_ALIGN(addr) ((addr)+(((uint32_t)addr)%4))

#define ZCRT_MAKE_VERSION(major,minor,macro) ( ((major)<<24) | ((minor)<<16) | (macro) )
#define ZCRT_CMP_VERSION(v1,v2) 

#ifndef NULL
#define  NULL 0
#endif

#define	 ZCRT_MAX_EVT 256		/* the max event id */
#define  ZCRT_NAME_LEN 64
#define  ZIPC_SVR_DEFAULT_PORT 5000

#define ZCRT_WAIT_FOREVER 0xFFFFFFFF
#define ZCRT_WAIT_NOWAIT (0)

#define ARG_UNUSED(a) ((a)=(a))

/** thread普通优先级 */
#define ZCRT_THREAD_PRI_NOMAL 0

#define ZCRT_LITTLE_ENDIAN 1	/*intel*/
#define ZCRT_BIG_ENDIAN 2		/*ppc*/

/** assert */
#ifndef __KERNEL__
#include <assert.h>
#define  ZCRT_ASSERT(a) assert((a))
#else
#include <linux/kernel.h>
#define ZCRT_ASSERT(expr)\
if(!(expr)) {\
	printk( "Assertion failed! %s,%s,%s,line=%d \n",\
#expr,__FILE__,__FUNCTION__,__LINE__);\
} 
#endif


/** 回调函数支持的最大参数个数*/
#define ZCRT_CB_MAX_ARGS 6

#define ZTIMER_INVALID_CONTAINER 0

/* 永远有效的timer container id */
#define ZTIMER_CONTAINER_CONSTID 0xfff

#endif
