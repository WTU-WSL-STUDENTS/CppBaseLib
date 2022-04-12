/*
 * @Description: Ïß³Ì³Ø
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-19 10:50:50
 * @LastEditors: like
 * @LastEditTime: 2022-01-19 10:54:46
 */
#ifndef SYSTEM_THREADING_TASKS_TASKSCHEDULER_HPP
#define SYSTEM_THREADING_TASKS_TASKSCHEDULER_HPP

#include <System.Threading.Mutex.hpp>
#include <queue>

namespace System::Threading::Tasks
{
    class TaskScheduler;
};
class System::Threading::Tasks::TaskScheduler
{
protected:
    Mutex mtx;
    TaskScheduler(){}
public:

};

#endif