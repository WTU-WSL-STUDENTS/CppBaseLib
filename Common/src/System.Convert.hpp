#ifndef CONVERT_H
#define CONVERT_H

#include "complied_entry.h"
#include <ctype.h>
#include <string>  

namespace System
{
class Convert
{
#if (defined C_METHORD) || (defined CPP_METHORD)
    #define IsDecDigit(ch)      (0 != isdigit(ch))  /* 十进制 */
    #define IsHexDigit(ch)      (0 != isxdigit(ch)) /* 十六进制 */
    #define IsAlpha(ch)         (0 != isalpha(ch))  /* 字母 */
    #define IsUpperAlpha(ch)    (0 != isupper(ch))  /* 大写字母 */
    #define IsLowAlpha(ch)      (0 != islower(ch))  /* 小写字母 */
    #define IsAlnum(ch)         (0 != isalnum(ch))  /* 字母或数字 */
    #define IsSpace(ch)         (0 != isspace(ch))  /* 空格(' ')、水平定位字符('\t')、归位键('\r')、换行('\n')、垂直定位字符('\v')或翻页('\f')的情况 */
    #define IsPunctuation(ch)   (0 != ispunct(ch)   )/* 非空格、非数字和非英文字母。 */
    
    /*
        %d十进制有符号整数

        %u十进制无符号整数

        %f浮点数

        %s字符串

        %c单个字符

        %p指针的值

        %e指数形式的浮点数

        %x,%X无符号以十六进制表示的整数

        %0无符号以八进制表示的整数

        %g自动选择合适的表示法
    */
    #define RETURN_INT_TO_STRING        char buf[16];   memset(buf, 0, sizeof(buf)); sprintf_s(buf, 16, "%d",   value); return buf;
    #define RETURN_LONGLONG_TO_STRING   char buf[32];   memset(buf, 0, sizeof(buf)); sprintf_s(buf, 32, "%lld", value); return buf;
    #define RETURN_ULONGLONG_TO_STRING  char buf[32];   memset(buf, 0, sizeof(buf)); sprintf_s(buf, 32, "%llu", value); return buf;
    #define RETURN_DOUBLE_TO_STRING     char buf[300];  memset(buf, 0, sizeof(buf)); sprintf_s(buf, 300,"%lf", value);  return buf;

