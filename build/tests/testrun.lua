--[[
	每个子目录即为一个testsuit，每个目录下可以有单独的init.lua,用于执行每个suit时使用
	建议测试的lua文件名以test开始，里面需要测试的case名称也以test开始，在执行时只查找所有以
    test开始的函数.
	每个test文件可以有一个Init函数，用于执行每个case前的初始化
	每个test文件可以有一个Destroy函数，用于执行每个case后的析构操作
	不需要执行的目录请放到excludeDir中
]]--

ZDC_DIR= ZDC_DIR or '/workspace/zdc/'
APP_DIR= APP_DIR or '/workspace/ptmstp/lua/'

package.path=APP_DIR..'lua/?.lua;'..ZDC_DIR..'lua/?.lua;'..ZDC_DIR..'lib/lua/?.lua;'..package.path
if not OS then
	if os.getenv("WINDIR") then
		OS='WIN32'
	else
		local uname=io.popen("uname -smp")
		if uname then
			local u=uname:read() or ''
			local s,m,p=string.match(u,"^(%S+)%s(%S+)%s(%S+)")
			if s=='Darwin' then
				if m=='x86_64' then
					OS='MAC64'
				else
					OS='MAC'
				end
			elseif s=='Linux' then
				if m=='x86_64' then
					OS='LINUX64'
				else
					OS='LINUX'
				end
			else
				print(s,m,p)
				error'unknow OS'
			end
		else
			error'unknow OS'
		end
	end
end

if OS=='WIN32' then
	package.cpath=[[.\?.dll;.\?51.dll;C:\Program Files\Lua\5.1\?.dll;C:\Program Files\Lua\5.1\?51.dll;C:\Program Files\Lua\5.1\clibs\?.dll;C:\Program Files\Lua\5.1\clibs\?51.dll;C:\Program Files\Lua\5.1\loadall.dll;C:\Program Files\Lua\5.1\clibs\loadall.dll]]
	package.cpath=package.cpath..[[;C:\Program Files (x86)\Lua\5.1\?.dll;C:\Program Files (x86)\Lua\5.1\?51.dll;C:\Program Files (x86)\Lua\5.1\clibs\?.dll;C:\Program Files (x86)\Lua\5.1\clibs\?51.dll;C:\Program Files (x86)\Lua\5.1\loadall.dll;C:\Program Files (x86)\Lua\5.1\clibs\loadall.dll]]
	package.cpath=APP_DIR..'../build/win32_lib/?.dll;'..package.cpath
	package.path=APP_DIR..'../build/win32_lib/?.luac;'..package.path
elseif OS=='MAC64' then
	package.cpath=ZDC_DIR..'lib/mac64/?.so;'..package.cpath
elseif OS=='LINUX' then
	package.cpath=APP_DIR..'../build/centos/?.so;'..package.cpath
elseif OS=='LINUX64' then
	package.cpath=ZDC_DIR..'lib/centos64/?.so;'..package.cpath
else
end
local mdbcmd=""

local zloader=require'zmdbloader'
local mdb,err=zloader.getmod({APP_DIR.."zos.mdb",decode=zloader.decode_decompress_default,ro=true})
if mdb then
	mdbcmd=[[
		local zloader=require'zmdbloader'
		local mdb1,err=zloader.getmod({APP_DIR.."zos.mdb",decode=zloader.decode_decompress_default,ro=true})
	]]
end

require "io"
require 'lfs'
require 'rings'

local excludeDir='common,.svn,.tmp_versions,'
local sucCnt = 0
local errCnt = 0
local testerrlist={}
local testerrinfolist={}
local onlyShow=false
local _test_config_key={'NE_ADDR', 'NE_PORT','IPC_ADDR','IPC_PORT'}
function set_test_config(S)
	for _,k	in ipairs(_test_config_key) do
		local v=_G[k]
		if v then
			if type(v)=='string' then
				S:dostring(string.format("%s='%s'",k,v))
			else
				S:dostring(k..'='..v)
			end
		end
	end
end

