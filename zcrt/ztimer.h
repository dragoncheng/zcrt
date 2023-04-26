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
* ����һ����ʱ������
*  @param module zcrt module
*  @return contianer id/ZTIMER_INVALID_CONTAINER
*/
uint16_t zcrt_timer_container_new( ZModule_t module);

/**
* �ͷ�һ����ʱ������
*  @param module zcrt module
*  @param contianer_id contianer id
*/
void zcrt_timer_container_delete( ZModule_t module, uint16_t container_id);

/**
* ���һ����ʱ��
*  @param module zcrt module
*  @param period ��ʱ�����ڣ���λ:����
*  @param cb callback
*  @param p1 arg1
*  @param p2 arg2
*  @return 0/error code
*  @note ��ʱ���ص������߳���ƽ̨���̲߳���ͬһ���̣߳����ڻص������б��������׳�job���������ڻص������д����߼�
*/
EZCRTErr zcrt_timercon_add(ZModule_t module, uint16_t container_id, uint32_t period, ZCRT_CB cb, void* p1, void* p2 );

/**
* ���һ����ʱ��,�������Ϊ�䳤����
*  @param module zcrt module
*  @param period ��ʱ�����ڣ���λ:����
*  @param cb callback
*  @param argn ��������,�������0
*  @param ... ����
*  @return 0/error code
*  @note ֻ֧��һ���Զ�ʱ���������Ҫ�ٴε��ã����ٴε��ñ��ӿ�
*  @note �����Ҫɾ����zcrt_timer_delete�ӿڵ�p1,p2������arg1,arg2����
*/
EZCRTErr zcrt_timercon_add_argn(ZModule_t module, uint16_t container_id, uint32_t period, ZCRT_CB cb, uint32_t argn, ...);

/**
* ���һ����ʱ��
*  @param module zcrt module
*  @param contianer_id ��ʱ��������֧��ZTIMER_CONTAINER_CONSTID
*  @param period ��ʱ�����ڣ���λ:����
*  @param cb callback
*  @param p1 arg1
*  @param p2 arg2
*  @return 0/error code
*  @note ��ʱ���ص������߳���ƽ̨���̲߳���ͬһ���̣߳����ڻص������б��������׳�job���������ڻص������д����߼�
*/
//EZCRTErr zcrt_timer_add(ZModule_t module, uint16_t container_id, uint32_t period, ZCRT_CB cb, void* p1, void* p2 );
#define zcrt_timer_add(module, period, cb, p1, p2) zcrt_timercon_add(module, ZTIMER_CONTAINER_CONSTID, period, cb, p1, p2)
/**
* ���һ����ʱ��,�������Ϊ�䳤����
*  @param module zcrt module
*  @param contianer_id ��ʱ��������֧��ZTIMER_CONTAINER_CONSTID
*  @param period ��ʱ�����ڣ���λ:����
*  @param cb callback
*  @param argn ��������,�������0
*  @param ... ����
*  @return 0/error code
*  @note ֻ֧��һ���Զ�ʱ���������Ҫ�ٴε��ã����ٴε��ñ��ӿ�
*  @note �����Ҫɾ����zcrt_timer_delete�ӿڵ�p1,p2������arg1,arg2����
*/
//EZCRTErr zcrt_timer_add_argn(ZModule_t module, uint16_t container_id, uint32_t period, ZCRT_CB cb, uint32_t argn, ...);
#define zcrt_timer_add_argn(module, period, cb, argn, ...) zcrt_timercon_add_argn(module, ZTIMER_CONTAINER_CONSTID, period, cb, argn, ...)

/**
* ���һ����ʱ��,�������Ϊ�䳤����
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
