/*
 * @Description: 二维矩阵阈值分割
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-09 08:22:32
 * @LastEditors: like
 * @LastEditTime: 2021-11-09 16:50:43
 */
#ifndef MAT2D_THRESHOLD_HPP
#define MAT2D_THRESHOLD_HPP
#include <Mat.hpp>
#include <Mat2dHistogram.hpp>

struct RLC
{
    public:
    int label;
    int index;
    int length;
    RLC(int i, int l):label(0),index(i),length(l){}
};

/** refrence : https://blog.csdn.net/liyuanbhu/article/details/49387483
 * 缺陷 : 当目标与背景的大小比例悬殊时，类间方差准则函数可能呈现双峰或多峰，此时效果不好
*/
bool AutoThreshold(Mat<unsigned char>* mat, int& thresholdVal)
{
    double hist[HISTOGRAM_LENGTH];
    NormalizedHistogram(mat, hist);
    PrintHistogram(hist);
    double cdf[HISTOGRAM_LENGTH] = {hist[0]};   /* 累积分布函数 */
    double gpa[HISTOGRAM_LENGTH] = {0};         /* 0~i 加权平均加权平均 */
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

bool ThresholdLowPass(Mat<unsigned char>* mat, int t)
{
    VALRET_ASSERT(mat, false);

    unsigned char* p = mat->p;
    for(int i = 0; i < mat->length; i++, p++)
    {
        if(*p > t)
        {
            *p = 0;
        }
    }
    return true;
}

#endif
