/*
 * @Description: 游程编码
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-12 16:24:32
 * @LastEditors: like
 * @LastEditTime: 2021-10-14 14:23:00
 */
#ifndef RLE_HPP
#define RLE_HPP
#include <ImageMat.hpp>

typedef unsigned long BinaryRle; /* 最高位记录值，32位平台最多支持 2^31次方长度, 最多支持 2^16(65536) * 2^15(32768)的二值图像 */

BinaryRle* RleBitCompress(Mat<MatKernel8Bit>* mat, size_t &rlcLen)
{
    BinaryRle* rlc = NULL;
    if(NULL == mat || 1 > mat->length)
    {
        return rlc;
    }
    MatKernel8Bit* p = mat->p;
    size_t rlcSize = mat->length * sizeof(BinaryRle);
    rlc = (BinaryRle*)malloc(rlcSize);
    memset(rlc, 0, rlcSize);
    rlcLen = 1;
    int bitVal = *p++;
    *rlc = bitVal ? 0x80000001 : 1;
    for(size_t i = 1; i < mat->length; i++)
    {
        if(bitVal == *p)
        {
            (*rlc)++;
            printf("%u\n", *rlc);
        }
        else
        {
            printf("Move Next\n");
            rlcLen++;
            rlc++;
            bitVal = *p;
            *rlc = bitVal ? 0x80000001 : 1;
        }
        p++;
    }
    return rlc;
}

#endif