#ifndef COMPLIED_ENTRY_H
#define COMPLIED_ENTRY_H

#ifndef WIN32
#define WIN32
#endif

#define C_METHORD
#define CPP_METHORD
  
#ifdef CPP_METHORD
#define C_CPP_METHORD
#endif

namespace System
{
    typedef unsigned char byte;
    typedef short Int16;
    typedef int Int32;
    typedef unsigned short UInt16;
    typedef unsigned int UInt32;
}


#ifdef WIN32
#define MSVC_32
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef long Int64;
typedef unsigned long UInt64;
#endif

#endif