local fun = require "lua_test"
ret = fun.Add(10,20)
print(ret)
local map = {}
map[0] = "first"
map[1] = "second"
map[2] = "third"
print(fun.DisplayMap(map))