function docase(file, func)
	local cmdf = [[
	package.path=[==[%s]==]
	package.cpath=[==[%s]==]
	APP_DIR='%s'
	%s
	f=require('%s')
	assert(f)
	if f.Init then f.Init() end
	f.%s()

	if f.Destroy then f.Destroy() end
	return true
	]]
	local cmddestr=[[
	if f.Destroy then f.Destroy() end
	return true
	]]
	local cmd=string.format(cmdf, package.path..';./?.lua',package.cpath, APP_DIR, mdbcmd, file, func)
	local S = rings.new (_G)
	set_test_config(S)
	local r,r1=S:dostring(cmd)
	if not r then
		S:dostring(cmddestr)
	end
	assert(r,r1)
	S:close()
end

function testsurecase(name, case)
	if(onlyShow) then
		print('\t'..case)
		return
	end

	print('TESTING: '..case..'...')
	local ret, err=pcall(function() return  docase(name, case) end,debug.traceback)
	if ret then
		sucCnt = sucCnt+1
		print('SUCCESS: '..case)
	else
		errCnt = errCnt +1
		table.insert(testerrlist, name..'/'..case)
		table.insert(testerrinfolist, name..'/'..case..'\r'..err)
		print('ERROR: '..case)
		print(err)
	end
	print()
end

function testfile(filename)
	local r,_,name, ext=string.find(filename, '(.*)(.lua)')
	if( (not r) or (not name) ) then
		name = filename
		ext = '.lua'
	end
	local f=assert(io.open(name..ext))
	local ct=f:read('*a')
	f:close()
	
	if(onlyShow) then
		print(name)
	end

	print('===================================')
	print('BEGIN PACKAGE: '..filename..'...\n')

	local tmod=require(name)

	--所有case名称必须以test或_M.test开始.
	--读取文件，是为了保证测试用例执行的顺序
	for k in string.gmatch(ct, 'function% +%_+%M+%.+(test[%w_]+)') do
		if tmod[k] then
			testsurecase(name, k)
		end
	end

	print('END PACKAGE: '..filename..'...\n')
	print('===================================')
end

function testdir(dirname)
	print('**************************************')
	print('         test suit: '..dirname)
	print('--------------------------------------')
	for file in lfs.dir(dirname) do
		if file ~= "." and file ~= ".." then
			local f = dirname..'/'..file
			local attr = lfs.attributes (f)
			assert (type(attr) == "table")
			if attr.mode ~= "directory" then
				testfile(f)
			end
		end
	end
end

function testall()
	for file in lfs.dir('.') do
		if file ~= "." and file ~= ".." then
			local attr = lfs.attributes (file)
			assert (type(attr) == "table")
			if attr.mode == "directory" and (not string.find(excludeDir, file..',')) then
				testdir(file)
			end
		end
	end
end

function show_statics()
	print('=======================================')
	print('           statics')
	print('---------------------------------------')
	if #testerrlist~=0 then
		print('failure detail list:')
		for k,v in pairs(testerrinfolist) do
			print(v)
		end

		print('---------------------------------------')
		print('failure list:')
		for k,v in pairs(testerrlist) do
			print('     '..v)
		end
	end
	print('---------------------------------------')
	print(''..sucCnt..' testcases is success')
	print(''..errCnt..' testcases is failure')
end

local function pathsplit(s, delim)

  assert (type (delim) == "string" and string.len (delim) > 0,
          "bad delimiter")

  local start = 1
  local t = {}  -- results table

  -- find each instance of a string followed by the delimiter
  while true do
    local pos = string.find (s, delim, start, true) -- plain find

    if not pos then
      break
    end

    table.insert (t, string.sub (s, start, pos - 1))
    start = pos + string.len (delim)
  end -- while

  -- insert final one (after last delimiter)
  table.insert (t, string.sub (s, start))

  return t[1], t[2],t[3]

end -- function spli

function parseArgs(target)
	if (not target) then
		return testall()
	end
	local dir, file, case = pathsplit(target, '.')
	if case then
		return testsurecase(dir..'/'..file, case)
	end
	
	if(file) then
		return testfile(dir..'/'..file)
	end

	return	testdir(target)
end

local target=nil

local function showHelp()
	print('gltest [-help]|[-show] [testsuit[.testcase[.test]]]')
	os.exit()
end

local function showCase()
	onlyShow=true
	target=arg[2]
end

local argcmd ={
['-help']=showHelp,
['-show']=showCase,
}

function main()
	target=arg[1]
	if arg[1] and argcmd[arg[1]]then
		argcmd[arg[1]]()
	end
	parseArgs(target)
	show_statics()
end

main()
