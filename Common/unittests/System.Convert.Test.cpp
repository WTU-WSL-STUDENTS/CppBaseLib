#include <System.Convert.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace System;

void TestAll(const char* data)
{
    // char str = data[0];
    // if(IsDecDigit(str))
    // {
    //     printf("IsDecDigit : %c\n", str);
    // }
    // if(IsHexDigit(str))
    // {
    //     printf("IsHexDigit : %c\n", str);
    // }
    // if(IsAlpha(str))
    // {
    //     printf("IsAlpha : %c\n", str);
    // }
    // if(IsUpperAlpha(str))
    // {
    //     printf("IsUpperAlpha : %c\n", str);
    // }
    // if(IsLowAlpha(str))
    // {
    //     printf("IsLowAlpha : %c\n", str);
    // }
    // if(IsAlnum(str))
    // {
    //     printf("IsAlnum : %c\n", str);
    // }
    // if(IsSpace(str))
    // {
    //     printf("IsSpace : %c\n", str);
    // }
    // if(IsPunctuation(str))
    // {
    //     printf("IsSpace : %c\n", str);
    // }

    if(Convert::IsStringAllDecDigit(data) && Convert::IsStringAllDecDigit(string(data)))
    {
        printf("IsStringAllDecDigit : %s\n", data);
    }
    if(Convert::IsStringAllHexDigit(data) && Convert::IsStringAllHexDigit(string(data)))
    {
        printf("IsStringAllHexDigit : %s\n", data);
    }
    if(Convert::IsStringAllAlpha(data) && Convert::IsStringAllAlpha(string(data)))
    {
        printf("IsStringAllAlpha : %s\n", data);
    }
    if(Convert::IsStringAllUpperAlpha(data) && Convert::IsStringAllUpperAlpha(string(data)))
    {
        printf("IsStringAllUpperAlpha : %s\n", data);
    }
    if(Convert::IsStringAllLowerAlpha(data) && Convert::IsStringAllLowerAlpha(string(data)))
    {
        printf("IsStringAllLowerAlpha : %s\n", data);
    }
    if(Convert::IsStringAllAlnum(data) && Convert::IsStringAllAlnum(string(data)))
    {
        printf("IsStringAllAlnum : %s\n", data);
    }
    if(0 < Convert::Contains(data, 'a') && 0 < Convert::Contains(string(data), 'a'))
    {
        printf("Contains a %s\n", data);
    }
}

int main()
{
    TestAll("21");
    TestAll("ff");
    TestAll("FA");
    TestAll("Fa");
    TestAll(" ");
    TestAll("+ \t \r \n \v \f");
    char* ip = "192.168.10.111";
    const char* splitedStr[MAX_SPLIT_COUNT];
    size_t len[MAX_SPLIT_COUNT];
    int count = Convert::SplitString(ip, splitedStr, len,'.');
    for(int i = 0; i < count; i++)
    {
        const char* src = splitedStr[i];
        char str[_MAX_PATH] = {0};
        memcpy(str, src, len[i]);
        printf("char*:%s\n", str);
    }
    string str[MAX_SPLIT_COUNT];
    count = Convert::SplitString(string(ip), str, ".");
    for(int i = 0; i < count; i++)
    {
        printf("string:%s\n", str[i].c_str());
    }
    /* 仅支持10进制字符串转成 int*/
    printf("ToByte:%d\n", Convert::ToByte("123"));
    printf("ToInt16:%d\n", Convert::ToInt16("9999"));
    printf("ToInt32:%d\n", Convert::ToInt32("99999999"));
    printf("ToInt64:%lld\n", Convert::ToInt64("999999999999"));
    return 0;
}