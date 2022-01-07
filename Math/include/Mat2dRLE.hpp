/*
 * @Description: ����ת�γ̱���
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-18 06:45:01
 * @LastEditors: like
 * @LastEditTime: 2021-12-22 14:49:46
 */
#ifndef MAT2D_RLE_HPP
#define MAT2D_RLE_HPP

#include <System.List.hpp>
#include <Mat.hpp>

// #define MAT2D_RLES_DEBUG

typedef unsigned long long BinaryRle; /* ���λ��¼Label��32λƽ̨���֧�ּ�¼ 2 ^ 63 �η����� */
#define GET_BINARY_RLE_LABEL(rle)   ((int)(rle >> (sizeof(BinaryRle) * 8 - 1)))
#define GET_BINARY_RLE_LENGTH(rle)  ((rle << 1) >> 1)
#define SET_NEW_BINARY_RLE_VAL(val) ((((BinaryRle)val) << (sizeof(BinaryRle) * 8 - 1)) + 1)
typedef System::List<BinaryRle> RLES;

/**
 * @brief ����ת��ֵ�γ̱���
 * 000111
 * 111000
 * ->
 * {0,3}{1,3}{1,3}{0,3}
 * @param mat �������
 * @return RLES* �γ̱���ָ��
 */
RLES* TransToRLE(Mat<unsigned char>* mat)
{
    VALRET_ASSERT(mat, NULL);
    RLES* rles = new RLES(mat->h * 8);/* ��ʼ����Ԥ����ÿ�� 8 �� 0 - 1 ���� */
    BinaryRle rle = 0;
    unsigned char *p = mat->p;
    bool currentLabel;
    for(int r = 0; r < mat->h; r++)
    {
        if(rle)/* ���ڻ��нضϵ����������� */
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
                    rle++;/* û�н���Խ���ж� */
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
                    rle++;/* û�н���Խ���ж� */
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
    if(rle)/* ���ڻ��нضϵ����������� */
    {
        rles->Add(rle);
    }
    return rles;
}

#if !defined(RLE_NEIGHBOURHOOD_4) && !defined(RLE_NEIGHBOURHOOD_8)
#define RLE_NEIGHBOURHOOD_8
#elif defined(RLE_NEIGHBOURHOOD_4) && defined(RLE_NEIGHBOURHOOD_8)
#undef RLE_NEIGHBOURHOOD_4
#endif

/**
 * @brief bwlabelʵ��
 * 
 * @param mat �������
 * @return System::List<RLES*> ��ͨ�򼯺� 
 */
System::List<RLES*> BwLabel(Mat<unsigned char>* mat)
{
    VALRET_ASSERT(mat, NULL);
    System::List<System::List<int>*>* tagList = new System::List<System::List<int>*>();
    
    /* ˮƽ���� */
    unsigned char *p = mat->p;
    for(int r = 0 ; r < mat->h; r++)
    {
        for(int c = 0; c < mat->w; c++)
        {
            
        }
    }

    System::IEnumerator<System::List<int>*>* it = tagList->GetEnumerator();
    while(it->MoveNext())
    {
        (*it->Current())->
    }
    delete tagList;
}
#endif