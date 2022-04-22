# CMakeList.txt: CppLearn 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("CppLearn")
# static_cast / dynamic_cast / const_cast / reinterpret_cast / c style cast 
add_executable (cast "cast.cpp")
# 多继承 / 菱形继承 内存分布
add_executable (multInheritation "multInheritation.cpp")
# 定义域与对象生命周期测试
add_executable (objectLifetime "objectLifetime.cpp")
# 虚函数表
add_executable (vftable "vftable.cpp")
# " 泛型 / 模板 约束"
add_executable (SFINAE "SFINAE.cpp")
# 右值引用, 万能引用，完美转发
add_executable (rvalueReference "rvalueReference.cpp")
add_executable (test "test.cpp")

# TODO: 如有需要，请添加测试并安装目标。
