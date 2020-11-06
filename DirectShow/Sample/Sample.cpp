// Sample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//
//https://blog.csdn.net/weixin_30443731/article/details/95829164
//




//[DllImport("avicap32.dll")]//包含了执行视频捕获的函数，它给AVI文件I/O和视频、音频设备驱动程序提供一个高级接口
//public static extern IntPtr capCreateCaptureWindow(string lpszWindowName, int dwStyle, int x, int y, int nWidth, int nHeight, IntPtr hwndParent, int nID);
//[DllImport("AVICAP32.dll", CharSet = CharSet.Unicode)]
//public static extern bool capGetDriverDescription(int wDriverIndex, StringBuilder lpszName, int cbName, StringBuilder lpszVer, int cbVer);
//[DllImport("User32.dll")]
//public static extern bool SendMessage(IntPtr hWnd, int wMsg, bool wParam, int lParam);
//[DllImport("User32.dll")]
//public static extern bool SendMessage(IntPtr hWnd, int wMsg, short wParam, int lParam);
//[DllImport("User32.dll")]
//public static extern bool SendMessage(IntPtr hWnd, int wMsg, int wParam, int lParam);
//[DllImport("User32.dll")]
//public static extern bool SendMessage(IntPtr hWnd, int wMsg, short wParam, FrameEventHandler lParam);
//[DllImport("User32.dll")]
//public static extern bool SendMessage(IntPtr hWnd, int wMsg, int wParam, ref BITMAPINFO lParam);
//[DllImport("User32.dll")]
//public static extern bool SendMessage(IntPtr hWnd, int wMsg, int wParam, ref CAPDRIVERCAPS lParam);
//[DllImport("User32.dll")]
//public static extern bool SendMessage(IntPtr hWnd, int wMsg, int wParam, ref CAPTUREPARMS lParam);
//[DllImport("User32.dll")]
//public static extern bool SendMessage(IntPtr hWnd, int wMsg, int wParam, ref CAPSTATUS lParam);
//[DllImport("User32.dll")]
//public static extern int SetWindowPos(IntPtr hWnd, int hWndInsertAfter, int x, int y, int cx, int cy, int wFlags);
//[DllImport("avicap32.dll")]
//public static extern int capGetVideoFormat(IntPtr hWnd, IntPtr psVideoFormat, int wSize);
//




//VideoHdr 结构   定义了视频数据块的头信息，在编写回调函数时常用到其数据成员lpData（指向数据缓存的指针）和dwBufferLength（数据缓存的大小）。     
//视频帧到缓存的捕获则需要应用回调函数和相应的数据块结构 VIDEOHDR 
struct VIDEOHDR
{
public:
    //IntPtr lpData;                 /* 指向数据缓存的指针 */
    int dwBufferLength;         /* 数据缓存的大小 */
    int dwBytesUsed;            /* Bytes actually used */
    int dwTimeCaptured;         /* Milliseconds from start of stream */
    int dwUser;                 /* for client's use */
    int dwFlags;                /* assorted flags (see defines) */
    int dwReserved;          /* reserved for driver */
};


#include <iostream>


//#pragma comment(lib,"avicap32.dll")
int main()
{

    //hWndC = capCreateCaptureWindow(
    //    TEXT("My Capture Window"),   // window name if pop-up 
    //    WS_CHILD | WS_VISIBLE,       // window style 
    //    0, 0, 160, 120,              // window position and dimensions
    //    (HWND)hwndParent,
    //    (int)nID /* child ID */);WM_CAP_GET_CAPSTREAMPTR
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
