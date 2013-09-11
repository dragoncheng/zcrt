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
* �¼��ص�����
*  @param evtid �¼�ID��Ŀǰֻ֧��<256
*  @param evtdata �¼���Ϣ
*  @param p1 arg1
*  @param p2 arg2
*/
typedef void(*ZCRT_EVT_CB)(uint32_t evtid, void* evtdata, void* p1, void* p2);

/**
* ��ע�¼�
*  @param module zcrt module
*  @param priority ���ȼ�������Խ�ߣ����ȼ�Խ��
*  @param evtid �¼�ID��Ŀǰֻ֧��<256
*  @param cb callback
*  @param p1 arg1
*  @param p2 arg2
*  @return 0/error code
*/
EZCRTErr zcrt_event_register( ZModule_t module, uint8_t priority, uint32_t evtid, ZCRT_EVT_CB cb, void* p1, void* p2 );

/**
* ȡ���¼���ע
*  @param module zcrt module
*  @param evtid �¼�ID��Ŀǰֻ֧��<256
*  @param cb callback
*  @param p1 arg1
*  @param p2 arg2
*  @return cb,p1��p2,�����ע��ʱһ��
*/
void zcrt_event_unregister( ZModule_t module, uint32_t evtid, ZCRT_EVT_CB cb, void* p1, void* p2 );

/**
* �����¼�
*  @param module zcrt module
*  @param evtid �¼�ID��Ŀǰֻ֧��<256
*  @param evtdata �¼���Ϣ
*/
void zcrt_event_send( ZModule_t module, uint32_t evtid, void* evtdata );

#ifdef __cplusplus
};
#endif

#endif
