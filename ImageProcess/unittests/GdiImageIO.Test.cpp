/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-04 16:09:48
 * @LastEditors: like
 * @LastEditTime: 2021-10-07 00:10:52
 */
#include <cv/GdiImageIO.hpp>
 
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
int main(int argc, char* argv[])
{
    MatBGR* img = img = new Mat<MatKernel24Bit>(570, 544);
    CV_BYTE* p = (CV_BYTE*)img->p;
    for(size_t i = 0; i < img->length * 3; i++)
    {
        p[i] = (CV_BYTE)i;
    }
    printf("%d, %d, %d, %d, %s, %s\n", 
        img->h,
        img->w,
        img->RowAt(img->h - 1)->R,
        img->RowAt(img->h - 1)->G,
        ToString<MatKernel24Bit>(img->RowAt(img->h - 1)).c_str(), 
        ToString<MatKernel24Bit>(img->RowAt(img->h) - 1).c_str()
    );
    int gdiError;
    if(GdiStatus::Ok != (gdiError = EnableGdi()))
    {
        printf("Enable Gdi Interface Failed, Error %d\n", gdiError);
    }
    else
    {
        printf("Enable Gdi Interface Sucess\n");
    }
    if(!ReadImage<MatKernel24Bit>(img, argv[1]))
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
    if(!WriteImage<MatKernel24Bit>(img, "test.bmp"))
    {
        printf("WriteImage Failed\n");
    }  
    else
    {
        printf("WriteImage Sucess\n");
    }
    DisableGdi();
    delete img;
    img = NULL;  
    return 0;
}
