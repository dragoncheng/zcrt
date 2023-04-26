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


#ifndef __zcrt_log_h__
#define __zcrt_log_h__

#ifdef __cplusplus
extern "C"{
#endif

/** log 级别 */
typedef enum
{
	ZELogLvl_FATAL=0,
	ZELogLvl_ALERT,
	ZELogLvl_CRITICAL,
	ZELogLvl_ERROR,
	ZELogLvl_WARNING,
	ZELogLvl_NOTICE,
	ZELogLvl_INFO,
	ZELogLvl_DEBUG,
	ZELogLvl_TOP,
}ZELogLvl;


typedef void* ZLOGPID_t;
/**
* 记录log
*  @param pid thread id,可以传入NULL
*  @param lvl logger level
*  @param fmt 格式化数据
*/
void zcrt_logger(const ZLOGPID_t pid, int lvl,const char* fmt,...);

/**
* 记录字符串log
*  @param pid thread id,可以传入NULL
*  @param lvl logger level
*  @param info 字符串
*/
void zcrt_logger_str(const ZLOGPID_t pid, int lvl,const char*);

#ifndef __KERNEL__
/**
* 记录DEBUG级别log
*/
#define ZLOG_DEBUG(...) zcrt_logger(0,ZELogLvl_DEBUG,__VA_ARGS__)
#define ZLOG_INFO(...) zcrt_logger(0,ZELogLvl_INFO,__VA_ARGS__)
#define ZLOG_WARN(...) zcrt_logger(0,ZELogLvl_WARNING,__VA_ARGS__)
#define ZLOG_ERROR(...) zcrt_logger(0,ZELogLvl_ERROR,__VA_ARGS__)
#define ZLOG_CRITICAL(...) zcrt_logger(0,ZELogLvl_CRITICAL,__VA_ARGS__)
#else
#include <linux/kernel.h> 
#define ZLOG_DEBUG(...) printk(KERN_DEBUG __VA_ARGS__)
#define ZLOG_INFO(...) printk(KERN_INFO __VA_ARGS__)
#define ZLOG_WARN(...) printk(KERN_WARNING __VA_ARGS__)
#define ZLOG_ERROR(...) printk(KERN_ERR __VA_ARGS__)
#define ZLOG_CRITICAL(...) printk(KERN_CRIT __VA_ARGS__)
#endif

#ifdef __cplusplus
};
#endif

#endif

