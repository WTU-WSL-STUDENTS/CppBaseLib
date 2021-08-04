#ifndef SYSTEM_DATETIME_HPP
#define SYSTEM_DATETIME_HPP

#include "complied_entry.h"
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace System
{
    class DateTime;
    #ifdef WIN32
    typedef SYSTEMTIME SystemTime;
    #else
    typedef struct _SYSTEMTIME {
        WORD wYear;
        WORD wMonth;
        WORD wDayOfWeek;
        WORD wDay;
        WORD wHour;
        WORD wMinute;
        WORD wSecond;
        WORD wMilliseconds;
    } SYSTEMTIME;
    #endif
}
class System::DateTime 
{
protected:
    SystemTime currentTime;
    char* strFormat;
public:
    DateTime():strFormat("%04d-%02d-%02d %02d:%02d:%02d.%03d"){}
    DateTime(WORD year, WORD month, WORD day, WORD hour, WORD minute, WORD second, WORD Millisecond)
        :currentTime({year, month, 0, day, hour, minute, second, Millisecond}), strFormat("%04d-%02d-%02d %02d:%02d:%02d.%03d"){}
    WORD Year(){return currentTime.wYear;};
    WORD Month(){return currentTime.wMonth;};
    WORD DayOfWeek(){return currentTime.wDayOfWeek;};
    WORD Day(){return currentTime.wDay;};
    WORD Hour(){return currentTime.wHour;};
    WORD Minute(){return currentTime.wMinute;};
    WORD Second(){return currentTime.wSecond;};
    WORD Milliseconds(){return currentTime.wMilliseconds;};
    virtual char* ToString()/* ͨ������ToStringʵ�ֲ�ͬ�Ĵ�ӡ��ʽ */
    {
        char buff[MAX_PATH];
	    sprintf(buff, strFormat, currentTime.wYear, currentTime.wMonth, currentTime.wDay
			, currentTime.wHour, currentTime.wMinute, currentTime.wSecond, currentTime.wMilliseconds);
        return buff;
    }
    static bool Now(DateTime& currentTime)/* ���ؼ������ǰ��ʾ��ʱ�� */
    {
        #ifdef WIN32
            GetLocalTime(&currentTime.currentTime);
            return true;
        #else     
            time_t currentSecondTime;
            struct timeval tv;
            time(&currentSecondTime);
            if(0 != gettimeofday(&tv, NULL))
            {
                return NULL;
            }
            struct tm currentUtcTime = *gmtime(&currentSecondTime);
            struct tm currentTime = *localtime(&currentSecondTime);
            currentTime.currentTime = SystemTime
            {
                1900 + currentTime.tm_year, 
                1 + currentTime.tm_mon, 
                currentTime.tm_wday, 
                currentTime.tm_mday, 
                currentTime.tm_hour, 
                currentTime.tm_min, 
                currentTime.tm_sec, 
                tv.tv_use / 1000
            };
            return true;
        #endif
    }
    static bool UtcNow(DateTime &currentTime)/* ���ؼ������ǰ��ʾ��ʱ��, ��ʾΪЭ��ͨ��ʱ�� (UTC) */
    {
        #ifdef WIN32
            TIME_ZONE_INFORMATION timeZoneinfo;
	        GetTimeZoneInformation(&timeZoneinfo);
	        SystemTime localTime = {0};
            GetLocalTime(&localTime);
            return TzSpecificLocalTimeToSystemTime(&timeZoneinfo, &localTime, &currentTime.currentTime);/* https://docs.microsoft.com/zh-cn/windows/win32/api/timezoneapi/nf-timezoneapi-tzspecificlocaltimetosystemtime */
        #else     
            time_t currentSecondTime;
            time(&currentSecondTime); 
            struct timeval tv;
            if(0 != gettimeofday(&tv, NULL))
            {
                return NULL;
            }
            struct tm currentUtcTime = *gmtime(&currentSecondTime);
            currentTime.currentTime = DateTime
            {
                1900 + currentUtcTime.tm_year, 
                1 + currentUtcTime.tm_mon, 
                currentUtcTime.tm_wday, 
                currentUtcTime.tm_mday, 
                currentUtcTime.tm_hour, 
                currentUtcTime.tm_min, 
                currentUtcTime.tm_sec, 
                tv.tv_use / 1000
            };
            return true;
        #endif

    }
};

#endif