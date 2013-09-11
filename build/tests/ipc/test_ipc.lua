module(..., package.seeall)

local cli

local node='testapi'
function Init()
	if not USE_NETLINK then
		cli=require 'ne.ipc.ipcclient'.connect(_G.IPC_ADDR or '127.0.0.1', _G.IPC_PORT or 5000)
	else
		cli=require 'ne.ipc.ipcnetlink'.connect(21)
	end
end

function Destroy()
	cli:close()
end

function test_testapi_ipc_test_call()
	local r,msg=cli:Call(0,node,'ipc_test_call',0x12,0xff)
	assert(r,msg)
	assert(msg==94,msg)
end
function test_testapi_ipc_api_ipc_test_basetype()
	local r,r1,r2,r3,r4,r5=cli:Call(0,node,'ipc_test_basetype',false,0x12,0xf1ff,-131234)
	assert(r,r1)
	assert(r1==50,''..r1)
	assert(r2==-2,''..r2)
	assert(r3==-2,''..r3)
	assert(r4==-233631179,''..r4)
	assert(r5==true)
end
function test_testapi_ipc_api_ipc_test_unsignedtype()
	local r,r1,r2,r3,r4=cli:Call(0,node,'ipc_test_unsignedtype',0x12,0xf1ff,0xf2131234)
	assert(r,r1)
	assert(r1==0x12,''..r1)
	assert(r2==0xfe,''..r2)
	assert(r3==0xfffe,''..r3)
	assert(r4==0xf2131235,''..r4)
end

local function cmpnumber(v1,v2)
	return v1>=v2-0.0005 and v1<=v2+0.0005
end

function test_testapi_ipc_api_ipc_test_number()
	local r,r1,r2,r3,r4=cli:Call(0,node,'ipc_test_number',12345,1234567898765,1234567890123)
	assert(r,r1)
	assert(cmpnumber(r1,12345),''..r1)
	assert(cmpnumber(r2,-567),''..r2)
	assert(cmpnumber(r3,98765432342),''..r3)
	assert(cmpnumber(r4,9876543210123),''..r4)
end

function test_testapi_ipc_api_ipc_test_string()
	local r,r1=cli:Call(0,node,'ipc_test_string',"test string")
	assert(r,r1)
	assert(r1=='params string', ''..r1)
end

function test_testapi_ipc_api_ipc_test_stringnull()
	local r,r1,r2=cli:Call(0,node,'ipc_test_stringnull',"")
	assert(r,r1)
	assert(not r1,''..(r1 or ''))
end

function test_testapi_ipc_api_ipc_test_array_int()
	local r,r1,r2=cli:Call(0,node,'ipc_test_array_int',{1,2,3,4,5},{1000001,1000002,1000003,1000004,1000005})
	assert(r,r1)
	assert(#r1, 10)
	for i=1,10 do
		assert(r1[i]==i,''..r1[i])
	end
end
function test_testapi_ipc_api_ipc_test_array_mix()
	local r,r1,r2,r3=cli:Call(0,node,'ipc_test_array_mix',{true,false,true,false},{1000001,1000002,1000003,1000004,1000005}, {12345, 12346, 12347})
	assert(r,r1)
	assert(#r1==3)
	assert(not r1[1])
	assert(r1[2])
	assert(not r1[3])

	assert(#r2==4)
	for i=1,4 do
		assert(r2[i]==0xf1ff+i-1, ''..r2[i])
	end

	assert(#r3==4)
	for i=1,4 do
		assert(cmpnumber(r3[i],-567+i-1), ''..r3[i])
	end
end
function test_testapi_ipc_api_ipc_test_struct_testStuct1()
	local r,r1=cli:Call(0,node,'ipc_test_struct_testStuct1',{p0=false,p1=30,p2=0x12345678,p3=-12,p4=-123456,p5='test'})
	assert(r,r1)
	assert(r1.p0)
	assert(r1.p1==0xee)
	assert(r1.p2==0x87654321)
	assert(r1.p3==-12,r1.p3)
	assert(r1.p4==-87654321,r1.p4)
	assert(r1.p5=='qwerty',r1.p5)
end
function test_testapi_ipc_api_ipc_test_struct_testStuctAndStruct()
	local r,r1=cli:Call(0,node,'ipc_test_struct_testStuctAndStruct',{p0=false,p1={p0=true,p1=12}})
	assert(r,r1)
	assert(r1.p0)
	assert(r1.p1)
	assert(r1.p1.p0==false)
	assert(r1.p1.p1==200)
end

function test_testapi_ipc_api_ipc_test_struct_testStuctArray()
	local p1={}
	for i=1,5 do
		p1[i]={p0=true,p1=i}
	end
	local r,r1=cli:Call(0,node,'ipc_test_struct_testStuctArray',p1)
	assert(r,r1)
	assert(r1)
	assert(#r1==10, ''..#r1)
	for i=1,10 do
		assert(r1[i].p0==false)
		assert(r1[i].p1==99+i)
	end
end

function test_testapi_ipc_api_ipc_test_struct_testArrayStruct()
	local p1={}
	for i=1,5 do
		p1[i]={p0=true,p1=i}
	end
	local r,r1=cli:Call(0,node,'ipc_test_struct_testStuctIsArray',{p0=true,p1=p1})
	assert(r,r1)
	assert(r1)
	assert(r1.p0)
	r1=r1.pa1
	assert(#r1==10, ''..#r1)
	for i=1,10 do
		assert(r1[i].p0==false)
		assert(r1[i].p1==99+i)
	end
end

function test_testapi_ipc_api_ipc_ipc_test_args()
	local r,r1=cli:Call(0,node,'ipc_test_args',100)
	assert(r,r1)
	assert(r1==10)
end

function test_testapi_ipc_api_ipc_ipc_test_strcut_default()
	local r,r1=cli:Call(0,node,'ipc_test_strcut_default',{p1=100,p5=1000})
	assert(r,r1)
end

function test_testapi_ipc_not_enough_params()
	local r,r1,r2,r3=cli:Call(0,node,'ipc_test_array_mix',{true,false,true,false},{1000001,1000002,1000003,1000004,1000005})
	assert(not r,r1)
end

function test_testapi_ipc_api_ipc_ipc_test_strcut_enum()
	local r,r1,r2=cli:Call(0,node,'ipc_test_strcut_enum',2,{p1=1})
	assert(r,r1)
	assert(r1==1,r1)
	assert(r2==2,r2)
end

local sn=1
local function _event_cb(evtid, evt)
	assert(sn==2)
	assert(evtid==2)
	assert(evt==111)
	sn=sn+1
end

function test_testapi_ipc_api_ipc_ipc_test_event()
	cli:reg_event(2,_event_cb)
	cli._react={EV_TIMEOUT=1}
	local r,r1=cli:Call(0,node,'ipc_test_event')
	assert(r,r1)
	assert(r1==-56)
	assert(sn==1)
	sn=sn+1
	cli:step()
	assert(sn==3)
end
