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


#ifndef __zcrt_timer_h__
#define __zcrt_timer_h__

#include "zcrt/zmacro.h"
#include "zcrt/zerrcode.h"
#include "zcrt/ztype.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* 添加一个定时器
*  @param module zcrt module
*  @param period 定时器周期，单位:毫秒
*  @param cb callback
*  @param p1 arg1
*  @param p2 arg2
*  @return 0/error code
*  @note 定时器回调函数线程与平台主线程不是同一个线程，故在回调函数中必须立即抛出job，而不是在回调函数中处理逻辑
*/
EZCRTErr zcrt_timer_add(ZModule_t module, uint32_t period, ZCRT_CB cb, void* p1, void* p2 );

/**
* 添加一个定时器,传入参数为变长参数
*  @param module zcrt module
*  @param period 定时器周期，单位:毫秒
*  @param cb callback
*  @param argn 参数个数,必须大于0
*  @param ... 参数
*  @return 0/error code
*  @note 只支持一次性定时器，如果需要再次调用，请再次调用本接口
*  @note 如果需要删除，zcrt_timer_delete接口的p1,p2参数用arg1,arg2代替
*/
EZCRTErr zcrt_timer_add_argn(ZModule_t module, uint32_t period, ZCRT_CB cb, uint32_t argn, ...);

/**
* 添加一个定时器,传入参数为变长参数
*  @param module zcrt module
*  @param cb callback
*  @param p1 arg1
*  @param p2 arg2
*  @return 0/error code
*/
EZCRTErr zcrt_timer_delete(ZModule_t module, ZCRT_CB cb, void* p1, void* p2 );

#ifdef __cplusplus
};
#endif

#endif
