#ifndef SYSTEM_DATETIME_HPP
#define SYSTEM_DATETIME_HPP

#include "CompliedEntry.h"
#include <stdio.h>

#ifdef __LINUX
#include <sys/time.h>
#elif __WINDOWS
#pragma warning(disable:4996)
#include <time.h>
int gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year  = wtm.wYear - 1900;
    tm.tm_mon   = wtm.wMonth - 1;
    tm.tm_mday  = wtm.wDay;
    tm.tm_hour  = wtm.wHour;
    tm.tm_min   = wtm.wMinute;
    tm.tm_sec   = wtm.wSecond;
    tm. tm_isdst= -1;
    clock = mktime(&tm);
    tp->tv_sec  = (long)clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return (0);
}
#endif

namespace System
{
    typedef tm DTime;
    class DateTime;
    enum DateTimeKind
    {
        None = 0,   /* δָ������ */
        Utc,        /* UTC */
        Local       /* ����ʱ�� */
    };
    enum DayOfWeek
    {
        Sunday = 0,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday
    };
    enum TimeFormatType
	{
		STAMP,         /* %04d-%02d-%02d %02d:%02d:%02d.%03d */
		STANDARD,      /* %04d-%02d-%02d %02d:%02d:%02d */
		SHORT,         /* %04d%02d%02d%02d%02d%02d */
		SHORTDATE,     /* %04d%02d%02d */
		SHORTTIME,     /* %02d%02d%02d */
		LONGDATE,      /* %04d-%02d-%02d */
		LONGTIME,      /* %02d:%02d:%02d */
		LONG,		   /* %04d%02d%02d%02d%02d%02d%03d */
		FORMATMAX
	};
    const char* TimeFormat[FORMATMAX] =
    {
        "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        "%04d-%02d-%02d %02d:%02d:%02d",
        "%04d%02d%02d%02d%02d%02d",
        "%04d%02d%02d",
        "%02d%02d%02d",
        "%04d-%02d-%02d",
        "%02d:%02d:%02d",
        "%04d%02d%02d%02d%02d%02d%03d"
    };
}
class System::DateTime 
{
/* ------------------------------------ type convert function --------------------------------------------- */
/**
 *  time_t
 *  |
 *  |--->Time_tToFileTime   ]FILETIME
 *  |
 *  |--->gmtime ]
 *  |<---mktime ]DateTime (UtcTime)
 *  |
 *  |--->localtime  ]
 *  |<---mktime     ]DateTime (LocalTime)
 *  |
 */
public:
    /**
     * @brief 1970��1��1����������ʱ�������ת���ļ�ʱ��
     * 
     * @param fileTime 
     * @param ft 
     */
    inline static void Time_tToFileTime(const time_t& fileTime, FILETIME& ft)
    {
        LONGLONG nLL = Int32x32To64(fileTime, 10000000) + 116444736000000000;
        ft.dwLowDateTime = (DWORD)nLL;
        ft.dwHighDateTime = (DWORD)(nLL >> 32);
    }
    /**
     * @brief 1970��1��1����������ʱ�������ת�ɱ���ʱ��
     * 
     * @param time 
     * @param dt 
     */
    inline static void Time_tToLocalTime(const time_t& time, DateTime& dt)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        dt.t = *localtime(&time);
        dt.milliseconds = tv.tv_usec / 1000;
        dt.kind = Local;
    }
    /**
     * @brief 1970��1��1����������ʱ�������ת�� Utc ʱ��
     * 
     * @param time 
     * @param dt 
     */
    inline static void Time_tToUtcTime(const time_t& time, DateTime& dt)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        dt.t = *gmtime(&time);
        dt.milliseconds = tv.tv_usec / 1000;
        dt.kind = Utc;
    }
    /**
     * @brief ʱ��ת����1970��1��1����������ʱ�������
     * 
     * @param dt 
     * @param time 
     */
    inline static void DateTimeToTime_t(const DateTime& dt, time_t& time)
    {
        time = mktime(&static_cast<tm>(dt.t));
    }
/* -------------------------------------------------------------------------------------------------------- */
protected:
    DTime t;
    int milliseconds;
    TimeFormatType formatType;
    DateTimeKind kind;
