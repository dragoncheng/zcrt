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


#ifndef __zcrt_event_h__
#define __zcrt_event_h__

#include "zcrt/ztype.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* 事件回调函数
*  @param evtid 事件ID，目前只支持<256
*  @param evtdata 事件信息
*  @param p1 arg1
*  @param p2 arg2
*/
typedef void(*ZCRT_EVT_CB)(uint32_t evtid, void* evtdata, void* p1, void* p2);

/**
* 关注事件
*  @param module zcrt module
*  @param priority 优先级，数字越高，优先级越高
*  @param evtid 事件ID，目前只支持<256
*  @param cb callback
*  @param p1 arg1
*  @param p2 arg2
*  @return 0/error code
*/
EZCRTErr zcrt_event_register( ZModule_t module, uint8_t priority, uint32_t evtid, ZCRT_EVT_CB cb, void* p1, void* p2 );

/**
* 取消事件关注
*  @param module zcrt module
*  @param evtid 事件ID，目前只支持<256
*  @param cb callback
*  @param p1 arg1
*  @param p2 arg2
*  @return cb,p1和p2,必须和注册时一致
*/
void zcrt_event_unregister( ZModule_t module, uint32_t evtid, ZCRT_EVT_CB cb, void* p1, void* p2 );

/**
* 发送事件
*  @param module zcrt module
*  @param evtid 事件ID，目前只支持<256
*  @param evtdata 事件信息
*/
void zcrt_event_send( ZModule_t module, uint32_t evtid, void* evtdata );

#ifdef __cplusplus
};
#endif

#endif
