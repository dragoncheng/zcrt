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
#include "zipc/zipc_parse.h"

EZCRTErr zipc_server_register( ZModule_t module, ZIPCComp_t comp)
{
	ZIPCModule_t ipc = NULL;
	if (module==NULL || comp==NULL )
	{
		return EZCRTErr_paramErr;
	}
	ipc = (ZIPCModule_t)module->zipc;
	if( zipc_parase_lookup_comp(ipc->modlist, comp->name, strlen(comp->name) ) != NULL)
	{
		return EZCRTErr_existed;

	}
	zcrt_hash_add( ipc->modlist, (void*)comp);

	return 0;
}

void zipc_server_unregister( ZModule_t module, ZIPCComp_t comp )
{
	ZIPCModule_t ipc = NULL;
	if (module==NULL || comp==NULL)
	{
		return;
	}
	ipc = (ZIPCModule_t)module->zipc;
	zcrt_hash_remove(ipc->modlist, (void*)comp);
}

void zipc_server_set_event_encode( ZModule_t module, ZCRT_EVENT_ENCODE_FN fn )
{
	if (module==NULL)
	{
		return;
	}
	((ZIPCModule_t)module->zipc)->evt_encode = fn ;
}

ZCRT_EVENT_ENCODE_FN zipc_server_get_event_encode( ZModule_t module )
{
	if (module==NULL)
	{
		return NULL;
	}
	return ((ZIPCModule_t)module->zipc)->evt_encode;
}
