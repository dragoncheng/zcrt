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

/** ���ȶ�������  */
typedef enum
{
	EZCRTSchType_zcrtThread,		/**<  ����zcrt��thread */
	EZCRTSchType_selfThread,	/**<  ˽��thread */
}EZCRTSchType;

/** ���ȶ������ȼ�  */
typedef enum
{
	EZCRTSchPri_priHigh,	/**<  �����ȼ� */
	EZCRTSchPri_priNormal,	/**<  normal */
	EZCRTSchPri_priLow,		/**<  �����ȼ� */
	EZCRTSchPri_num
}EZCRTPriority;

/**
* �������ȶ���
*  @param module crt module
*  @param name name
*  @param type ��������
*  @param pri  �������ȼ�
*  @param maxsize  ���ȶ������job
*  @return ���ȶ��о��
*/
ZHANDLE_t zcrt_schedule_create(ZModule_t module, const char* name, EZCRTSchType type, EZCRTPriority pri, uint16_t maxsize);
/**
* ɾ�����ȶ���
*  @param sch ���ȶ��о��
*/
void zcrt_schedule_delete(ZHANDLE_t sch);

/**
* ����job
*  @param sch schedule handle
*  @param job callback
*  @param p1 arg1
*  @param p2 arg2
*  @return ���ȶ��о��
*/
EZCRTErr zcrt_schedule_sendjob(ZHANDLE_t sch, ZCRT_CB job, void* p1, void* p2);

/**
* ���������job
*  @param sch schedule handle
*  @param job callback
*  @param argn ��������,�������0
*  @param ... ����
*  @return ���ȶ��о��
*/
EZCRTErr zcrt_schedule_sendjob_argn(ZHANDLE_t sch, ZCRT_CB job, uint32_t argn, ...);

#ifdef __cplusplus
};
#endif

#endif
