/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-04 14:20:56
 * @LastEditors: like
 * @LastEditTime: 2021-10-07 00:08:53
 */
#ifndef IMAGE_IO_HPP
#define IMAGE_IO_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <io.h>
#include <windows.h>
#include <gdiplus.h>
#include <ImageMat.hpp>
#include <map>

using namespace std;
using namespace Gdiplus;

std::wstring StoWs(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
wstring StoWs(const char* szIn)
{
    int length = MultiByteToWideChar(CP_ACP, 0, szIn, -1, NULL, 0);
    WCHAR* buf = new WCHAR[length + 1];
    ZeroMemory(buf, (length + 1) * sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP, 0, szIn, -1, buf, length);
    std::wstring strRet(buf);
    delete[] buf;
    return strRet;
}
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // ???????
    UINT  size = 0;         // ?????????
    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
    Gdiplus::GetImageEncodersSize(&num, &size);    // ???????
    if (size == 0)
        return -1;
    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;
    GetImageEncoders(num, size, pImageCodecInfo);    // ??????????
    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)    // ??????????CLSID?*pClsid
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }
    free(pImageCodecInfo);
    return -1;
}
enum GdiStatus
{
    Ok = 0,
    GenericError = 1,
    InvalidParameter = 2,
    OutOfMemory = 3,
    ObjectBusy = 4,
    InsufficientBuffer = 5,
    NotImplemented = 6,
    Win32Error = 7,
    WrongState = 8,
    Aborted = 9,
    FileNotFound = 10,
    ValueOverflow = 11,
    AccessDenied = 12,
    UnknownImageFormat = 13,
    FontFamilyNotFound = 14,
    FontStyleNotFound = 15,
    NotTrueTypeFont = 16,
    UnsupportedGdiplusVersion = 17,
    GdiplusNotInitialized = 18,
    PropertyNotFound = 19,
    PropertyNotSupported = 20,
#if (GDIPVER >= 0x0110)
    ProfileNotFound = 21,
#endif //(GDIPVER >= 0x0110)
};

ULONG_PTR gdiplustoken;
int EnableGdi()
{
    GdiplusStartupInput gdiplusstartupinput;
    return GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);
}
void DisableGdi()
{
    GdiplusShutdown(gdiplustoken);
}
std::map<const char*, int> gdiFormatMapping = 
{
    {typeid(MatKernel8Bit).name()   , PixelFormat8bppIndexed},
    {typeid(MatKernel555).name()    , PixelFormat16bppRGB555},
    {typeid(MatKernel24Bit).name()  , PixelFormat24bppRGB},
    {typeid(MatKernel32Bit).name()  , PixelFormat32bppARGB}
};
template<typename T>
bool ReadImage(Mat<T>*  img, const char* filepath)
{
    if(_access(filepath, 0))
        return false;
    wstring infilename = StoWs(filepath);
    Bitmap* bmp = Bitmap::FromFile(infilename.c_str());
    UINT height = bmp->GetHeight();
    UINT width = bmp->GetWidth();
    if(height != img->h && width != img->w)
    {
        return false;
    }
    Gdiplus::BitmapData bmpData;
    int gdiError =  bmp->LockBits(&Gdiplus::Rect(0, 0, width, height), Gdiplus::ImageLockModeWrite, bmp->GetPixelFormat(), &bmpData); 
    if(GdiStatus::Ok != gdiError)
    {
        printf("LockBits Failed, Error %d\n", gdiError);
        return false;
    }
    CV_BYTE* srcPtr = (CV_BYTE*)bmpData.Scan0;
    size_t rowByteCount = img->w * sizeof(T);
    for(UINT i = 0;i < height;i++)
    {
        memcpy(img->RowAt(i), srcPtr, rowByteCount);
        srcPtr  += bmpData.Stride;
    }
    bmp->UnlockBits(&bmpData);
    delete bmp;
    bmp = NULL;
    return true;
}
template<typename T>
bool WriteImage(Mat<T>* img, const char* filepath)
{
    int width  = img->w;
    int height = img->h;
    Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(width, height, gdiFormatMapping[typeid(T).name()]);
    Gdiplus::BitmapData bmpData;
    bmp->LockBits(&Gdiplus::Rect(0, 0, width, height), Gdiplus::ImageLockModeWrite, bmp->GetPixelFormat(), &bmpData); 
    CV_BYTE* destPtr  = (CV_BYTE*)bmpData.Scan0;
    size_t rowLength = width * sizeof(T);
    for(int i = 0; i < height; i++)
    {
        memcpy(destPtr, img->RowAt(i), rowLength);
        destPtr  += bmpData.Stride;
    }
    bmp->UnlockBits(&bmpData);
    CLSID pngClsid;
    GetEncoderClsid(L"image/bmp", &pngClsid);
    wstring infilename = StoWs(filepath);
    bmp->Save(infilename.c_str(), &pngClsid, NULL); 
    delete bmp;
    bmp = NULL;
    return 0 == _access(filepath, 0);  
}

#endif