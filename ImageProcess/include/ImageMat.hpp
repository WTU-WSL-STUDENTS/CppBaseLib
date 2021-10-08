/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-04 10:21:11
 * @LastEditors: like
 * @LastEditTime: 2021-10-08 16:16:30
 */
#ifndef IMAGE_MAT_HPP
#define IMAGE_MAT_HPP

#define _CRT_SECURE_NO_WARNINGS
#include <Mat.hpp>

#define IN
#define OUT
#define IN_OUT IN OUT

typedef unsigned char CV_BYTE;
#pragma pack(1)

typedef struct _ColorBGRA
{
    CV_BYTE B;
    CV_BYTE G;
    CV_BYTE R;
    CV_BYTE A;
}ColorBGRA;

typedef struct _ColorBGR
{
    CV_BYTE B;
    CV_BYTE G;
    CV_BYTE R;
}ColorBGR;
#pragma pack()

/* 定义图像矩阵核类型 */
typedef CV_BYTE         MatKernel8Bit;
typedef unsigned short  MatKernel16Bit;
typedef unsigned short  MatKernel555;
typedef unsigned short  MatKernel565; 
typedef unsigned short  MatKernel1555; 
typedef ColorBGR        MatKernel24Bit;
typedef ColorBGRA       MatKernel32Bit;

/* 定义矩阵类型 */
typedef Mat<MatKernel8Bit>  MatGray;
typedef Mat<MatKernel24Bit> MatBGR;
typedef Mat<MatKernel32Bit> MatBGRA;

#if !(defined(ENABLE_MAT_KERNEL_555) || defined(ENABLE_MAT_KERNEL_565) || defined(ENABLE_MAT_KERNEL_1555))
#define ENABLE_MAT_KERNEL_555
#endif
#if   defined(ENABLE_MAT_KERNEL_555)
    /* 解析16位位图rgb分量 : https://blog.csdn.net/xqhrs232/article/details/8022711 */
    inline void  DecomposeMatKernel16Bit(const MatKernel16Bit& val, ColorBGR& pixel)
    {
        pixel = {(CV_BYTE)(val & 0x001f), (CV_BYTE)(val & 0x03e0), (CV_BYTE)(val & 0x7c00)};
    }
    template<>
    inline std::string ToString(const MatKernel555* t) /* 16位位图ToString只支持 555数据格式 */
    {
        ColorBGR d;
        DecomposeMatKernel16Bit(*t, d);
        char buffer[24] = {0};
        sprintf(buffer, "(%d, %d, %d)", d.R, d.G, d.B);
        return string(buffer);
    }
#elif defined(ENABLE_MAT_KERNEL_565)
    /* 565的调色盘的位置包含掩码 3*4 ~ 4*4个字节*/
    inline void  DecomposeMatKernel16Bit(const MatKernel16Bit& val, ColorBGR& pixel)
    {
        pixel = {(CV_BYTE)(val & 0x001f), (CV_BYTE)(val & 0x07e0), (CV_BYTE)(val & 0xf800)};
    }
    template<>
    inline std::string ToString(const MatKernel565* t)
    {
        ColorBGR d;
        DecomposeMatKernel16Bit(*t, d);
        char buffer[24] = {0};
        sprintf(buffer, "(%d, %d, %d)", d.R, d.G, d.B);
        return string(buffer);
    }
#elif defined(ENABLE_MAT_KERNEL_1555)
    /* 1,5,5,5的 BGRA,有2^5 * 2^5 * 2^5个调色盘 */
    inline void  DecomposeMatKernel16Bit(const MatKernel16Bit& val, ColorBGRA& pixel)
    {
        pixel = {(CV_BYTE)(val & 0x001f), (CV_BYTE)(val & 0x03e0), (CV_BYTE)(val & 0x7c00), (CV_BYTE)(val & 0x8000)};
    }
    template<>
    inline std::string ToString(const MatKernel1555* t)
    {
        ColorBGRA d;
        DecomposeMatKernel16Bit(*t, d);
        char buffer[24] = {0};
        sprintf(buffer, "(%d, %d, %d, %d)", d.R, d.G, d.B, d.A);
        return string(buffer);
    }
#endif
template<>
inline std::string ToString<MatKernel24Bit>(const MatKernel24Bit* t)
{
    char buffer[24] = {0};
    sprintf(buffer, "(%d, %d, %d)", t->R, t->G, t->B);
    return string(buffer);
}
template<>
inline std::string ToString<MatKernel32Bit>(const MatKernel32Bit* t)
{
    char buffer[24] = {0};
    sprintf(buffer, "(%d, %d, %d, %d)", t->R, t->G, t->B, t->A);
    return string(buffer);
}

#pragma pack(push, 2)
typedef struct _BitmapFileHeader
{
    unsigned short bfType;  /*42 4d*/
    unsigned long bfSize;   /*整个文件大小*/
    unsigned int bfReserved;/*占位*/
    unsigned long bfOffset; /*整个头文件大小*/
}BitmapFileHeader, PBitmapFileHeader;
#pragma pack(pop)
typedef struct _BitmapInfoHeader
{
    unsigned long biSize;           /* BitmapInfoHeader在文件中占用的字节（不存在字节对齐） = 40*/
    long biWidth;
    long biHeight;
    unsigned short biPlanes;        /* 位平面深度1 */
    unsigned short biBitcount;      /* 位深度 */
    unsigned long biCompression;    /* 压缩类型 */
    unsigned long biSizeImage;      /* 位图大小 */
    long biXPelsPerMeter;           /* 0 */
    long biYPelsPerMeter;           /* 0 */
    unsigned long biClrUsed;        /* 标准颜色:0 自定义颜色：自定义颜色数量 */
    unsigned long biClrImportant;   /* 0 */
}BitmapInfoHeader;
typedef ColorBGRA RGBAQuad;
typedef struct _BitmapInfo : BitmapInfoHeader
{
    RGBAQuad palette[1];
}BitmapInfo, *PBitmapInfo;

#endif