--[[
Description: 
Version: 1.0
Autor: like
Date: 2021-09-30 17:20:37
LastEditors: like
LastEditTime: 2021-09-30 17:21:04
--]]
print("加载就会调用：")
local fun = require "lua_test"
ret = fun.Add(10,20)
print(ret)
local map = {}
map[0] = "first"
map[1] = "second"
map[2] = "third"
print(fun.DisplayMap(map))

function calledbycpp(left, right)
	print("通过lua_pcall调用：")
	sum = left + right;
	return sum;
end