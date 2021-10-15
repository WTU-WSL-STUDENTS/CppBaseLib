/*
 * @Description: �γ̱���
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-12 16:24:32
 * @LastEditors: like
 * @LastEditTime: 2021-10-14 14:23:00
 */
#ifndef RLE_HPP
#define RLE_HPP
#include <ImageMat.hpp>

typedef unsigned long BinaryRle; /* ���λ��¼ֵ��32λƽ̨���֧�� 2^31�η�����, ���֧�� 2^16(65536) * 2^15(32768)�Ķ�ֵͼ�� */

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