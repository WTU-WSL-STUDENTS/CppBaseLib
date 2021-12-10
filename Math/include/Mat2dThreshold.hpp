/*
 * @Description: 二维矩阵阈值分割
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-09 08:22:32
 * @LastEditors: like
 * @LastEditTime: 2021-12-07 08:01:24
 */
#ifndef MAT2D_THRESHOLD_HPP
#define MAT2D_THRESHOLD_HPP

#include <Mat.hpp>
#include <Mat2dHistogram.hpp>

// #define MAT2D_THRESHOLD_DEBUG

/** refrence : https://blog.csdn.net/liyuanbhu/article/details/49387483
 * 缺陷 : 当目标与背景的大小比例悬殊时，类间方差准则函数可能呈现双峰或多峰，此时效果不好
*/
bool AutoThreshold(Mat<unsigned char>* mat, int& thresholdVal)
{
    double hist[HISTOGRAM_LENGTH];
    VALRET_ASSERT(NormalizedHistogram(mat, hist), false);
#ifdef MAT2D_THRESHOLD_DEBUG
    for(int i = 0; i < HISTOGRAM_LENGTH; i++)
    {
        printf("(%d, %lf) \n", i, hist[i]);
    }
#endif
    double cdf[HISTOGRAM_LENGTH];   /* 累积分布函数 */
    cdf[0] = hist[0];
    double gpa[HISTOGRAM_LENGTH];   /* 0~i 加权平均加权平均 */
    gpa[0] = 0;
    for(int i = 1; i < HISTOGRAM_LENGTH; i++)
    {
        cdf[i] = hist[i - 1] + hist[i];
        gpa[i] = gpa [i - 1] + i * hist[i];
    }
    double mean = gpa[255];
    double icv  = 0;
    int icvIndex= 0;
    for(int i = 1; i < HISTOGRAM_LENGTH; i++)
    {
        double PA = hist[i];
        double PB = 1 - PA;
        if( PA > 0.001 && PA < 0.999)
        {
            double MA   = gpa[i] / PA;
            double MB   = mean - gpa[i]; 
            double _icv = PA * (MA - mean) * (MA - mean) + PB * (MB - mean) * (MB - mean);
            if(_icv > icv)
            {
                icv      = _icv;
                icvIndex = i;
            }
        }
    }
    thresholdVal = icvIndex;
    return true;
}
/* pass : [0, t) */
bool ThresholdLowPass(Mat<unsigned char>* mat, int t)
{
    VALRET_ASSERT(mat, false);
    unsigned char* p = mat->p;
    for(size_t i = 0; i < mat->length; i++, p++)
    {
        if(*p > t)
        {
            *p = 255;
        }
    }
    return true;
}
/* pass : (t, 255] */
bool ThresholdHighPass(Mat<unsigned char>* mat, int t)
{
    VALRET_ASSERT(mat, false);
    unsigned char* p = mat->p;
    for(size_t i = 0; i < mat->length; i++, p++)
    {
        if(*p < t)
        {
            *p = 0;
        }
    }
    return true;
}
/* pass : [min, max] */
bool Threshold(Mat<unsigned char>* mat, unsigned char min, unsigned char max)
{
    VALRET_ASSERT(mat, false);
    unsigned char* p = mat->p;
    for(size_t i = 0; i < mat->length; i++, p++)
    {
        if(min > *p || *p > max)
        {
            *p = 0;
        }
    }
    return true;
}
unsigned char RealBinaryValue = 255; /* ThresholdToBinary 中 0 - N 分布的 N 的值, 默认为 255 */
/* pass : [0, t) */
bool ThresholdLowPassToBinary(Mat<unsigned char>* mat, int t)
{
    VALRET_ASSERT(mat, false);
    unsigned char* p = mat->p;
    for(size_t i = 0; i < mat->length; i++, p++)
    {
        *p = *p > t ? 0 : RealBinaryValue;
    }
    return true;
}
/* pass : (t, 255] */
bool ThresholdHighPassToBinary(Mat<unsigned char>* mat, int t)
{
    VALRET_ASSERT(mat, false);
    unsigned char* p = mat->p;
    for(size_t i = 0; i < mat->length; i++, p++)
    {
        *p = *p < t ? 0 : RealBinaryValue;
    }
    return true;
}
/* pass : [min, max] */
bool ThresholdToBinary(Mat<unsigned char>* mat, unsigned char min, unsigned char max)
{
    VALRET_ASSERT(mat, false);
    unsigned char* p = mat->p;
    for(size_t i = 0; i < mat->length; i++, p++)
    {
        *p = min > *p || *p > max ? 0 : RealBinaryValue;
    }
    return true;
}
#endif
