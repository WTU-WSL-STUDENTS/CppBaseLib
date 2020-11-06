#ifndef avicapHelper
#define avicapHelper

//https://docs.microsoft.com/zh-cn/windows/win32/api/vfw/nf-vfw-capgetdriverdescriptiona?redirectedfrom=MSDN&f1url=%3FappId%3DDev16IDEF1%26l%3DZH-CN%26k%3Dk(VFW%252FcapGetDriverDescription);k(capGetDriverDescription);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue

#define CameraHandle HWND
#include <Windows.h>
#include "CapMsgDef.h"
#include <Vfw.h>

//https://docs.microsoft.com/en-us/windows/win32/api/vfw/ns-vfw-captureparms
class ICamreaInfo
{
public:
    int dwRequestMicroSecPerFrame;          // �������岥���ʣ��Ժ���Ϊ��λ��Ĭ��Ϊ66667���൱��15��ÿ�롣
    bool fMakeUserHitOKToCapture;           // Show "Hit OK to cap" dlg?��ʼ�����־λ�����ֵΪ�棬���ڿ�ʼ����ǰҪ����һ��ѯ�ʶԻ���Ĭ��Ϊ�١�
    unsigned int wPercentDropForError;      //������������ٷֱȣ����Դ�0�仯��100��Ĭ��ֵΪ10��
    bool fYield;                            //�����̱߳�־λ�����Ϊ�棬�������������һ���߳�������Ƶ���Ĳ���Ĭ��ֵ�Ǽ١������������Ϊ���棬�����Ҫ�ڳ����д���һЩǱ�ڵĲ�������Ϊ����Ƶ����ʱ������������û�б����Ρ�
    int dwIndexSize;                        // ��AVI�ļ���������������Ŀ��������(32K)
    unsigned int wChunkGranularity;         // AVI�ļ����߼��ߴ磬���ֽ�Ϊ��λ�����ֵ��0����˵���óߴ罥�� �� Win32���������á�(2K)
    bool fUsingDOSMemory;                   // Use DOS buffers?
    unsigned int wNumVideoRequested;        // �����������������Ƶ����
    bool fCaptureAudio;                     // ��Ƶ��־λ�������Ƶ�����ڲ������ֵΪ�档
    unsigned int wNumAudioRequested;        // �����������Ƶ���棬Ĭ��ֵΪ10��
    unsigned int vKeyAbort;                 // ��ֹ���������������룬Ĭ��ֵΪVK_ESCAPE
    bool fAbortLeftMouse;           // ��ֹ��������־λ�������ֵΪ�棬�����������������������������ò�����ֹ��Ĭ��ֵΪ�档
    bool fAbortRightMouse;          // Abort on right mouse?
    bool fLimitEnabled;             // �������ʱ�����ƣ����Ϊ�棬��ʱ�䵽���Ժ󲶻������ֹ��Ĭ��Ϊ��
    unsigned int wTimeLimit;        // ������ֹʱ�䣬ֻ�� fLimitEnabled����ʱ.��λ����Ч
    bool fMCIControl;               // Use MCI video source?
    bool fStepMCIDevice;            // Step MCI device?MCI �豸��־��
    int dwMCIStartTime;             // Time to start in MS
    int dwMCIStopTime;              // Time to stop in MS
    bool fStepCaptureAt2x;          // Perform spatial averaging 2x
    int wStepCaptureAverageFrames;  // ������ƽ��������������ʱ����Ĳ���ʱ�䣬����ֵ��5
    int dwAudioBufferSize;          // ��Ƶ����ĳߴ磬�����Ĭ��ֵ0������ߴ������0.5�룬��10k�ֽڡ�
    int fDisableWriteCache;         // Attempt to disable write cache
    int AVStreamMaster;             //����Ƶͬ����־��
};

//�����������Ϣ
class CamDescription
{
public:
    int cameraIndex;    //������豸ID
    int lpwstrLen;      //�����������Ϣ�ĳ���
    LPWSTR name;        //���Ĭ������
    LPWSTR version;     //����汾

    LPCWSTR widgetName;
    HWND dockerWidgetHandle;
    CamDescription(int camIdx, int strLen = 128) :cameraIndex(camIdx), lpwstrLen(strLen){}
protected:  //��ֹ�޲�ʵ����
    CamDescription() :CamDescription(-1) {}
};

