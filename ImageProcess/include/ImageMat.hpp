/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-04 10:21:11
 * @LastEditors: like
 * @LastEditTime: 2021-10-13 17:09:36
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

/* ����ͼ���������� */
typedef struct _MatKernel1Bit
{
    unsigned int d;
    inline int GetBit(int index/* 0 - 31 */)
    {
        return (d & (1 << index)) >> index;
    }
}MatKernel1Bit;
typedef CV_BYTE         MatKernel8Bit;
typedef unsigned short  MatKernel16Bit;
typedef MatKernel16Bit  MatKernel555;
typedef MatKernel16Bit  MatKernel565; 
typedef MatKernel16Bit  MatKernel1555; 
typedef ColorBGR        MatKernel24Bit;
typedef ColorBGRA       MatKernel32Bit;

/* ����������� */
typedef Mat<MatKernel8Bit>  MatGray;
typedef Mat<MatKernel24Bit> MatBGR;
typedef Mat<MatKernel32Bit> MatBGRA;

#if !(defined(ENABLE_MAT_KERNEL_555) || defined(ENABLE_MAT_KERNEL_565) || defined(ENABLE_MAT_KERNEL_1555))
#define ENABLE_MAT_KERNEL_555
#endif

template<>
inline std::string ToString<MatKernel16Bit>(const MatKernel16Bit* t)
{
    char buffer[24] = {0};
    if(t)
    {
#if   defined(ENABLE_MAT_KERNEL_555)
        /* ����16λλͼrgb���� : https://blog.csdn.net/xqhrs232/article/details/8022711 */
        ColorBGR d = {(CV_BYTE)(*t & 0x001f), (CV_BYTE)(*t & 0x03e0), (CV_BYTE)(*t & 0x7c00)};
        sprintf(buffer, "(%d, %d, %d)", d.R, d.G, d.B);
#elif defined(ENABLE_MAT_KERNEL_565)
        /* 565�ĵ�ɫ�̵�λ�ð������� 3*4 ~ 4*4���ֽ�*/
        ColorBGR d = {(CV_BYTE)(*t & 0x001f), (CV_BYTE)(*t & 0x07e0), (CV_BYTE)(*t & 0xf800)};
        sprintf(buffer, "(%d, %d, %d)", d.R, d.G, d.B);
#elif defined(ENABLE_MAT_KERNEL_1555)
        /* 1,5,5,5�� BGRA,��2^5 * 2^5 * 2^5����ɫ�� */
        ColorBGRA d = {(CV_BYTE)(*t & 0x001f), (CV_BYTE)(*t & 0x03e0), (CV_BYTE)(*t & 0x7c00), (CV_BYTE)(*t & 0x8000)};
        sprintf(buffer, "(%d, %d, %d, %d)", d.R, d.G, d.B, d.A);
#else
    Error(Not Defined MatKernel16Bit Type)
#endif
    }
    return string(buffer);
}
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
    unsigned long bfSize;   /*�����ļ���С*/
    unsigned int bfReserved;/*ռλ*/
    unsigned long bfOffset; /*����ͷ�ļ���С*/
}BitmapFileHeader, PBitmapFileHeader;
#pragma pack(pop)
typedef struct _BitmapInfoHeader
{
    unsigned long biSize;           /* BitmapInfoHeader���ļ���ռ�õ��ֽڣ��������ֽڶ��룩 = 40*/
    long biWidth;
    long biHeight;
    unsigned short biPlanes;        /* λƽ�����1 */
    unsigned short biBitcount;      /* λ��� */
    unsigned long biCompression;    /* ѹ������, 0����ѹ��;1��RLE8 8�����γ̱��룬ֻ����8λλͼ;2��RLE4 4�����γ̱��룬ֻ����4λλͼ; BI_BITFIELDS ������BLE����ֻ����16/32λλͼ  */
    unsigned long biSizeImage;      /* λͼ��С, biStride * biHeight �����biCompression��ΪBI_RGBʱ��ֵ����Ϊ0 */
    long biXPelsPerMeter;           /* 0/0x0ec4 */
    long biYPelsPerMeter;           /* 0/0x0ec4 */
    unsigned long biClrUsed;        /* ��׼��ɫ:0 �Զ�����ɫ���Զ�����ɫ���� */
    unsigned long biClrImportant;   /* 0 */
}BitmapInfoHeader;
typedef ColorBGRA RGBAQuad;
typedef struct _BitmapInfo : BitmapInfoHeader
{
    RGBAQuad palette[1];
}BitmapInfo, *PBitmapInfo;

#endif