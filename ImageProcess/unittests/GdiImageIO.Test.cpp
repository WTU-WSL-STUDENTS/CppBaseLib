/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-04 16:09:48
 * @LastEditors: like
 * @LastEditTime: 2021-10-08 21:52:04
 */
#include <System.Debug.Dump.hpp>
#include <cv/GdiImageIO.hpp>
#include <stdio.h>

const char *GetEventMessage(DWORD dwCtrlType)
{
    switch ( dwCtrlType )
    {
    case CTRL_C_EVENT:
        DisableGdi();
        return "CTRL_C_EVENT";
 
    case CTRL_BREAK_EVENT:
        return "CTRL_BREAK_EVENT";
 
    case CTRL_CLOSE_EVENT:
        return "CTRL_CLOSE_EVENT";
 
    case CTRL_LOGOFF_EVENT:
        return "CTRL_LOGOFF_EVENT";
 
    case CTRL_SHUTDOWN_EVENT:
        return "CTRL_SHUTDOWN_EVENT";
    }
 
    return "Unknown";
}
int GdiReadWrite(const char* srcpath, const char* destpath)
{
    MatBGR* img = img = new Mat<MatKernel24Bit>(300, 200);
    printf("%d, %d, %d, %d, %s, %s\n", 
        img->h,
        img->w,
        img->RowAt(img->h - 1)->R,
        img->RowAt(img->h - 1)->G,
        ToString<MatKernel24Bit>(img->RowAt(img->h - 1)).c_str(), 
        ToString<MatKernel24Bit>(img->RowAt(img->h) - 1).c_str()
    );

    if(!ReadImage<MatKernel24Bit>(img, srcpath))
    {
        printf("Read Image Failed\n");
        DisableGdi();
        return 1;
    }
    else
    {
        printf("Read Image Sucess\n");
    }
    printf("%d, %d, %s, %s\n", 
        img->h,
        img->w,
        ToString<MatKernel24Bit>(img->RowAt(img->h - 1)).c_str(), 
        ToString<MatKernel24Bit>(img->RowAt(img->h - 1) + img->w - 1).c_str()
    );
    char buffer[255] = {0};
    strcat(buffer, destpath);
    strcat(buffer, "\\GdiImageIO.Test.GdiReadWrite.300_200_24.bmp");
    if(!WriteImage<MatKernel24Bit>(img, destpath))
    {
        printf("WriteImage Failed\n");
    }  
    else
    {
        printf("WriteImage Sucess\n");
    }
    delete img;
    img = NULL;  
    return 0;
}
template<typename T>
void KernelAdd(T& val, int rowIndex)
{
    val = (T)(rowIndex);
}
template<>
void KernelAdd<MatKernel32Bit>(MatKernel32Bit& val, int rowIndex)
{
    val = {(CV_BYTE)rowIndex, (CV_BYTE)rowIndex, (CV_BYTE)rowIndex, 0};
}
template<>
void KernelAdd<MatKernel24Bit>(MatKernel24Bit& val, int rowIndex)
{
    val = {(CV_BYTE)rowIndex, (CV_BYTE)rowIndex, (CV_BYTE)rowIndex};
}
template<typename T>
int GdiCreateImage(const char* destpath, const T& initVal)
{
    Mat<T>* img = img = new Mat<T>(300, 256);
    printf("%d, %d, %s, %s\n", 
        img->h,
        img->w,
        ToString<T>(img->RowAt(img->h - 1)).c_str(), 
        ToString<T>(img->RowAt(img->h) - 1).c_str()
    );
    img->Init(initVal);
    T* p = img->p;
    for(int r = 0; r < img->h; r++)
    {
        for(int c = 0; c < img->w; c++)
        {
            KernelAdd(*p, r);
            p++;
        }
        printf("%d\n", r);
    }
    char buffer[255] = {0};
    sprintf(buffer, "%s\\GdiImageIO.Test.GdiCreateImage.%d_%d_%d.bmp", destpath, img->w, img->h, 8 * sizeof(T));
    if(!WriteImage<T>(img, buffer))
    {
        printf("WriteImage Failed\n");
    }  
    else
    {
        printf("WriteImage Sucess\n");
    }
    delete img;
    img = NULL;  
    return 0;
    
}
int main(int argc, char* argv[])
{
    // SetUnhandledExceptionFilter(ExceptionFilter);
    int gdiError;
    if(GdiStatus::Ok != (gdiError = EnableGdi()))
    {
        printf("Enable Gdi Interface Failed, Error %d\n", gdiError);
    }
    else
    {
        printf("Enable Gdi Interface Sucess\n");
    }
    (strrchr(argv[0], '\\'))[0] = 0; // 删除文件名，只获得路径字串//
    char inPath[255] = {0};
    memcpy(inPath, argv[1], strlen(argv[1]));
    strcat(inPath, "\\300_200_24.bmp");
    // GdiReadWrite(inPath, argv[0]);
    GdiCreateImage<MatKernel32Bit>(argv[0], MatKernel32Bit{0, 0, 255, 0});
    GdiCreateImage<MatKernel24Bit>(argv[0], MatKernel24Bit{0, 0, 255});
    GdiCreateImage<MatKernel16Bit>(argv[0], 0xff0000);
    DisableGdi();
    return 0;
}
