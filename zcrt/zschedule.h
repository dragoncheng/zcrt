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



#ifndef __zcrt_schedule_h__
#define __zcrt_schedule_h__

#include "zcrt/ztype.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 调度队列类型  */
typedef enum
{
	EZCRTSchType_zcrtThread,		/**<  公用zcrt的thread */
	EZCRTSchType_selfThread,	/**<  私有thread */
}EZCRTSchType;

/** 调度队列优先级  */
typedef enum
{
	EZCRTSchPri_priHigh,	/**<  高优先级 */
	EZCRTSchPri_priNormal,	/**<  normal */
	EZCRTSchPri_priLow,		/**<  低优先级 */
	EZCRTSchPri_num
}EZCRTPriority;

/**
* 创建调度队列
*  @param module crt module
*  @param name name
*  @param type 队列类型
*  @param pri  队列优先级
*  @param maxsize  调度队列最大job
*  @return 调度队列句柄
*/
ZHANDLE_t zcrt_schedule_create(ZModule_t module, const char* name, EZCRTSchType type, EZCRTPriority pri, uint16_t maxsize);
/**
* 删除调度队列
*  @param sch 调度队列句柄
*/
void zcrt_schedule_delete(ZHANDLE_t sch);

/**
* 发送job
*  @param sch schedule handle
*  @param job callback
*  @param p1 arg1
*  @param p2 arg2
*  @return 调度队列句柄
*/
EZCRTErr zcrt_schedule_sendjob(ZHANDLE_t sch, ZCRT_CB job, void* p1, void* p2);

/**
* 多参数发送job
*  @param sch schedule handle
*  @param job callback
*  @param argn 参数个数,必须大于0
*  @param ... 参数
*  @return 调度队列句柄
*/
EZCRTErr zcrt_schedule_sendjob_argn(ZHANDLE_t sch, ZCRT_CB job, uint32_t argn, ...);

#ifdef __cplusplus
};
#endif

#endif
