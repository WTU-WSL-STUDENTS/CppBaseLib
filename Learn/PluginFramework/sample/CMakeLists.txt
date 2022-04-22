include_directories(../include)
include_directories(Plugin)
add_library(Filesystem SHARED "Plugin/Filesystem.cpp")
add_executable(PluginEntries "PluginEntries.cpp")