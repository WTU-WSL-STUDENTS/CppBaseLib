#ifndef SYSTEM_NET_FTP_WEB_REQUEST_HPP
#define SYSTEM_NET_FTP_WEB_REQUEST_HPP

#define _CRT_SECURE_NO_DEPRECATE
#include <CompliedEntry.h>
extern "C"
{
    #include <ftplib.h>
}
#include <stdio.h>
#include <map>
#include <string>

#define FTP_MAX_BUFFER_SIZE 1024

/* https://docs.microsoft.com/zh-cn/dotnet/api/system.net.ftpstatuscode?view=net-5.0 */
namespace System::Net
{
    typedef enum _FtpStatusCode
    {
        Undefined                       = 0     ,   //包含此值是为了完整性，服务器从不返回此值。
        RestartMarker                   = 110	,   //指示响应包含一个重新启动标记回复。 此状态附带的说明文本包含用户数据流标记和服务器标记。
        ServiceTemporarilyNotAvailable  = 120	,   //指示此服务现在不可用；请稍后再试您的请求。
        DataAlreadyOpen	                = 125	,   //指示数据连接已打开并且请求的传输已开始。
        OpeningData	                    = 150	,   //指示服务器正在打开数据连接。
        CommandOK	                    = 200	,   //指示命令成功完成。
        CommandExtraneous	            = 202	,   //指示服务器未执行该命令，因为无需此命令。
        DirectoryStatus	                = 212	,   //指示目录状态。
        FileStatus	                    = 213	,   //指示文件状态。
        SystemType	                    = 215	,   //指示使用 Internet Assigned Numbers Authority（Internet 号码分配机构）出版的 Assigned Numbers（号码分配）文档中发布的系统名的系统类型名称。
        SendUserCommand	                = 220	,   //指示服务器已能进行用户登录操作。
        ClosingControl	                = 221	,   //指示服务器正在关闭管理连接。
        ClosingData	                    = 226	,   //指示服务器正在关闭数据连接，并且请求的文件操作成功。
        EnteringPassive	                = 227	,   //指示服务器正在进入被动模式。
        LoggedInProceed	                = 230	,   //指示用户已登录且可发送命令。
        ServerWantsSecureSession	    = 234	,   //指示服务器接受客户端指定的身份验证机制，并且安全数据交换完成。
        FileActionOK	                = 250	,   //指示请求的文件操作成功完成。
        PathnameCreated	                = 257	,   //指示请求的路径名已创建。
        SendPasswordCommand	            = 331	,   //指示服务器需要提供密码。
        NeedLoginAccount	            = 332	,   //指示服务器需要提供登录帐户。
        FileCommandPending	            = 350	,   //指示请求的文件操作需要其他信息。
        ServiceNotAvailable	            = 421	,   //指示此服务不可用。
        CantOpenData	                = 425	,   //指示无法打开数据连接。
        ConnectionClosed	            = 426	,   //指示连接已关闭。
        ActionNotTakenFileUnavailableOrBusy = 450	,   //指定无法对指定文件执行请求的操作，原因是该文件不可用或正在使用中。
        ActionAbortedLocalProcessingError	= 451	,   //指示发生了阻止完成请求操作的错误。
        ActionNotTakenInsufficientSpace	    = 452	,   //指示不能执行请求的操作，因为服务器上没有足够的空间。
        CommandSyntaxError	            = 500	,   //指示命令具有语法错误或不是服务器可识别的命令。
        ArgumentSyntaxError	            = 501	,   //指示一个或多个命令参数具有语法错误。
        CommandNotImplemented	        = 502	,   //指示 FTP 服务器未执行该命令。
        BadCommandSequence	            = 503	,   //指示命令序列的顺序不正确。
        NotLoggedIn	                    = 530	,   //指示登录信息必须发送到服务器。
        AccountNeeded	                = 532	,   //指示需要服务器上的用户帐户。
        ActionNotTakenFileUnavailable	= 550	,   //指定无法对指定文件执行请求的操作，原因是该文件不可用。
        ActionAbortedUnknownPageType	= 551	,   //指示不能采取请求的操作，因为指定的页类型是未知的。 RFC 959 的 3.1.2.3 节中介绍了页类型。
        FileActionAborted	            = 552	,   //指示不能执行请求的操作。
        ActionNotTakenFilenameNotAllowed= 553	    //指定无法对指定文件执行请求的操作。

    }FtpStatusCode;
    typedef enum _ConnectMode
    {
        Positive = 0,   /* 主动发包，等FTP服务器回包，这种模式容易被防火墙干掉 */
        Negative        /* 被动等FTP服务求发消息 */
    }ConnectMode;
    typedef enum _TransportMode
    {
        Text = 0,
        Binary
    }TransportMode;
    typedef struct _FtpInfo
    {       
        char* ip;
        char* port;
        char* user;
        char* psw;
        ConnectMode connectMode;
        TransportMode transportMode;
        NetBuf* ctrlConnBuffer; /*命令控制连接*/
        NetBuf* dataConnBuffer; /*数据传输连接*/
    }FtpInfo;
    class FtpWebRequest;
}

