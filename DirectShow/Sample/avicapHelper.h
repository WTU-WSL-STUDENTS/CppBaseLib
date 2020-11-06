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
    int dwRequestMicroSecPerFrame;          // 期望的桢播放率，以毫秒为单位，默认为66667，相当于15桢每秒。
    bool fMakeUserHitOKToCapture;           // Show "Hit OK to cap" dlg?开始捕获标志位，如果值为真，则在开始捕获前要产生一个询问对话框，默认为假。
    unsigned int wPercentDropForError;      //所允许的最大丢桢百分比，可以从0变化到100，默认值为10。
    bool fYield;                            //另起线程标志位，如果为真，则程序重新启动一个线程用于视频流的捕获，默认值是假。但是如果你是为了真，你必须要在程序中处理一些潜在的操作，因为当视频捕获时，其他操作并没有被屏蔽。
    int dwIndexSize;                        // 在AVI文件中所允许的最大数目的索引项(32K)
    unsigned int wChunkGranularity;         // AVI文件的逻辑尺寸，以字节为单位。如果值是0，则说明该尺寸渐增 在 Win32程序中无用。(2K)
    bool fUsingDOSMemory;                   // Use DOS buffers?
    unsigned int wNumVideoRequested;        // 所被允许分配的最大视频缓存
    bool fCaptureAudio;                     // 音频标志位，如果音频流正在捕获，则该值为真。
    unsigned int wNumAudioRequested;        // 最大数量的音频缓存，默认值为10。
    unsigned int vKeyAbort;                 // 终止流捕获的虚拟键盘码，默认值为VK_ESCAPE
    bool fAbortLeftMouse;           // 终止鼠标左键标志位，如果该值为真，则在流捕获过程中如果点击鼠标左键则该捕获终止，默认值为真。
    bool fAbortRightMouse;          // Abort on right mouse?
    bool fLimitEnabled;             // 捕获操作时间限制，如果为真，则时间到了以后捕获操作终止，默认为假
    unsigned int wTimeLimit;        // 具体终止时间，只有 fLimitEnabled是真时.该位才有效
    bool fMCIControl;               // Use MCI video source?
    bool fStepMCIDevice;            // Step MCI device?MCI 设备标志。
    int dwMCIStartTime;             // Time to start in MS
    int dwMCIStopTime;              // Time to stop in MS
    bool fStepCaptureAt2x;          // Perform spatial averaging 2x
    int wStepCaptureAverageFrames;  // 当基于平均采样来创建桢时，桢的采样时间，典型值是5
    int dwAudioBufferSize;          // 音频缓存的尺寸，如果用默认值0，缓存尺寸是最大0.5秒，或10k字节。
    int fDisableWriteCache;         // Attempt to disable write cache
    int AVStreamMaster;             //音视频同步标志。
};

//相机的描述信息
class CamDescription
{
public:
    int cameraIndex;    //相机的设备ID
    int lpwstrLen;      //相机的描述信息的长度
    LPWSTR name;        //相机默认名称
    LPWSTR version;     //相机版本

    LPCWSTR widgetName;
    HWND dockerWidgetHandle;
    CamDescription(int camIdx, int strLen = 128) :cameraIndex(camIdx), lpwstrLen(strLen){}
protected:  //禁止无参实例化
    CamDescription() :CamDescription(-1) {}
};

