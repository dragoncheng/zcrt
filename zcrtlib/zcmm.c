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

static int32_t s_uniqueid=0;
uint32_t g_zcrt_starttime=0;	/*启动时间*/
uint32_t g_zcrt_start_cpu_clock=0;	/*启动时CPU时间，用于计算运行时间*/

uint32_t zcrt_unique_id( void )
{
	return (uint32_t)zcrt_atom_inc(&s_uniqueid);
}

uint32_t zcrt_start_time()
{
	return g_zcrt_starttime;
}

uint32_t zcrt_running_clock(void)
{
	return (uint32_t)(zcrt_cpu_clock()-g_zcrt_start_cpu_clock);
}

#ifndef __KERNEL__
#include <time.h>
uint32_t zcrt_current_time(void)
{
	return (uint32_t)time((time_t*)NULL); 
}
uint32_t zcrt_cpu_clock(void)
{
	return (uint32_t)clock();
}
#endif
