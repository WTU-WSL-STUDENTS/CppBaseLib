set(THIRD_PARTY_PATH ../thirdparty)
set(Release_PATH    ../../build/bin/${Path_Suffix})
set(Lib_Path        ../../build/lib/${Path_Suffix})
include_directories(../include)

add_executable(System.Convert.Test System.Convert.Test.cpp)
add_executable(System.Console.Test System.Console.Test.cpp)

include_directories(${THIRD_PARTY_PATH}/sqlite-3-36)
add_executable(System.Data.Sqlite3.Test System.Data.Sqlite3.Test.cpp)
target_link_libraries(System.Data.Sqlite3.Test ${Lib_Path}/sqlite3)

include_directories(${THIRD_PARTY_PATH}/glad/include)
include_directories(${THIRD_PARTY_PATH}/glfw-3.3.4/include)
include_directories(${THIRD_PARTY_PATH}/glfw-3.3.4/deps/)
add_executable(System.Form.OpenGL.Test System.Form.OpenGL.Test.cpp)
target_link_libraries(System.Form.OpenGL.Test ${Lib_Path}/glfw3 ${Lib_Path}/glad)


add_executable(System.IO.Directory.Test System.IO.Directory.Test.cpp)
add_executable(System.IO.FileStream.Test System.IO.FileStream.Test.cpp)
add_executable(System.IO.Ini.Test System.IO.Ini.Test.cpp)
add_executable(System.IO.MemoryMappedFiles.Recv.Test System.IO.MemoryMappedFiles.Recv.Test.cpp)
add_executable(System.IO.MemoryMappedFiles.Send.Test System.IO.MemoryMappedFiles.Send.Test.cpp)
add_executable(System.Socket.Tcp.Client.Test System.Socket.Tcp.Client.Test.cpp)
add_executable(System.Socket.Tcp.Server.Test System.Socket.Tcp.Server.Test.cpp)
add_executable(System.Socket.Udp.Client.Unicast.Test System.Socket.Udp.Client.Unicast.Test.cpp)
add_executable(System.Socket.Udp.Client.Multicast.Test System.Socket.Udp.Client.Multicast.Test.cpp)
add_executable(System.Socket.Udp.Client.Broadcast.Test System.Socket.Udp.Client.Broadcast.Test.cpp)
add_executable(System.Socket.Udp.Server.Test System.Socket.Udp.Server.Test.cpp)
add_executable(System.Threading.Thread.Test System.Threading.Thread.Test.cpp)
add_executable(System.Threading.ThreadPool.Test System.Threading.ThreadPool.Test.cpp)
add_executable(System.Threading.Timer.Test System.Threading.Timer.Test.cpp)
add_executable(System.Threading.WaitHandle.Test System.Threading.WaitHandle.Test.cpp)

add_executable(System.LinkList.Test System.LinkList.Test.cpp)
add_executable(System.List.Test System.List.Test.cpp)
add_executable(System.Messaging.MessageQueue.Test System.Messaging.MessageQueue.Test.cpp)

include_directories(${THIRD_PARTY_PATH}/ftplib-4.0-1)
add_executable(System.Net.FtpWebRequest.Test System.Net.FtpWebRequest.Test.cpp)
target_link_libraries(System.Net.FtpWebRequest.Test ${Lib_Path}/ftplib)

include_directories(${THIRD_PARTY_PATH}/tinyxml-2-6-2)
add_executable(System.Xml.Test System.Xml.Test.cpp)
target_link_libraries(System.Xml.Test ${Lib_Path}/xmllib)

include_directories(${THIRD_PARTY_PATH}/nlohmann.json)
add_executable(System.Text.Json.Test System.Text.Json.Test.cpp)