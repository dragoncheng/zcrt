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
* 原子递增
*  @param size  value pointer
*  @return 递增后的值
*/
int32_t	zcrt_atom_inc(int32_t * cntPtr);
/**
* 原子递减
*  @param size  value pointer
*  @return 递减后的值
*/
int32_t	zcrt_atom_dec(int32_t * cntPtr);

/**
* 分配内存
*  @param size 需要内存大小
*  @param tag  调用模块id，用于内存分析
*  @return 内存地址
*/
void* zcrt_malloc(uint32_t size, uint32_t tag);

/**
* 释放内存
*  @param p 内存地址
*/
void zcrt_free(void* p);

/**
* realloc内存
*  @param p 内存地址
*/
void* zcrt_realloc(void* p, uint32_t size, uint32_t tag);

/* 关闭cpu中断 */
void zcrt_irq_lock( void );
/* 打开cpu中断 */
void zcrt_irq_unlock( void );

/**
* 创建信号量
*  @param name name
*  @param initial  初始化值???
*  @return 信号量句柄
*/
ZHANDLE_t	zcrt_sem_create( char * name, uint16_t initial);
/**
* 删除信号量
*  @param h 信号量句柄
*/
void zcrt_sem_delete(ZHANDLE_t h);
/**
* 获取信号量
*  @param h 信号量句柄
*  @param timeout 单位：微秒/ ZCRT_WAIT_FOREVER:永远等待/ ZCRT_WAIT_NOWAIT:不等待
*  @return 0:成功 /EZCrtOSRet_timeout:超时/EZCrtOSRet_badObject:无效信号量
*  @note  在linux内核下不能使用ZCRT_WAIT_FOREVER，这样会导致“blocked for more than 120 seconds”错误
*/
EZCRTErr		zcrt_sem_take(ZHANDLE_t h, uint32_t timeout);
/**
* 释放信号量
*  @param h 信号量句柄
*/
void zcrt_sem_give(ZHANDLE_t h);

/**
* 创建互斥量
*  @param name name
*  @return 互斥量句柄
*/
ZHANDLE_t	zcrt_mutex_create( char * name);
/**
* 删除互斥量
*  @param h 互斥量句柄
*/
void			zcrt_mutex_delete(ZHANDLE_t h);
/**
* 互斥量lock
*  @param h 互斥量句柄
*/
void			zcrt_mutex_lock(ZHANDLE_t h);
/**
* 互斥量unlock
*  @param h 互斥量句柄
*/
void			zcrt_mutex_unlock(ZHANDLE_t h);
/**
* 互斥量trylock
*  @param h 互斥量句柄
*  @return 0:可以lock; ~0: 被其他地方lock 
*/
EZCRTErr		zcrt_mutex_trylock(ZHANDLE_t h);

/**
* 获取操作系统Thread
*  @param name name
*  @param ss 栈大小(推荐<8K)
*  @param prio 优先级
*  @param f 处理函数指针
*  @param arg 参数
*  @return thread句柄
*/
ZHANDLE_t	zcrt_thread_create(char *name, int32_t ss, int32_t prio, int (f)(void *), void *arg);

/**
* stop thread
*  @param h thread句柄
*/
void zcrt_thread_stop(ZHANDLE_t h);

/**
* thread释放需要停止
*  @param h thread句柄
*  @note 如果代码需要在内核中运行，必须在循环中调用该函数来检测，用于线程退出
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
*  @param msec 等待时间，单位：毫秒
*/
void zcrt_sleep(int32_t msec);

/**
* 获取一个唯一ID号
*  @return 唯一ID
*/
uint32_t zcrt_unique_id(void);

/**
* 获取时标频率
*  @return 时标频率
*/
uint32_t zcrt_get_time_freq(void);

/**
* 获取时标
*  @param ts 时标返回值
*/
void zcrt_get_time64(int64_t* ts);

/**
* 获取系统启动时间(公元1970年1月1日的UTC时间从0时0分0秒算起到现在所经过的秒数)
*  @return ts 时标返回值
*/
uint32_t zcrt_start_time(void);

/**
* 获取系统当前时间(公元1970年1月1日的UTC时间从0时0分0秒算起到现在所经过的秒数)
*  @return ts 返回值
*/
uint32_t zcrt_current_time(void);

/**
* 系统运行时间长度
*  @return ts 返回值
*/
uint32_t zcrt_running_clock(void);

/**
* 获取CPU时间,单位ms
*  @return ts 返回值
*/
uint32_t zcrt_cpu_clock(void);

#ifdef __cplusplus
}
#endif

#endif
