md lib
set LIBDIR="A:\Microsoft Visual Studio\2019\Community-2020-3-22\VC\Tools\MSVC\14.25.28610\bin\Hostx86\x86\lib.exe"
%LIBDIR% /MACHINE:X64 /DEF:x64/libfftw3-3.def /OUT:lib/libfftw3-3_64.lib
%LIBDIR% /MACHINE:X64 /DEF:x64/libfftw3f-3.def /OUT:lib/libfftw3f-3_64.lib
%LIBDIR% /MACHINE:X64 /DEF:x64/libfftw3l-3.def /OUT:lib/libfftw3l-3_64.lib

%LIBDIR% /MACHINE:X86 /DEF:x86/libfftw3-3.def /OUT:lib/libfftw3-3_32.lib
%LIBDIR% /MACHINE:X86 /DEF:x86/libfftw3f-3.def /OUT:lib/libfftw3f-3_32.lib
%LIBDIR% /MACHINE:X86 /DEF:x86/libfftw3l-3.def /OUT:lib/libfftw3l-3_32.lib