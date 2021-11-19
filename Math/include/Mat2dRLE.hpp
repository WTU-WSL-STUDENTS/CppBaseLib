/*
 * @Description: 矩阵转游程编码
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-18 06:45:01
 * @LastEditors: like
 * @LastEditTime: 2021-11-18 06:46:22
 */
#ifndef MAT2D_RLE_HPP
#define MAT2D_RLE_HPP

#include <System.List.hpp>
#include <Mat.hpp>

// #define MAT2D_RLES_DEBUG

typedef unsigned long long BinaryRle; /* 最高位记录Label，32位平台最多支持记录 2 ^ 63 次方长度 */
#define GET_BINARY_RLE_LABEL(rle)   ((int)(rle >> (sizeof(BinaryRle) * 8 - 1)))
#define GET_BINARY_RLE_LENGTH(rle)  ((rle << 1) >> 1)
#define SET_NEW_BINARY_RLE_VAL(val) ((((BinaryRle)val) << (sizeof(BinaryRle) * 8 - 1)) + 1)
typedef System::List<BinaryRle> RLES;

/* 矩阵转二值游程编码 */
RLES* TransToRLE(Mat<unsigned char>* mat)
{
    VALRET_ASSERT(mat, NULL);
    RLES* rles = new RLES(mat->h * 8);/* 初始容量预期是每行 8 次 0 - 1 跳变 */
    BinaryRle rle = 0;
    unsigned char *p = mat->p;
    bool currentLabel;
    for(int r = 0; r < mat->h; r++)
    {
        if(rle)/* 存在换行截断的连续数据区 */
        {
            rles->Add(rle);
        }
        currentLabel = *p;
        rle = SET_NEW_BINARY_RLE_VAL(*p++);
        for(int c = 1; c < mat->w; c++, p++)
        {
#ifdef MAT2D_RLES_DEBUG
            printf("%d, %lld\n", currentLabel, rle);        
#endif
            if(currentLabel)/* 1 */
            {
                if(*p)
                {
                    rle++;/* 没有进行越界判断 */
                }
                else
                {
                    rles->Add(rle);
                    currentLabel = 0;
                    rle = SET_NEW_BINARY_RLE_VAL(0);
                }
            }
            else /* 0 */
            {
                if(0 == *p)
                {
                    rle++;/* 没有进行越界判断 */
                }
                else
                {
                    rles->Add(rle);
                    currentLabel = 1;
                    rle = SET_NEW_BINARY_RLE_VAL(1);
                }
            }
        }
    }
    if(rle)/* 存在换行截断的连续数据区 */
    {
        rles->Add(rle);
    }
    return rles;
}

#endif