public:
    DateTime() : milliseconds(0), formatType(STANDARD), kind(DateTimeKind::None){}
    DateTime(int year /* [1900, now]*/, int month /* [1, 12]*/, int day/* [1, 31]*/, int hour, int minute, int second, int millisecond = 0)
        : t({second, minute, hour, day, month - 1, year - 1900, 0, 0, -1}), 
        milliseconds(millisecond), formatType(STANDARD), kind(DateTimeKind::None){}
    inline int Year()
    {
        return 1900 + t.tm_year;
    }
    inline int Month()
    {
        return t.tm_mon;
    }
    inline System::DayOfWeek DayOfWeek()
    {
        return (System::DayOfWeek)t.tm_wday;
    }
    inline int DayOfYear()
    {
        return t.tm_yday;
    }
    inline int Day()
    {
        return t.tm_mday;
    }
    inline int Hour()
    {
        return t.tm_hour;
    }
    inline int Minute()
    {
        return t.tm_min;
    }
    inline int Second()
    {
        return t.tm_sec;
    }
    inline int Milliseconds()
    {
        return milliseconds;
    }
    /**
     * @brief ͨ������ToStringʵ�ֲ�ͬ�Ĵ�ӡ��ʽ
     * 
     * @return char* 
     */
    inline char* ToString()
    {
        char buff[_MAX_PATH] = {0};
	    sprintf(buff, TimeFormat[formatType], Year(), Month(), Day(), Hour(), Minute(), Second(), Milliseconds());
        return buff;
    }
    /**
     * @brief ʱ��Ƚ�����
     *  С����| t1 ���� t2��
     *  ��    | t1 �� t2 ��ͬ��
     *  ������| t1 ���� t2��
     * 
     * @param t1 
     * @param t2 
     * @return int ���ؽ��
     */
    inline static int Compare(const DateTime& t1, const DateTime& t2)
    {
        int* p1 = (int*)&t1.t;
        int* p2 = (int*)&t2.t;
        for(int i = 5; i > -1; i--)
        {
            if(p1[i] != p2[i])
            {
                return p1[i] > p2[i] ? 1 : -1;
            }
        }
        return 0;
    }
    /**
     * @brief ָ�������°���������
     * 
     * @param year 
     * @param month 
     * @return int 
     */
    inline static int DaysInMonth (int year, int month)
    {
        switch(month)
        {
            case 1 :
            case 3 :
            case 5 :
            case 7 :
            case 8 :
            case 10 :
            case 12 : 
                return 31;
            break;
            case 4 :
            case 6 :
            case 9 :
            case 11 :
                return 30;
            case 2:
                return IsLeapYear(year) ?  29 : 28;
            default :
                return 0;
        }
    }
    /**
     * @brief ��ָ���� Windows �ļ�ʱ��ת��Ϊ��Ч�ı���ʱ��
     * 
     * @param fileTime tickcount 
     * @param dt ����ʱ��
     * @return true 
     * @return false 
     */
    inline static bool FromFileTime(const time_t& fileTime, DateTime &dt)
    {
        Time_tToLocalTime(fileTime, dt);
        return true;
    }
    /**
     * @brief ��ָ���� Windows �ļ�ʱ��ת��Ϊ��Ч��UTCʱ��
     * 
     * @param fileTime 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool FromFileTimeUTC(const time_t fileTime, DateTime &dt)
    {
        Time_tToLocalTime(fileTime, dt);
        return true;
    }
    /**
     * @brief �Ƿ�Ϊ����
     * 
     * @param year 
     * @return true 
     * @return false 
     */
    inline static bool IsLeapYear(int year)
    {
        return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    }
    /**
     * @brief �� DateTime תΪ�����ļ�ʱ��
     * 
     * @return time_t 
     */
    inline time_t ToFileTime()
    {
        time_t t;
        DateTimeToTime_t(ToLocalTime(), t);
        return t;
    }
    /**
     * @brief �� DateTime תΪ UTC �ļ�ʱ��
     * 
     * @return time_t 
     */
    inline time_t ToFileTimeUTC()
    {
        time_t t;
        DateTimeToTime_t(ToUtcTime(), t);
        return t;
    }
    /**
     * @brief �� DateTime תΪ����ʱ��
     * 
     * @return DateTime& 
     */
    inline DateTime& ToLocalTime()
    {
        DateTime localTime;
        if(kind == DateTimeKind::Utc)
        {
            time_t t;
            DateTimeToTime_t(*this, t);
            Time_tToLocalTime(t, localTime);
            localTime.milliseconds = milliseconds;
        }
        else
        {
            localTime = *this;
        }
        return localTime;
    }
    /**
     * @brief �� DateTime תΪ UTC ʱ��
     * 
     * @return DateTime& 
     */
    inline DateTime& ToUtcTime()
    {
        DateTime utc;
        if(kind == DateTimeKind::Local)
        {
            time_t t;
            DateTimeToTime_t(*this, t);
            Time_tToUtcTime(t, utc);
            utc.milliseconds = milliseconds;
        }
        else
        {
            utc = *this;
        }
        return utc;
    }
    /**
     * @brief ���ؼ������ǰ��ʾ��ʱ��
     * 
     * @return DateTime& 
     */
    inline static DateTime& Now()
    {
        time_t now;
        time(&now);
        DateTime dt;
        Time_tToLocalTime(now, dt);
        return dt;
    }
    /**
     * @brief ���ؼ������ǰ��ʾ��ʱ��, ��ʾΪЭ��ͨ��ʱ�� (UTC)
     * 
     * @return DateTime& 
     */
    inline static DateTime& UtcNow()
    {
        time_t now;
        time(&now);
        DateTime dt;
        Time_tToUtcTime(now, dt);
        return dt;
    }
};
#ifdef __WINDOWS
#pragma warning(default:4996)
#endif
#endif