class System::Net::FtpWebRequest
{
private:
    FtpInfo info;
public:
    FtpWebRequest(char* ip, char* port, char* user = "", char* psw = "") : 
        info({ip, port, user, psw, 0 == strcmp("", port) ? Negative : Positive, Text, NULL, NULL}){::FtpInit();}
    ~FtpWebRequest(){Close();}
    bool Connect(int timeout = 30000)
    {
        if(!info.ip || NULL == info.port)
        {
            return NULL;
        }
        char host[1024] = {0};
        char* str = strcat(host, info.ip);
        if(Positive == info.connectMode)
        {
            str = strcat(str, ":");
            str = strcat(str, info.port);
        }
        printf("%s", str);
        return 1 == ::FtpConnect(str, &info.ctrlConnBuffer, timeout);
    }
    void DisConnect()
    {
        if(!info.ctrlConnBuffer)
        {
            return;
        }
	    ::FtpQuit(info.ctrlConnBuffer);
	    info.ctrlConnBuffer = NULL;
    }
    bool Login()
    {
        if(!info.ctrlConnBuffer)
        {
            return false;
        }
	    return 1 == ::FtpLogin(info.user, info.psw, info.ctrlConnBuffer);
    }
    void Close()
    {
        if( info.ctrlConnBuffer)
        {
		    FtpQuit(info.ctrlConnBuffer);
        }
        if( info.dataConnBuffer)
        {
		    FtpQuit(info.dataConnBuffer);
        }
    }
    char* GetResponse()
    {
        return ::FtpLastResponse(info.ctrlConnBuffer);
    }

    bool ChangeDir(const char* destPath)
    {
        return 1 == FtpChdir(destPath, info.ctrlConnBuffer);
    }
    bool GoLastDir()
    {
        return 1 == FtpCDUp(info.ctrlConnBuffer);
    }
    bool MakeDir(const char* strMakePath)
    {
        return 1 == FtpMkdir(strMakePath, info.ctrlConnBuffer);
    }
    bool RemoveDir(const char* strMakePath)
    {
        return 1 == FtpRmdir(strMakePath, info.ctrlConnBuffer);
    }
    char* CurrentDir()
    {
	    char buf[FTP_MAX_BUFFER_SIZE] = {0};
        char* p = buf;
        int ret = FtpPwd(p, FTP_MAX_BUFFER_SIZE, info.ctrlConnBuffer);
        return 1 == ret ? p : NULL;
    }
    char* ListDir(char* dirPath = NULL)
    {
        char buf[FTP_MAX_BUFFER_SIZE] = {0};
        char* p = buf;
        int ret =  FtpDir(p, dirPath, info.ctrlConnBuffer);
        return p;
    }
    bool DownloadRemoteFileToLocal(const char* remotePath, const char* toLocalPath, TransportMode mode = Text)
    {
        return  1 == FtpGet(toLocalPath, remotePath, mode, info.ctrlConnBuffer);
    }
    bool UploadLocalFileToRemote(const char* toRemotePath, const char* LocalPath, TransportMode mode = Text)
    {
        return  1 == FtpPut(LocalPath, toRemotePath, mode, info.ctrlConnBuffer);
    }
    bool Delete(const char* remotePath)
    {
        return 1 == FtpDelete(remotePath, info.ctrlConnBuffer);
    }
    bool RenameRemoteFile(const char* now, const char* desire, TransportMode mode = Text)
    {
        return  1 == FtpRename(now, desire, info.ctrlConnBuffer);
    }
    size_t FileSize(const char* remotePath, TransportMode mode = Text)
    {
        unsigned int size;
        return 1 == FtpSize(remotePath, &size, mode, info.ctrlConnBuffer) ? size : 0xffffffff;
    }
    bool SetCallBack(FtpCallback func, void* args,unsigned int argsLen, unsigned int spendedMillSecond)
    {
        FtpCallbackOptions tOpt;
        tOpt.cbFunc = func;
        tOpt.cbArg = args;
        tOpt.bytesXferred = argsLen;
        tOpt.idleTime = spendedMillSecond;
        return 1 == FtpSetCallback(&tOpt, info.ctrlConnBuffer);
    }
    void ClearCallBack(){FtpClearCallback(info.ctrlConnBuffer);}
};

#endif