    #define MAX_SPLIT_COUNT 255
#endif
public:
#ifdef C_METHORD
	static bool IsStringAllDecDigit(const char* str)
	{
        do
        {
            if(!IsDecDigit(*str))
            {
                return false;
            }
            str++;
        } while (0 != *str);
        return true;
	}
	static bool IsStringAllHexDigit(const char* str)
	{
        do
        {
            if(!IsHexDigit(*str))
            {
                return false;
            }
            str++;
        } while (0 != *str);
        return true;
	}
    static bool IsStringAllAlpha(const char* str)
    {
        do
        {
            if(!IsAlpha(*str))
            {
                return false;
            }
            str++;
        } while (0 != *str);
        return true;
    }
    static bool IsStringAllUpperAlpha(const char* str)
    {
        do
        {
            if(!IsUpperAlpha(*str))
            {
                return false;
            }
            str++;
        } while (0 != *str);
        return true;
    }
    static bool IsStringAllLowerAlpha(const char* str)
    {
        do
        {
            if(!IsLowAlpha(*str))
            {
                return false;
            }
            str++;
        } while (0 != *str);
        return true;
    }
    static bool IsStringAllAlnum(const char* str)
    {
        do
        {
            if(!IsAlnum(*str))
            {
                return false;
            }
            str++;
        } while (0 != *str);
        return true;
    }
    static int Contains(const char* str, char target)
    {
        int nRet = 0;
        do
        {
            if(target == *str)
            {
                return nRet;
            }
            str++;
            nRet++;
        } while (0 != *str);
        return -1;
    }
    static char ToChar(const char* str)
    {
        return (char)atoi(str);
    }
    static byte ToByte(const char* str)
    {
        return (byte)atoi(str);
    }
    static Int16 ToInt16(const char* str)
    {
        return (short)atoi(str);
    }
    static Int32 ToInt32(const char* str)
    {
        return (int)atoi(str);
    }
    static Int64 ToInt64(const char* str)
    {
		return _atoi64(str);
    }
    static double ToDouble(const char* str)
    {
        return atof(str);
    }
    static char* ToString(char value)
    {
        RETURN_INT_TO_STRING
    }
    static char* ToString(byte value)
    {
        RETURN_INT_TO_STRING
    }
    static char* ToString(Int16 value)
    {
        RETURN_INT_TO_STRING
    }
    static char* ToString(UInt16 value)
    {
        RETURN_INT_TO_STRING
    }
    static char* ToString(Int32 value)
    {
        RETURN_INT_TO_STRING
    }
    static char* ToString(UInt32 value)
    {
        RETURN_INT_TO_STRING
    }
    static char* ToString(Int64 value)
    {
        RETURN_LONGLONG_TO_STRING
    }
    static char* ToString(UInt64 value)
    {
        RETURN_ULONGLONG_TO_STRING
    }
    static char* ToString(double value)
    {
        RETURN_DOUBLE_TO_STRING
    }
    static bool ToUpper(char& src)
    {  
        if (IsLowAlpha(src))
        {
            src -= 32;
            return true;
        }
        return false;
    }
    static bool ToLower(char& src)
    {
        if (IsUpperAlpha(src))
        {
            src += 32;
            return true;
        }
        return false;
    }
	static int SplitString(const char* str, const char* (&splitedStr)[MAX_SPLIT_COUNT]/* str中分割起始地址*/,size_t (&len)[MAX_SPLIT_COUNT], char sep = ' ')/* "ab c de " -> "ab" "c" "de" */
    {
        int    count = 0;
        int    currentStart = 0;
        size_t currentLen = 0;
        for(int i = 0; 0 != str[i]; i++)
        {
            if(sep != str[i])
            {
                currentLen++;
            }
            else
            {
                splitedStr[count] = &(str[currentStart]);
                len[count] = currentLen;
                count++;
                currentLen = 0;
                currentStart = i + 1;
            }
        }
        if(0 < currentLen)
        {
            splitedStr[count] = &(str[currentStart]);
            len[count] = currentLen;
            count++;
        }
        return count;
    }
#endif
#if defined CPP_METHORD
    static bool IsStringAllDecDigit(const std::string str)
	{
        std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char c) { return !IsDecDigit(c);});
        return it == str.end();
	}
	static bool IsStringAllHexDigit(const std::string str)
	{
        std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char c) { return !IsHexDigit(c);});
        return it == str.end();
	}
	static bool IsStringAllAlpha(const std::string str)
	{
        std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char c) { return !IsAlpha(c);});
        return it == str.end();
	}
	static bool IsStringAllUpperAlpha(const std::string str)
	{
        std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char c) { return !IsUpperAlpha(c);});
        return it == str.end();
	}
	static bool IsStringAllLowerAlpha(const std::string str)
	{
        std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char c) { return !IsLowAlpha(c);});
        return it == str.end();
	}
	static bool IsStringAllAlnum(const std::string str)
	{
        std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char c) { return !isalnum(c);});
        return it == str.end();
	}
    static int Contains(const std::string str, char target)
    {
        return 0 < str.find_first_of(target);
    } 
    static char ToChar(const std::string str)
    {
        return (char)atoi(str.c_str());
    }
    static byte ToByte(const std::string str)
    {
        return (byte)atoi(str.c_str());
    }
    static Int16 ToInt16(const std::string str)
    {
        return (short)atoi(str.c_str());
    }
    static Int32 ToInt32(const std::string str)
    {
        return (int)atoi(str.c_str());
    }
    static Int64 ToInt64(const std::string str)
    {
		return _atoi64(str.c_str());
    }
    #ifndef C_METHORD
        static std::string ToString(char value)
        {
            return std::to_string(value);
        }
        static std::string ToString(byte value)
        {
            return std::to_string(value);
        }
        static std::string ToString(Int16 value)
        {
            return std::to_string(value);
        }
        static std::string ToString(UInt16 value)
        {
            return std::to_string(value);
        }
        static std::string ToString(Int32 value)
        {
            return std::to_string(value);
        }
        static std::string ToString(UInt32 value)
        {
            return std::to_string(value);
        }
        static std::string ToString(Int64 value)
        {
            return std::to_string(value);
        }
        static std::string ToString(UInt64 value)
        {
            return std::to_string(value);
        }
    #endif
	static int SplitString(std::string str, std::string (&splitedStr)[MAX_SPLIT_COUNT], std::string sep = " ")
    {
        int count = 0;
        std::string strTmp;
		std::string::size_type nPosBegin = 0;
		std::string::size_type nCommaPos = 0;

		while (nPosBegin != std::string::npos)
		{
			nCommaPos = str.find(sep, nPosBegin);
			if (nCommaPos != std::string::npos)
			{
				strTmp = str.substr(nPosBegin, nCommaPos - nPosBegin);
				nPosBegin = nCommaPos + sep.length();
			}
			else
			{
				strTmp = str.substr(nPosBegin);
				nPosBegin = nCommaPos;
			}
            splitedStr[count++] = strTmp;
		}
        return count;
    }
#endif
};
}

#endif