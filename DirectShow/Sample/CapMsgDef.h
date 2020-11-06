#pragma once
#ifndef CapMsgDef
#define CapMsgDef

const int WM_START = 0x400;    //此并非摄像头消息0x400表示的就是1024  
//const int WS_CHILD = 0x40000000L;
//const int WS_VISIBLE = 0x10000000L;
//const int SWP_NOMOVE = 0x2;
//const int SWP_NOZORDER = 0x4;
const int WM_CAP_GET_CAPSTREAMPTR = WM_START + 1;
const int WM_CAP_SET_CALLBACK_ERROR = WM_START + 2;//设置收回错误
const int WM_CAP_SET_CALLBACK_STATUS = WM_START + 3;//设置收回状态
const int WM_CAP_SET_CALLBACK_YIELD = WM_START + 4;//设置收回出产
const int WM_CAP_SET_CALLBACK_FRAME = WM_START + 5;//设置收回结构
const int WM_CAP_SET_CALLBACK_VIDEOSTREAM = WM_START + 6;//设置收回视频流
const int WM_CAP_SET_CALLBACK_WAVESTREAM = WM_START + 7;//设置收回视频波流
const int WM_CAP_GET_USER_DATA = WM_START + 8;//获得使用者数据
const int WM_CAP_SET_USER_DATA = WM_START + 9;//设置使用者数据
const int WM_CAP_DRIVER_CONNECT = WM_START + 10;//驱动程序连接
const int WM_CAP_DRIVER_DISCONNECT = WM_START + 11;//断开启动程序连接
const int WM_CAP_DRIVER_GET_NAME = WM_START + 12;//获得驱动程序名字
const int WM_CAP_DRIVER_GET_VERSION = WM_START + 13;//获得驱动程序版本
const int WM_CAP_DRIVER_GET_CAPS = WM_START + 14;//获得驱动程序帽子
const int WM_CAP_FILE_SET_CAPTURE_FILE = WM_START + 20;//设置捕获文件
const int WM_CAP_FILE_GET_CAPTURE_FILE = WM_START + 21;//获得捕获文件
const int WM_CAP_FILE_ALLOCATE = WM_START + 22;//分派文件
const int WM_CAP_FILE_SAVEAS = WM_START + 23;//另存文件为
const int WM_CAP_FILE_SET_INFOCHUNK = WM_START + 24;//设置开始文件
const int WM_CAP_FILE_SAVEDIB = WM_START + 25;//保存文件
const int WM_CAP_EDIT_COPY = WM_START + 30;//编辑复制
const int WM_CAP_SET_AUDIOFORMAT = WM_START + 35;//设置音频格式
const int WM_CAP_GET_AUDIOFORMAT = WM_START + 36;//捕获音频格式
const int WM_CAP_DLG_VIDEOFORMAT = WM_START + 41;//1065 打开视频格式设置对话框
const int WM_CAP_DLG_VIDEOSOURCE = WM_START + 42;//1066 打开属性设置对话框，设置对比度亮度等
const int WM_CAP_DLG_VIDEODISPLAY = WM_START + 43;//1067 打开视频显示
const int WM_CAP_GET_VIDEOFORMAT = WM_START + 44;//1068 获得视频格式
const int WM_CAP_SET_VIDEOFORMAT = WM_START + 45;//1069 设置视频格式
const int WM_CAP_DLG_VIDEOCOMPRESSION = WM_START + 46;//1070 打开压缩设置对话框
const int WM_CAP_SET_PREVIEW = WM_START + 50;//设置预览
const int WM_CAP_SET_OVERLAY = WM_START + 51;//设置覆盖
const int WM_CAP_SET_PREVIEWRATE = WM_START + 52;//设置预览比例
const int WM_CAP_SET_SCALE = WM_START + 53;//设置刻度
const int WM_CAP_GET_STATUS = WM_START + 54;//获得状态
const int WM_CAP_SET_SCROLL = WM_START + 55;//设置卷
const int WM_CAP_GRAB_FRAME = WM_START + 60;//逮捕结构
const int WM_CAP_GRAB_FRAME_NOSTOP = WM_START + 61;//停止逮捕结构
const int WM_CAP_SEQUENCE = WM_START + 62;//次序
const int WM_CAP_SEQUENCE_NOFILE = WM_START + 63;//使用WM_CAP_SEUENCE_NOFILE消息（capCaptureSequenceNoFile宏），可以不向磁盘文件写入数据。该消息仅在配合回调函数时有用，它允许你的应用程序直接使用音视频数据。
const int WM_CAP_SET_SEQUENCE_SETUP = WM_START + 64;//设置安装次序
const int WM_CAP_GET_SEQUENCE_SETUP = WM_START + 65;//获得安装次序
const int WM_CAP_SET_MCI_DEVICE = WM_START + 66;//设置媒体控制接口
const int WM_CAP_GET_MCI_DEVICE = WM_START + 67;//获得媒体控制接口 
const int WM_CAP_STOP = WM_START + 68;//停止
const int WM_CAP_ABORT = WM_START + 69;//异常中断
const int WM_CAP_SINGLE_FRAME_OPEN = WM_START + 70;//打开单一的结构
const int WM_CAP_SINGLE_FRAME_CLOSE = WM_START + 71;//关闭单一的结构
const int WM_CAP_SINGLE_FRAME = WM_START + 72;//单一的结构
const int WM_CAP_PAL_OPEN = WM_START + 80;//打开视频
const int WM_CAP_PAL_SAVE = WM_START + 81;//保存视频
const int WM_CAP_PAL_PASTE = WM_START + 82;//粘贴视频
const int WM_CAP_PAL_AUTOCREATE = WM_START + 83; //自动创造
const int WM_CAP_PAL_MANUALCREATE = WM_START + 84;//手动创造
const int WM_CAP_SET_CALLBACK_CAPCONTROL = WM_START + 85;// 设置收回的错误


#endif // !CapMsgDef
