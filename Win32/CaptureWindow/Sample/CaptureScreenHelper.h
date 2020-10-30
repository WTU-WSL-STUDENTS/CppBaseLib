#pragma once

#include <iostream>
#include "windows.h"

namespace CaptureScreenUlt {
    int SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);
    HBITMAP GetCaptureBmp(int startX = 0, int startY = 0, int width = 1280, int height = 720);
};
/************************************************************************/
/* hBitmap    Ϊ�ղŵ���Ļλͼ���
/* lpFileName Ϊ��Ҫ�����λͼ�ļ���
/************************************************************************/
int CaptureScreenUlt::SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName)
{
    HDC hDC;        //�豸������
    int iBits;      //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
    WORD wBitCount; //λͼ��ÿ��������ռ�ֽ���    
    DWORD dwPaletteSize = 0;    //�����ɫ���С
    DWORD dwBmBitsSize;         //λͼ�������ֽڴ�С
    DWORD dwDIBSize;    // λͼ�ļ���С
    DWORD dwWritten;    //д���ļ��ֽ���
    BITMAP Bitmap;      //λͼ�ṹ
    BITMAPFILEHEADER   bmfHdr;  //λͼ���Խṹ   
    BITMAPINFOHEADER   bi;      //λͼ�ļ�ͷ�ṹ
    LPBITMAPINFOHEADER lpbi;    //λͼ��Ϣͷ�ṹ     ָ��λͼ��Ϣͷ�ṹ
    HANDLE fh;      //�����ļ����
    HANDLE hDib;    //�����ڴ���
    HANDLE hPal;    //�����ڴ���
    HANDLE hOldPal = NULL;  //��ɫ����  

    //����λͼ�ļ�ÿ��������ռ�ֽ���   
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


    //�����ɫ���С   
    if (wBitCount <= 8)
        dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);



    //����λͼ��Ϣͷ�ṹ   
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

    //Ϊλͼ���ݷ����ڴ�   
    hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    if (lpbi == NULL)
    {
        return 0;
    }

    *lpbi = bi;
    // �����ɫ��
    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = GetDC(NULL);
        hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }
    // ��ȡ�õ�ɫ�����µ�����ֵ   
    GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
        (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
        (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
    //�ָ���ɫ��      
    if (hOldPal)
    {
        SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }
    //����λͼ�ļ�       
    fh = CreateFile(lpFileName, GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    // ����λͼ�ļ�ͷ   
    bmfHdr.bfType = 0x4D42;  // "BM"   
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    // д��λͼ�ļ�ͷ   
    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    // д��λͼ�ļ���������   
    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

    //���      
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);

    return 1;
}

/// <summary>
/// ��ͼ
/// </summary>
/// <param name="startX">��Ļ�����X</param>
/// <param name="startY">��Ļ�����Y</param>
/// <param name="width">��ͼ����Ļ��Ҳ�Ƿ���ֵͼƬ�Ŀ�</param>
/// <param name="height">��ͼ����Ļ�ߣ�Ҳ�Ƿ���ֵͼƬ�ĸ�</param>
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
/// ȫ������
/// </summary>
/// <param name="saveToWhere"> �ļ�·����������C����Ҫ����ԱȨ��,���磺L"D:\\11.bmp"</param>
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
/// �Զ��������ͼ
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
