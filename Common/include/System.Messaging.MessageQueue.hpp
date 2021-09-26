/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-13 15:27:42
 * @LastEditors: like
 * @LastEditTime: 2021-09-14 16:30:48
 */
#ifndef SYSTEM_MESSAGING_MESSAGEQUEUE_HPP
#define SYSTEM_MESSAGING_MESSAGEQUEUE_HPP

// https://docs.microsoft.com/zh-cn/windows/win32/winmsg/messages-and-message-queues?redirectedfrom=MSDN
#include <CompliedEntry.h>
#if defined(MSVC)
#include <winuser.h>
#pragma comment( lib, "user32.lib") 
#endif
#include <string>
#include <vector>

using namespace std;

namespace System::Messaging
{
/*
    Value	Meaning
    BSF_ALLOWSFW
    0x00000080
    Enables the recipient to set the foreground window while processing the message.
    BSF_FLUSHDISK
    0x00000004
    Flushes the disk after each recipient processes the message.
    BSF_FORCEIFHUNG
    0x00000020
    Continues to broadcast the message, even if the time-out period elapses or one of the recipients is not responding.
    BSF_IGNORECURRENTTASK
    0x00000002
    Does not send the message to windows that belong to the current task. This prevents an application from receiving its own message.
    BSF_NOHANG
    0x00000008
    Forces a nonresponsive application to time out. If one of the recipients times out, do not continue broadcasting the message.
    BSF_NOTIMEOUTIFNOTHUNG
    0x00000040
    Waits for a response to the message, as long as the recipient is not being unresponsive. Does not time out.
    BSF_POSTMESSAGE
    0x00000010
    Posts the message. Do not use in combination with BSF_QUERY.
    BSF_QUERY
    0x00000001
    Sends the message to one recipient at a time, sending to a subsequent recipient only if the current recipient returns TRUE.
    BSF_SENDNOTIFYMESSAGE
    0x00000100
    Sends the message using SendNotifyMessage function. Do not use in combination with BSF_QUERY.
*/
    typedef unsigned int Message;
    typedef struct _MsgInfo
    {
        WPARAM wParam;
        LPARAM pParam;
    }MsgInfo;

    typedef enum _BroadcastType
    {
        AllComponents   = 0,    /* 向所有组件进行广播       */
        AllApplications = 0x08, /* 向所有应用程序进行广播   */
        AllDesktops     = 0x10  /* 向桌面应用进行广播       */
    }BroadcastType;
    bool SendBroadcast(Message msg, DWORD flags, BroadcastType type, const MsgInfo& info)
    {
        if(BroadcastSystemMessage(flags, (DWORD*)&type, msg, info.wParam, info.pParam))
        {
            return true;
        }
        printf("Broadcast Message Failed, Error Code : %d\n", GetLastError());
        return false;
    }
    class MessageQueue;
}
class System::Messaging::MessageQueue
{
public:
    MessageQueue(string path){}
    /**
     * @description: 
     * @param { 控件的ID或者高位底位组合起来分别表示鼠标的位置 } WPARAM
     * @param { 某种结构的指针或者是某种类型的句柄 } LPARAM
     * @return {*}
     * @author: like
     */    
    // void SendSystemMessage(WPARAM wParam/* */, LPARAM lParam)/* 16位操作系统遗留下来的产物 https://baike.baidu.com/item/WParam/6098975 */
    // {
    //     BroadcastSystemMessage(wParam, lParam);
    // }
};
#endif