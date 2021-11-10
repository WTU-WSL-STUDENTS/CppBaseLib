/*
 * @Description: ��ά������ֵ�ָ�
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
 * ȱ�� : ��Ŀ���뱳���Ĵ�С��������ʱ����䷽��׼�������ܳ���˫����壬��ʱЧ������
*/
bool AutoThreshold(Mat<unsigned char>* mat, int& thresholdVal)
{
    double hist[HISTOGRAM_LENGTH];
    NormalizedHistogram(mat, hist);
    PrintHistogram(hist);
    double cdf[HISTOGRAM_LENGTH] = {hist[0]};   /* �ۻ��ֲ����� */
    double gpa[HISTOGRAM_LENGTH] = {0};         /* 0~i ��Ȩƽ����Ȩƽ�� */
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