/**
    if (SendMessage(capHandle, WM_CAP_DRIVER_CONNECT, 0, 0))
    {
        //-----------------------------------------------------------------------
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_DRIVER_GET_CAPS, AviCapture.SizeOf(CapDriverCAPS), ref CapDriverCAPS);//获得当前视频 CAPDRIVERCAPS定义了捕获驱动器的能力，如有无视频叠加能力、有无控制视频源、视频格式的对话框等；
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_GET_STATUS, AviCapture.SizeOf(CapStatus), ref CapStatus);//获得当前视频流的尺寸 存入CapStatus结构

        AviCapture.BITMAPINFO bitmapInfo = new AviCapture.BITMAPINFO();//设置视频格式 (height and width in pixels, bits per frame).
        bitmapInfo.bmiHeader = new AviCapture.BITMAPINFOHEADER();
        bitmapInfo.bmiHeader.biSize = AviCapture.SizeOf(bitmapInfo.bmiHeader);
        bitmapInfo.bmiHeader.biWidth = mWidth;
        bitmapInfo.bmiHeader.biHeight = mHeight;
        bitmapInfo.bmiHeader.biPlanes = 1;
        bitmapInfo.bmiHeader.biBitCount = 24;
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_PREVIEWRATE, 34, 0);//设置在PREVIEW模式下设定视频窗口的刷新率 设置每34毫秒显示一帧，即显示帧速为每秒29帧
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_SCALE, 1, 0);//打开预览视频的缩放比例
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_VIDEOFORMAT, AviCapture.SizeOf(bitmapInfo), ref bitmapInfo);


        AviCapture.CAPTUREPARMS captureparms = new AviCapture.CAPTUREPARMS();
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_GET_SEQUENCE_SETUP, AviCapture.SizeOf(captureparms), ref captureparms);
        if (CapDriverCAPS.fHasOverlay)
        {
            AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_OVERLAY, 1, 0);//启用叠加 注：据说启用此项可以加快渲染速度
        }
        AviCapture.SendMessage(this.lwndC, AviCapture.WM_CAP_SET_PREVIEW, 1, 0);//设置显示图像启动预览模式 PREVIEW
        AviCapture.SetWindowPos(this.lwndC, 0, 0, 0, mWidth, mHeight, AviCapture.SWP_NOZORDER | AviCapture.SWP_NOMOVE);//使捕获窗口与进来的视频流尺寸保持一致

    }
    else
    {
        System.Windows.Forms.MessageBox.Show("未能连接设备，请检查设备连接及是否有其他程序占用！");
        flag = false;

    }
**/

//查询当前的硬件驱动？0-9
static int GetDriverDescription(OUT CamDescription des,int idx){return capGetDriverDescription(des.cameraIndex, des.name, des.lpwstrLen, des.version, des.lpwstrLen);}

//创建相机句柄（在软件中虚拟的相机对象）
static CameraHandle OpenCamera(int x, int y, int nWidth, int nHeight, CamDescription des) { return    capCreateCaptureWindow(des.widgetName, WS_CHILD | WS_VISIBLE, x, y, nWidth, nHeight, des.dockerWidgetHandle, des.cameraIndex); }

//尝试打开相机
static int Connect(CameraHandle capHandle) { return SendMessage(capHandle, WM_CAP_DRIVER_CONNECT, 0, 0); }

//查询相机的参数
static int GetCameraInfo(CameraHandle capHandle, ICamreaInfo* params) { return SendMessage(capHandle, WM_CAP_GET_SEQUENCE_SETUP, sizeof(CamDescription), (LPARAM)params); }

//设置相机的参数
static int SetCamreaInfo(CameraHandle capHandle, ICamreaInfo* params) { SendMessage(capHandle, WM_CAP_SET_SEQUENCE_SETUP, sizeof(CamDescription), (LPARAM)params); }

//保存图片至指定路径
static int GrabImage(CameraHandle capHandle, LPARAM path) { return SendMessage(capHandle, WM_CAP_FILE_SAVEDIB, 0, path); }

//显示对话框 是否由新的图像尺寸?通知给父窗口告诉它我的尺寸改变了:
static int ShowDialog(CameraHandle capHandle) { return SendMessage(capHandle, WM_CAP_DLG_VIDEOFORMAT, 0, 0); }

//打开对话框 询问视频压缩格式
static int SetVideoCompression(CameraHandle lwndC) { return SendMessage(lwndC, WM_CAP_DLG_VIDEOCOMPRESSION, 0, 0); }

//开始录像
static void StartRecordVideo(CameraHandle lwndC,int path)
{
    SendMessage(lwndC, WM_CAP_FILE_SET_CAPTURE_FILE, 0, path);
    SendMessage(lwndC, WM_CAP_SEQUENCE, 0, 0);
}

//停止录像
static void StopRecordVideo(CameraHandle lwndC)
{
    SendMessage(lwndC, WM_CAP_STOP, 0, 0);
}

//关闭相机
static int CloseCamera(CameraHandle capHandle, CamDescription des) { return SendMessage(capHandle, WM_CAP_DRIVER_DISCONNECT, des.cameraIndex, 0); }





#endif // !avicapHelper
