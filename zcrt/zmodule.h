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


#ifndef __zcrt_module_h__
#define __zcrt_module_h__

#include "zcrt/ztype.h"

/**
* 创建一个ZCRT module
*  @param port ipc服务端口号
*  @return 模块句柄
*  @note 建议一个工程在启动时创建一个ZCRT module，后面用户模块使用该module
*  @note 不同ZCRT module之间无交互
*/
ZModule_t zcrt_module_new(uint16_t port);

/**
* 释放ZCRT module
*  @param m 模块句柄
*/
void zcrt_module_delete(ZModule_t m);

/**
* lock ZCRT module
*  @param m 模块句柄
*/
void zcrt_module_lock(ZModule_t m);

/**
* unlock ZCRT module
*  @param m 模块句柄
*/
void zcrt_module_unlock(ZModule_t m);


void zcrt_set_global_module(ZModule_t m);

ZModule_t zcrt_get_global_module( void );

#endif
