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



#ifndef __zcrt_os_h__
#define __zcrt_os_h__

#include "zcrt/ztype.h"
#include "zcrt/zerrcode.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* ԭ�ӵ���
*  @param size  value pointer
*  @return �������ֵ
*/
int32_t	zcrt_atom_inc(int32_t * cntPtr);
/**
* ԭ�ӵݼ�
*  @param size  value pointer
*  @return �ݼ����ֵ
*/
int32_t	zcrt_atom_dec(int32_t * cntPtr);

/**
* �����ڴ�
*  @param size ��Ҫ�ڴ��С
*  @param tag  ����ģ��id�������ڴ����
*  @return �ڴ��ַ
*/
void* zcrt_malloc(uint32_t size, uint32_t tag);

/**
* �ͷ��ڴ�
*  @param p �ڴ��ַ
*/
void zcrt_free(void* p);

/**
* realloc�ڴ�
*  @param p �ڴ��ַ
*/
void* zcrt_realloc(void* p, uint32_t size, uint32_t tag);

/* �ر�cpu�ж� */
void zcrt_irq_lock( void );
/* ��cpu�ж� */
void zcrt_irq_unlock( void );

/**
* �����ź���
*  @param name name
*  @param initial  ��ʼ��ֵ???
*  @return �ź������
*/
ZHANDLE_t	zcrt_sem_create( char * name, uint16_t initial);
/**
* ɾ���ź���
*  @param h �ź������
*/
void zcrt_sem_delete(ZHANDLE_t h);
/**
* ��ȡ�ź���
*  @param h �ź������
*  @param timeout ��λ��΢��/ ZCRT_WAIT_FOREVER:��Զ�ȴ�/ ZCRT_WAIT_NOWAIT:���ȴ�
*  @return 0:�ɹ� /EZCrtOSRet_timeout:��ʱ/EZCrtOSRet_badObject:��Ч�ź���
*  @note  ��linux�ں��²���ʹ��ZCRT_WAIT_FOREVER�������ᵼ�¡�blocked for more than 120 seconds������
*/
EZCRTErr		zcrt_sem_take(ZHANDLE_t h, uint32_t timeout);
/**
* �ͷ��ź���
*  @param h �ź������
*/
void zcrt_sem_give(ZHANDLE_t h);

/**
* ����������
*  @param name name
*  @return ���������
*/
ZHANDLE_t	zcrt_mutex_create( char * name);
/**
* ɾ��������
*  @param h ���������
*/
void			zcrt_mutex_delete(ZHANDLE_t h);
/**
* ������lock
*  @param h ���������
*/
void			zcrt_mutex_lock(ZHANDLE_t h);
/**
* ������unlock
*  @param h ���������
*/
void			zcrt_mutex_unlock(ZHANDLE_t h);
/**
* ������trylock
*  @param h ���������
*  @return 0:����lock; ~0: �������ط�lock 
*/
EZCRTErr		zcrt_mutex_trylock(ZHANDLE_t h);

/**
* ��ȡ����ϵͳThread
*  @param name name
*  @param ss ջ��С(�Ƽ�<8K)
*  @param prio ���ȼ�
*  @param f ������ָ��
*  @param arg ����
*  @return thread���
*/
ZHANDLE_t	zcrt_thread_create(char *name, int32_t ss, int32_t prio, int (f)(void *), void *arg);

/**
* stop thread
*  @param h thread���
*/
void zcrt_thread_stop(ZHANDLE_t h);

/**
* thread�ͷ���Ҫֹͣ
*  @param h thread���
*  @note ���������Ҫ���ں������У�������ѭ���е��øú�������⣬�����߳��˳�
*  @code
	void testThread(void* data)
	{
		while(!zcrt_thread_should_stop)
		{
			......
			zcrt_sleep(1);
		}
	}
*/
bool_t zcrt_thread_should_stop(ZHANDLE_t h);

/**
* sleep
*  @param msec �ȴ�ʱ�䣬��λ������
*/
void zcrt_sleep(int32_t msec);

/**
* ��ȡһ��ΨһID��
*  @return ΨһID
*/
uint32_t zcrt_unique_id(void);

/**
* ��ȡʱ��Ƶ��
*  @return ʱ��Ƶ��
*/
uint32_t zcrt_get_time_freq(void);

/**
* ��ȡʱ��
*  @param ts ʱ�귵��ֵ
*/
void zcrt_get_time64(int64_t* ts);

/**
* ��ȡϵͳ����ʱ��(��Ԫ1970��1��1�յ�UTCʱ���0ʱ0��0����������������������)
*  @return ts ʱ�귵��ֵ
*/
uint32_t zcrt_start_time(void);

/**
* ��ȡϵͳ��ǰʱ��(��Ԫ1970��1��1�յ�UTCʱ���0ʱ0��0����������������������)
*  @return ts ����ֵ
*/
uint32_t zcrt_current_time(void);

/**
* ϵͳ����ʱ�䳤��
*  @return ts ����ֵ
*/
uint32_t zcrt_running_clock(void);

/**
* ��ȡCPUʱ��,��λms
*  @return ts ����ֵ
*/
uint32_t zcrt_cpu_clock(void);

#ifdef __cplusplus
}
#endif

#endif
