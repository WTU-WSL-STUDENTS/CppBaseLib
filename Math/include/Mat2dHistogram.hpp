/*
 * @Description: ֱ��ͼͳ�� / ֱ��ͼ���⻯
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-09 07:44:29
 * @LastEditors: like
 * @LastEditTime: 2021-11-09 08:59:47
 */
#ifndef MAT2D_HISTOGRAM_HPP
#define MAT2D_HISTOGRAM_HPP
#include <Mat.hpp>

#ifndef HISTOGRAM_LENGTH
#define HISTOGRAM_LENGTH (1 << (8 * sizeof(char)))
#endif

/*��һ��*/
bool NormalizedHistogram(Mat<unsigned char>* mat, double (&hist)[HISTOGRAM_LENGTH]/* container */)
{
    VALRET_ASSERT(mat, false);
    memset(hist, 0, sizeof(hist[0]) * HISTOGRAM_LENGTH);
    unsigned char *p = mat->p;
    for(size_t i = 0; i < mat->length; i++,p++)
    {
        hist[*p]++;
    }
    int N = mat->h * mat->w;
    for(int i = 0; i < HISTOGRAM_LENGTH; i++)
    {
        hist[i] /= N;
    }
    return true;
}
/*ֱ��ͼ���⻯*/
bool EqualizeHistogram(Mat<unsigned char> *mat, double histEqualized[HISTOGRAM_LENGTH]) /*https://blog.csdn.net/qq_15971883/article/details/88699218*/
{
    double normalized[HISTOGRAM_LENGTH];
    VALRET_ASSERT(NormalizedHistogram(mat, normalized), false); /* ����ֲ�Ƶ�� */
    double cdf[HISTOGRAM_LENGTH] = {normalized[0]};             /*�ۻ��ֲ�Ƶ��*/
    histEqualized[0]                  = {cdf[0] * (HISTOGRAM_LENGTH - 1)};/*index->piexlMap[index] : ������ɫ��ӳ��*/
    for(int i = 1; i < HISTOGRAM_LENGTH; i++)
    {
        cdf[i]          = cdf[i - 1] + normalized[i];
        histEqualized[i]= cdf[i] * (HISTOGRAM_LENGTH - 1);
    }
    return true;
}
/*ֱ��ͼ���⻯*/
void SelfEqualizeHistogram(Mat<unsigned char> *mat)
{
    double hist[HISTOGRAM_LENGTH];
    VOIDRET_ASSERT(EqualizeHistogram(mat, hist));
    unsigned char* p = mat->p;
#ifdef MAT2D_HISTOGRAM_DEBUG
    for(int i = min; i < HISTOGRAM_LENGTH; i++)
    {
        printf("(%d, %lf) \n", i, hist[i]);
    }
#endif
    for(size_t i = 0; i < mat->length; i++, p++)
    {
        *p = (unsigned char)hist[*p];
    }
}

#endif