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
        Undefined                       = 0     ,   //������ֵ��Ϊ�������ԣ��������Ӳ����ش�ֵ��
        RestartMarker                   = 110	,   //ָʾ��Ӧ����һ������������ǻظ��� ��״̬������˵���ı������û���������Ǻͷ�������ǡ�
        ServiceTemporarilyNotAvailable  = 120	,   //ָʾ�˷������ڲ����ã����Ժ�������������
        DataAlreadyOpen	                = 125	,   //ָʾ���������Ѵ򿪲�������Ĵ����ѿ�ʼ��
        OpeningData	                    = 150	,   //ָʾ���������ڴ��������ӡ�
        CommandOK	                    = 200	,   //ָʾ����ɹ���ɡ�
        CommandExtraneous	            = 202	,   //ָʾ������δִ�и������Ϊ��������
        DirectoryStatus	                = 212	,   //ָʾĿ¼״̬��
        FileStatus	                    = 213	,   //ָʾ�ļ�״̬��
        SystemType	                    = 215	,   //ָʾʹ�� Internet Assigned Numbers Authority��Internet ����������������� Assigned Numbers��������䣩�ĵ��з�����ϵͳ����ϵͳ�������ơ�
        SendUserCommand	                = 220	,   //ָʾ���������ܽ����û���¼������
        ClosingControl	                = 221	,   //ָʾ���������ڹرչ������ӡ�
        ClosingData	                    = 226	,   //ָʾ���������ڹر��������ӣ�����������ļ������ɹ���
        EnteringPassive	                = 227	,   //ָʾ���������ڽ��뱻��ģʽ��
        LoggedInProceed	                = 230	,   //ָʾ�û��ѵ�¼�ҿɷ������
        ServerWantsSecureSession	    = 234	,   //ָʾ���������ܿͻ���ָ���������֤���ƣ����Ұ�ȫ���ݽ�����ɡ�
        FileActionOK	                = 250	,   //ָʾ������ļ������ɹ���ɡ�
        PathnameCreated	                = 257	,   //ָʾ�����·�����Ѵ�����
        SendPasswordCommand	            = 331	,   //ָʾ��������Ҫ�ṩ���롣
        NeedLoginAccount	            = 332	,   //ָʾ��������Ҫ�ṩ��¼�ʻ���
        FileCommandPending	            = 350	,   //ָʾ������ļ�������Ҫ������Ϣ��
        ServiceNotAvailable	            = 421	,   //ָʾ�˷��񲻿��á�
        CantOpenData	                = 425	,   //ָʾ�޷����������ӡ�
        ConnectionClosed	            = 426	,   //ָʾ�����ѹرա�
        ActionNotTakenFileUnavailableOrBusy = 450	,   //ָ���޷���ָ���ļ�ִ������Ĳ�����ԭ���Ǹ��ļ������û�����ʹ���С�
        ActionAbortedLocalProcessingError	= 451	,   //ָʾ��������ֹ�����������Ĵ���
        ActionNotTakenInsufficientSpace	    = 452	,   //ָʾ����ִ������Ĳ�������Ϊ��������û���㹻�Ŀռ䡣
        CommandSyntaxError	            = 500	,   //ָʾ��������﷨������Ƿ�������ʶ������
        ArgumentSyntaxError	            = 501	,   //ָʾһ������������������﷨����
        CommandNotImplemented	        = 502	,   //ָʾ FTP ������δִ�и����
        BadCommandSequence	            = 503	,   //ָʾ�������е�˳����ȷ��
        NotLoggedIn	                    = 530	,   //ָʾ��¼��Ϣ���뷢�͵���������
        AccountNeeded	                = 532	,   //ָʾ��Ҫ�������ϵ��û��ʻ���
        ActionNotTakenFileUnavailable	= 550	,   //ָ���޷���ָ���ļ�ִ������Ĳ�����ԭ���Ǹ��ļ������á�
        ActionAbortedUnknownPageType	= 551	,   //ָʾ���ܲ�ȡ����Ĳ�������Ϊָ����ҳ������δ֪�ġ� RFC 959 �� 3.1.2.3 ���н�����ҳ���͡�
        FileActionAborted	            = 552	,   //ָʾ����ִ������Ĳ�����
        ActionNotTakenFilenameNotAllowed= 553	    //ָ���޷���ָ���ļ�ִ������Ĳ�����

    }FtpStatusCode;
    typedef enum _ConnectMode
    {
        Positive = 0,   /* ������������FTP�������ذ�������ģʽ���ױ�����ǽ�ɵ� */
        Negative        /* ������FTP��������Ϣ */
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
        NetBuf* ctrlConnBuffer; /*�����������*/
        NetBuf* dataConnBuffer; /*���ݴ�������*/
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