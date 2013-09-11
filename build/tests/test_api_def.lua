package.path=package.path..';../../../tools/?.lua'

local zipctype=require 'zipcpkg.zipctype'
local zipcsvr2c=require 'zipcpkg.zipcsvr2c'
local zipc2h=require 'zipcpkg.zipc2h'

local STRUCT_DEF=zipctype.STRUCT_DEF
local API_DEF=zipctype.API_DEF
local C_SVR_HEAD=zipctype.C_SVR_HEAD
C_SVR_HEAD([[
#include "test_api_def.h"
]])

API_DEF('testModule','ipc_test_call'){
	doc='test base call',
	ret={'void', doc='no return'},
	{'int8_t', 'param1', doc='int8 type'},
	{'int8_t', 'param2', doc='第二个'},
	{'int8_t', 'param3',out=true},
}

API_DEF('testModule','ipc_test_basetype'){
	doc='test base type, no include unsinged',
	ret={'int8_t'},
	{'bool_t', 'param0'},
	{'int8_t', 'param1'},
	{'int16_t', 'param2'},
	{'int32_t', 'param3'},
	{'int8_t', 'param4',out=true},
	{'int16_t', 'param5',out=true},
	{'int32_t', 'param6',out=true},
	{'bool_t', 'param7',out=true},
}

API_DEF('testModule','ipc_test_unsignedtype'){
	doc='测试unsigned类型',
	ret={'uint8_t'},
	{'uint8_t', 'p1'},
	{'uint16_t', 'p2'},
	{'uint32_t', 'p3'},
	{'uint8_t', 'p4',out=true},
	{'uint16_t', 'p5',out=true},
	{'uint32_t', 'p6',out=true},
}

API_DEF('testModule','ipc_test_number'){
	doc='测试int64',
	ret={'int32_t'},
	{'int32_t', 'p1'},
	{'int64_t', 'p2'},
	{'int64_t', 'p3'},
	{'int32_t', 'p4',out=true},
	{'int64_t', 'p5',out=true},
	{'int64_t', 'p6',out=true},
}
API_DEF('testModule','ipc_test_string'){
	doc='测试string',
	ret={'void'},
	{'zstr_t', 'p1'},
	{'zstr_t', 'p2',out=true},
}

API_DEF('testModule','ipc_test_stringnull'){
	doc='测试string space and null',
	ret={'void'},
	{'zstr_t', 'p1'},
	{'zstr_t', 'p2',out=true},
}

API_DEF('testModule','ipc_test_array_int'){
	doc='test array',
	ret={'void'},
	{'array', 'p1', item='int8_t'},
	{'array', 'p2', item='int32_t'},
	{'array', 'p3', item='int32_t', out=true},
}
API_DEF('testModule','ipc_test_array_mix'){
	doc='test array',
	ret={'void'},
	{'array', 'p1', item='bool_t'},
	{'array', 'p2', item='uint32_t'},
	{'array', 'p3', item='int32_t'},
	{'array', 'p4', item='bool_t', out=true},
	{'array', 'p5', item='uint32_t', out=true},
	{'array', 'p6', item='int32_t', out=true},
}

STRUCT_DEF('testStuct1'){
	doc='test struct1',
	{'bool_t', 	'p0', doc='bool param'},
	{'uint8_t', 'p1', doc='uint8 param'},
	{'uint32_t','p2', doc='uint32 param'},
	{'int8_t',  'p3', doc='int8 param'},
	{'int32_t', 'p4', doc='int32 param'},
	{'zstr_t', 	'p5', doc='str param'},
}
API_DEF('testModule','ipc_test_struct_testStuct1'){
	doc='testStuct1',
	ret={'void'},
	{'testStuct1', 'p1'},
	{'testStuct1', 'p2', out=true},
}

STRUCT_DEF('testStuct2'){
	doc='test struct2',
	{'bool_t', 	'p0', doc='bool param'},
	{'uint8_t', 'p1', doc='uint8 param'},
}
STRUCT_DEF('testStuct3'){
	doc='test struct2',
	{'bool_t', 	'p0', doc='bool param'},
	{'testStuct2', 'p1', doc='testStuct2 param'},
}
API_DEF('testModule','ipc_test_struct_testStuctAndStruct'){
	doc='testStuctAndStruct',
	ret={'void'},
	{'testStuct3', 'p1'},
	{'testStuct3', 'p2', out=true},
}

API_DEF('testModule','ipc_test_struct_testStuctArray'){
	doc='testStuctArray',
	ret={'void'},
	{'array', 'p1', item='testStuct2'},
	{'array', 'p2', item='testStuct2', out=true},
}

STRUCT_DEF('testStuctIsArray'){
	doc='testStuctIsArray',
	{'bool_t', 	'p0', doc='bool param'},
	{'array', 'pa1', item='testStuct2', doc='testStuct2 array'},
}

API_DEF('testModule','ipc_test_struct_testStuctIsArray'){
	doc='testStuctIsArray',
	ret={'void'},
	{'testStuctIsArray', 'p1'},
	{'testStuctIsArray', 'p2', out=true},
}

API_DEF('testModule','ipc_test_args'){
	doc='test args',
	ret={'void'},
	{'ZModule_t', 'module', args=1},
	{'int32_t*', 'arg1', args=2,},
	{'int8_t', 'p1'},
	{'int8_t', 'p2', out=true},
}

STRUCT_DEF('testStructDefault'){
	doc='struct default',
	{'int8_t', 'p1'},
	{'bool_t', 'p2', default=true},
	{'int32_t', 'p3', default=0x12345678},
	{'zstr_t', 'p4', default='test'},
	{'uint32_t', 'p5',default=12345},
}

API_DEF('testModule','ipc_test_strcut_default'){
	doc='test struct default',
	ret={'void'},
	{'testStructDefault', 'p1'},
}

STRUCT_DEF('testStructEnum'){
	doc='struct enum',
	{'ETest', 'p1'},
}
API_DEF('testModule','ipc_test_strcut_enum'){
	doc='test enum and struct enum',
	ret={'ETest'},
	{'ETest', 'p1'},
	{'testStructEnum', 'p2'},
	{'ETest', 'p3',out=true},
}

API_DEF('testModule','ipc_test_event'){
	doc='test event',
	ret={'void'},
	{'ZModule_t', 'module', args=1},
	{'int8_t', 'p1',out=true},
}

local code=''
local hdr=''
for k,v in pairs(zipctype.packagelist) do
	local c,h=zipcsvr2c.ipcsvr_to_c_decode(k,zipctype.datalist,v, "test_api_decode_def.h", zipctype.c_svr_headr, zipctype.api_cb_mod, "test_api_decode")
	code=code..c
	hdr=hdr..h
end

local f=assert(io.open('test_api_decode.c','w'))
f:write(code)
f:close()

local f=assert(io.open('test_api_decode.h','w'))
f:write(hdr)
f:close()

local code=''
for k,v in pairs(zipctype.packagelist) do
	code=code..zipc2h.ipc_to_c_header(k,zipctype.datalist,v,zipctype.c_svr_headr)
end
local f=assert(io.open('test_api_decode_def.h','w'))
f:write(code)
f:close()
