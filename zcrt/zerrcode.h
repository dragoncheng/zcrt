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


#ifndef __zcrt_err_code_h__
#define __zcrt_err_code_h__

typedef enum
{
	EZCRTErr_success=0,

	EZCRTErr_paramErr=-100,
	EZCRTErr_invalidCrtModule=-99,
	EZCRTErr_full=-98,
	EZCRTErr_noMem=-97,
	EZCRTErr_timeout=-96,
	EZCRTErr_badObject=-95,
	EZCRTErr_sockInitErr=-94,
	EZCRTErr_existed=-93,

	EZCRTErr_ipcCommonHdrErr=-79,
	EZCRTErr_ipcBodyErr=-78,
	EZCRTErr_ipcNotSupport=-77,
	EZCRTErr_ipcNoModule=-76,
	EZCRTErr_ipcNoAct=-75,
	EZCRTErr_ipcTLVErr=-74,
	EZCRTErr_ipcArgsErr=-73,

}EZCRTErr;

#endif
