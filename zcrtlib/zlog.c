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


#include "zcrt/zcrt.h"

#ifndef __KERNEL__
#include <stdarg.h>
#include <stdio.h>
#include <time.h> 
static char* s_lvlname[ZELogLvl_TOP+1]=
{
	"FATAL",
	"ALERT",
	"CRITICAL",
	"ERROR",
	"WARNING",
	"NOTICE",
	"INFO",
	"DEBUG",
	"TOP"
};

void zcrt_logger( const ZLOGPID_t pid, int lvl,const char* fmt,... )
{
	va_list ap;
	time_t timep;
	struct tm p;
	time(&timep);
#ifdef WIN32
	localtime_s(&p, &timep);
#else
	localtime_r(&timep, &p);
#endif
	if (lvl>=0 && lvl<=ZELogLvl_TOP)
	{
		printf("%04d-%02d-%02d %02d:%02d:%02d  ZCRT:[%s] %d ",(1900+p.tm_year),(1+p.tm_mon), p.tm_mday
				,p.tm_hour, p.tm_min, p.tm_sec
				,s_lvlname[lvl], (int)pid);
	}
	else
	{
		printf("%04d-%02d-%02d %02d:%02d:%02d  ZCRT:[%d] %d ",(1900+p.tm_year),(1+p.tm_mon), p.tm_mday
			,p.tm_hour, p.tm_min, p.tm_sec
			,lvl, (int)pid);
	}

	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\n");
}

void zcrt_logger_str( const ZLOGPID_t pid, int lvl,const char* info)
{
	time_t timep;
	struct tm p;
	time(&timep);
#ifdef WIN32
	localtime_s(&p, &timep);
#else
	localtime_r(&timep, &p);
#endif
	if (lvl>=0 && lvl<=ZELogLvl_TOP)
	{
		printf("%04d-%02d-%02d %02d:%02d:%02d  ZCRT:[%s] %d %s\n",(1900+p.tm_year),(1+p.tm_mon), p.tm_mday
			,p.tm_hour, p.tm_min, p.tm_sec
			,s_lvlname[lvl], (int)pid, info);
	}
	else
	{
		printf("%04d-%02d-%02d %02d:%02d:%02d  ZCRT:[%d] %d %s\n",(1900+p.tm_year),(1+p.tm_mon), p.tm_mday
			,p.tm_hour, p.tm_min, p.tm_sec
			,lvl, (int)pid, info);
	}
}
#else
void zcrt_logger( const ZLOGPID_t pid, int lvl,const char* fmt,... )
{
}
void zcrt_logger_str( const ZLOGPID_t pid, int lvl,const char* info)
{
}
#endif