/**
    if (SendMessage(capHandle, WM_CAP_DRIVER_CONNECT, 0, 0))
    {
        //-----------------------------------------------------------------------
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_DRIVER_GET_CAPS, AviCapture.SizeOf(CapDriverCAPS), ref CapDriverCAPS);//��õ�ǰ��Ƶ CAPDRIVERCAPS�����˲�������������������������Ƶ�������������޿�����ƵԴ����Ƶ��ʽ�ĶԻ���ȣ�
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_GET_STATUS, AviCapture.SizeOf(CapStatus), ref CapStatus);//��õ�ǰ��Ƶ���ĳߴ� ����CapStatus�ṹ

        AviCapture.BITMAPINFO bitmapInfo = new AviCapture.BITMAPINFO();//������Ƶ��ʽ (height and width in pixels, bits per frame).
        bitmapInfo.bmiHeader = new AviCapture.BITMAPINFOHEADER();
        bitmapInfo.bmiHeader.biSize = AviCapture.SizeOf(bitmapInfo.bmiHeader);
        bitmapInfo.bmiHeader.biWidth = mWidth;
        bitmapInfo.bmiHeader.biHeight = mHeight;
        bitmapInfo.bmiHeader.biPlanes = 1;
        bitmapInfo.bmiHeader.biBitCount = 24;
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_PREVIEWRATE, 34, 0);//������PREVIEWģʽ���趨��Ƶ���ڵ�ˢ���� ����ÿ34������ʾһ֡������ʾ֡��Ϊÿ��29֡
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_SCALE, 1, 0);//��Ԥ����Ƶ�����ű���
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_VIDEOFORMAT, AviCapture.SizeOf(bitmapInfo), ref bitmapInfo);


        AviCapture.CAPTUREPARMS captureparms = new AviCapture.CAPTUREPARMS();
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_GET_SEQUENCE_SETUP, AviCapture.SizeOf(captureparms), ref captureparms);
        if (CapDriverCAPS.fHasOverlay)
        {
            AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_OVERLAY, 1, 0);//���õ��� ע����˵���ô�����Լӿ���Ⱦ�ٶ�
        }
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_PREVIEW, 1, 0);//������ʾͼ������Ԥ��ģʽ PREVIEW
        AviCapture.SetWindowPos(this.lwndC, 0, 0, 0, mWidth, mHeight, AviCapture.SWP_NOZORDER | AviCapture.SWP_NOMOVE);//ʹ���񴰿����������Ƶ���ߴ籣��һ��

    }
    else
    {
        System.Windows.Forms.MessageBox.Show("δ�������豸�������豸���Ӽ��Ƿ�����������ռ�ã�");
        flag = false;

    }
**/

//��ѯ��ǰ��Ӳ��������0-9
static int GetDriverDescription(OUT CamDescription des,int idx){return capGetDriverDescription(des.cameraIndex, des.name, des.lpwstrLen, des.version, des.lpwstrLen);}

//�������������������������������
static CameraHandle OpenCamera(int x, int y, int nWidth, int nHeight, CamDescription des) { return    capCreateCaptureWindow(des.widgetName, WS_CHILD | WS_VISIBLE, x, y, nWidth, nHeight, des.dockerWidgetHandle, des.cameraIndex); }

//���Դ����
static int Connect(CameraHandle capHandle) { return SendMessage(capHandle, WM_CAP_DRIVER_CONNECT, 0, 0); }

//��ѯ����Ĳ���
static int GetCameraInfo(CameraHandle capHandle, ICamreaInfo* params) { return SendMessage(capHandle, WM_CAP_GET_SEQUENCE_SETUP, sizeof(CamDescription), (LPARAM)params); }

//��������Ĳ���
static int SetCamreaInfo(CameraHandle capHandle, ICamreaInfo* params) { SendMessage(capHandle, WM_CAP_SET_SEQUENCE_SETUP, sizeof(CamDescription), (LPARAM)params); }

//����ͼƬ��ָ��·��
static int GrabImage(CameraHandle capHandle, LPARAM path) { return SendMessage(capHandle, WM_CAP_FILE_SAVEDIB, 0, path); }

//��ʾ�Ի��� �Ƿ����µ�ͼ��ߴ�?֪ͨ�������ڸ������ҵĳߴ�ı���:
static int ShowDialog(CameraHandle capHandle) { return SendMessage(capHandle, WM_CAP_DLG_VIDEOFORMAT, 0, 0); }

//�򿪶Ի��� ѯ����Ƶѹ����ʽ
static int SetVideoCompression(CameraHandle lwndC) { return SendMessage(lwndC, WM_CAP_DLG_VIDEOCOMPRESSION, 0, 0); }

//��ʼ¼��
static void StartRecordVideo(CameraHandle lwndC,int path)
{
    SendMessage(lwndC, WM_CAP_FILE_SET_CAPTURE_FILE, 0, path);
    SendMessage(lwndC, WM_CAP_SEQUENCE, 0, 0);
}

//ֹͣ¼��
static void StopRecordVideo(CameraHandle lwndC)
{
    SendMessage(lwndC, WM_CAP_STOP, 0, 0);
}

//�ر����
static int CloseCamera(CameraHandle capHandle, CamDescription des) { return SendMessage(capHandle, WM_CAP_DRIVER_DISCONNECT, des.cameraIndex, 0); }





#endif // !avicapHelper
