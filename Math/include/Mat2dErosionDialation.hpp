/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-12-07 07:55:10
 * @LastEditors: like
 * @LastEditTime: 2021-12-10 07:41:59
 */
#ifndef MAT2D_EROSION_DIALATION_HPP
#define MAT2D_EROSION_DIALATION_HPP

#ifndef MAT2D_EROSION_DIALATION_DEBUG
#define MAT2D_EROSION_DIALATION_DEBUG
#undef MAT2D_EROSION_DIALATION_DEBUG
#endif
#include <Mat.hpp>
#include <Mat2dMakeBorder.hpp>
#include <Mat2dSmoothing.hpp>

/**
 * @brief 腐蚀操作, 通过直方图排序取最小值实现
 * 
 * @tparam T 
 * @param mat 原始矩阵
 * @param kernelWidth 矩形模板宽度
 * @param kernelHeight 矩形模板高度
 * @return Mat<T>* 腐蚀之后的矩阵
 */
template<typename T>
Mat<T>* Erosion(Mat<T>* mat, int kernelWidth, int kernelHeight)
{
    VALRET_ASSERT(mat && (kernelWidth& 1) && (kernelHeight & 1), NULL);
#define GET_MIN_VALUE(hist, val)\
    do                      \
    {                       \
        for(int i = 0; i < 256; i++)    \
        {                   \
            if(hist[i])     \
            {               \
                val = i;    \
                break;      \
            }               \
        }                   \
    }while(0)
    int tkHalfWidth  = kernelWidth >> 1;
    int tkHalfHeight = kernelHeight >> 1;
    Mat<T>* out = new Mat<T>(mat->w, mat->h);
    memset(out->p, 0, sizeof(T) * out->length);
    unsigned int hist[256];
    int meadianSize = 1;
    for
    (
        int tkRow = tkHalfHeight, top = 0, bottom = kernelHeight;
        tkRow < mat->h - tkHalfHeight; 
        tkRow++, top++, bottom++
    )
    {
        INIT_LEFT_BEGIN_HISTGRAM(mat, hist, top, 0, bottom, kernelWidth);
        for
        (
            int tkCol = tkHalfWidth, left = 0, right = kernelWidth - 1; 
            tkCol < mat->w - tkHalfWidth; 
            tkCol++, left++, right++
        )
        {
            MOVE_HISTGRAM_RIGHT(mat, hist, top, left, bottom, right);
#ifdef MAT2D_EROSION_DIALATION_DEBUG
            for(int i = 0; i < 256; i++)
            {
                if(hist[i])
                    printf("%d, ", i);
            }
            printf("\n");
#endif
            GET_MIN_VALUE(hist, out->p[tkRow * mat->w + tkCol]);
        }
    }

    return out;
}
/**
 * @brief 膨胀操作, 通过直方图排序取最大值实现
 * 
 * @tparam T 
 * @param mat 原始矩阵
 * @param kernelWidth 矩形模板宽度
 * @param kernelHeight 矩形模板高度
 * @return Mat<T>* 膨胀之后的矩阵
 */
template<typename T>
Mat<T>* Dialation(Mat<T>* mat, int kernelWidth, int kernelHeight)
{
    VALRET_ASSERT(mat && (kernelWidth& 1) && (kernelHeight & 1), NULL);
#define GET_MAX_VALUE(hist, val)\
    do                      \
    {                       \
        for(int i = 255; i > -1; i--)  \
        {                   \
            if(hist[i])     \
            {               \
                val = i;    \
                break;      \
            }               \
        }                   \
    }while(0)
    int tkHalfWidth  = kernelWidth >> 1;
    int tkHalfHeight = kernelHeight >> 1;
    Mat<T>* out = new Mat<T>(mat->w, mat->h);
    memset(out->p, 0, sizeof(T) * out->length);
    unsigned int hist[256];
    for
    (
        int tkRow = tkHalfHeight, top = 0, bottom = kernelHeight;
        tkRow < mat->h - tkHalfHeight; 
        tkRow++, top++, bottom++
    )
    {
        INIT_LEFT_BEGIN_HISTGRAM(mat, hist, top, 0, bottom, kernelWidth);
        for
        (
            int tkCol = tkHalfWidth, left = 0, right = kernelWidth - 1; 
            tkCol < mat->w - tkHalfWidth; 
            tkCol++, left++, right++
        )
        {
            MOVE_HISTGRAM_RIGHT(mat, hist, top, left, bottom, right);
#ifdef MAT2D_EROSION_DIALATION_DEBUG
            for(int i = 0; i < 256; i++)
            {
                if(hist[i])
                    printf("%d, ", i);
            }
            printf("\n");
#endif
            GET_MAX_VALUE(hist, out->p[tkRow * mat->w + tkCol]);
        }
    }

    return out;
}

#endif