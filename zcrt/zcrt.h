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


#ifndef __zcrt_inc_h__
#define __zcrt_inc_h__

#ifndef __KERNEL__
#include <string.h>
#include <memory.h>
#else
#include <linux/string.h>
#endif

#include "zcrt/zmacro.h"
#include "zcrt/zerrcode.h"
#include "zcrt/ztype.h"

#include "zcrt/zlog.h"

#include "zcrt/zcrt_os.h"
#include "zcrt/zschedule.h"
#include "zcrt/ztimer.h"
#include "zcrt/zevent.h"
#include "zcrt/zmodule.h"

#include "zcrt/zarraylist.h"
#include "zcrt/zlinklist.h"
#include "zcrt/zhash.h"
#include "zcrt/zbuf.h"
#include "zcrt/zstr.h"

#include "zipc/zipc_server.h"
#include "zipc/zipc_tlv.h"
#include "zipc/zipc_parse.h"

#endif
