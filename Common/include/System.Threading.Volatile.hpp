/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-14 14:07:06
 * @LastEditors: like
 * @LastEditTime: 2022-02-15 16:26:15
 */
#ifndef SYSTEM_THREADING_VOLATILE_HPP
#define SYSTEM_THREADING_VOLATILE_HPP

#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
namespace System::Threading
{
    class Volatile;/* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.volatile?view=net-5.0 */
};
/**
 * @brief ���ñ������Ż���ָֹ������, ȷ��д���ڴ�λ�õ�ֵ���������д��������ɼ�.
 * reference : 
 * https://blog.csdn.net/caoshangpa/article/details/78853919
 * http://www.rdrop.com/users/paulmck/scalability/paper/whymb.2010.06.07c.pdf
 * 
 */
class System::Threading::Volatile
{
private:
    Volatile(){}
public:
    template<typename T>
    inline static T Read(T& location)
    {
        return ACCESS_ONCE(location);
    }
    template<typename T>
    inline static void Write(T& location, T& val)
    {
        ACCESS_ONCE(location) = val;
    }
};

#endif