copy ..\cmake\CMakeLists.txt ..\build
copy ..\cmake\CommonUnitTests.cmake ..\build
@REM ����ָ��CMakeLists.txt��vs2019�ı���������Win32����
cmake -G "Visual Studio 16 2019" -A Win32 ..\build\CMakeLists.txt
@REM ����x64ƽ̨���б���,����л�ƽ̨���б��룬��Ҫ��..\buildĿ¼�ڵ��ļ�ȫ��ɾ���� ����̨����Ĭ��Ҳ��x64 cmake ../build/CMakeLists.txt
@REM cmake -G "Visual Studio 16 2019" -A x64 ..\build\CMakeLists.txt
cmake --build ..\build 