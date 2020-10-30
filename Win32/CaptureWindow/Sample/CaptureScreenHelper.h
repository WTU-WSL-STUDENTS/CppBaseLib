#pragma once

#include <iostream>
#include "windows.h"

namespace CaptureScreenUlt {
    int SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);
    HBITMAP GetCaptureBmp(int startX = 0, int startY = 0, int width = 1280, int height = 720);
};
/************************************************************************/
/* hBitmap    为刚才的屏幕位图句柄
/* lpFileName 为需要保存的位图文件名
/************************************************************************/
int CaptureScreenUlt::SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName)
{
    HDC hDC;        //设备描述表
    int iBits;      //当前显示分辨率下每个像素所占字节数
    WORD wBitCount; //位图中每个像素所占字节数    
    DWORD dwPaletteSize = 0;    //定义调色板大小
    DWORD dwBmBitsSize;         //位图中像素字节大小
    DWORD dwDIBSize;    // 位图文件大小
    DWORD dwWritten;    //写入文件字节数
    BITMAP Bitmap;      //位图结构
    BITMAPFILEHEADER   bmfHdr;  //位图属性结构   
    BITMAPINFOHEADER   bi;      //位图文件头结构
    LPBITMAPINFOHEADER lpbi;    //位图信息头结构     指向位图信息头结构
    HANDLE fh;      //定义文件句柄
    HANDLE hDib;    //分配内存句柄
    HANDLE hPal;    //分配内存句柄
    HANDLE hOldPal = NULL;  //调色板句柄  

    //计算位图文件每个像素所占字节数   
    hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);

    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else if (iBits <= 24)
        wBitCount = 24;
    else if (iBits <= 32)
        wBitCount = 24;


    //计算调色板大小   
    if (wBitCount <= 8)
        dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);



    //设置位图信息头结构   
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap.bmWidth;
    bi.biHeight = Bitmap.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

    //为位图内容分配内存   
    hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    if (lpbi == NULL)
    {
        return 0;
    }

    *lpbi = bi;
    // 处理调色板
    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = GetDC(NULL);
        hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }
    // 获取该调色板下新的像素值   
    GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
        (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
        (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
    //恢复调色板      
    if (hOldPal)
    {
        SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }
    //创建位图文件       
    fh = CreateFile(lpFileName, GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    // 设置位图文件头   
    bmfHdr.bfType = 0x4D42;  // "BM"   
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    // 写入位图文件头   
    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    // 写入位图文件其余内容   
    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

    //清除      
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);

    return 1;
}

/// <summary>
/// 截图
/// </summary>
/// <param name="startX">屏幕中起点X</param>
/// <param name="startY">屏幕中起点Y</param>
/// <param name="width">截图的屏幕宽，也是返回值图片的宽</param>
/// <param name="height">截图的屏幕高，也是返回值图片的高</param>
/// <returns></returns>
HBITMAP CaptureScreenUlt::GetCaptureBmp(int startX, int startY,int width,int height)
{
    HDC     hDC;
    HDC     MemDC;
    BYTE* Data;
    HBITMAP   hBmp;
    BITMAPINFO   bi;
   
    memset(&bi, 0, sizeof(bi));
    bi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bi.bmiHeader.biWidth = width;//GetSystemMetrics(SM_CXSCREEN);
    bi.bmiHeader.biHeight = height;// GetSystemMetrics(SM_CYSCREEN);
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 24;
    hDC = GetDC(NULL);  //hdcScreen 
    MemDC = CreateCompatibleDC(hDC);
    hBmp = CreateDIBSection(MemDC, &bi, DIB_RGB_COLORS, (void**)&Data, NULL, NULL);
    SelectObject(MemDC, hBmp);
    BitBlt(MemDC, 0,0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, hDC, startX, startY, SRCCOPY);
    ReleaseDC(NULL, hDC);
    DeleteDC(MemDC);
    return   hBmp;
}

/// <summary>
/// 全屏截屏
/// </summary>
/// <param name="saveToWhere"> 文件路径，保存至C盘需要管理员权限,例如：L"D:\\11.bmp"</param>
/// <returns></returns>
int DoCapture(LPCWSTR saveToWhere) {
    return CaptureScreenUlt::SaveBitmapToFile(
        CaptureScreenUlt::GetCaptureBmp(
            0,0,
            GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)),
            saveToWhere
    );
}

/// <summary>
/// 自定义区域截图
/// </summary>
/// <param name="saveToWhere"></param>
/// <param name="top"></param>
/// <param name="left"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
int DoCapture(LPCWSTR saveToWhere, int top, int left, int width, int height)
{
    return CaptureScreenUlt::SaveBitmapToFile(CaptureScreenUlt::GetCaptureBmp(left, top, width, height), saveToWhere);
}
