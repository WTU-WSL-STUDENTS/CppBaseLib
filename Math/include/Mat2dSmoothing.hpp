/*
 * @Description: ����Ԫ��ƽ��, ������Ƶ, ǿ����Ƶ��һ��ͨ�������Ȩʵ��
 * ������� :
 * �������� :
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-19 07:00:03
 * @LastEditors: like
 * @LastEditTime: 2021-11-19 16:05:51
 */
#ifndef MAT2D_SMOOTHING_HPP
#define MAT2D_SMOOTHING_HPP

#include <Mat.hpp>

/* ģ���С����Ϊ�����Σ��߳�����Ϊ����, �������������Ҳ�����ⶨ��ģ���С����Ȩ�����ֲ�, ģ��Ԫ�ظ��� >= 2 */
template<typename T>
Mat<T>* CreateMatTemplateKernel(int w, int h, T* vecData)
{
    VALRET_ASSERT((0 == w % 2) && (0 == h % 2) && (3 < (w + h)) && vecData, NULL);/* ģ����������Χ 1 �������ϵ��ھ����� */
    return new Mat<T>(w, h, vecData);
}
/* ������ģ�Ծ�����м�Ȩ����, ������ ֱ���Ҳ��޷��ƶ�����л���, ���ϵ��½�����ģ�ƶ� , ֱ����ģ�²ഥ�׻�������� */
template<typename T>
bool Mean(Mat<T>* mat, Mat<T>* templateKernel)
{
    VALRET_ASSERT(mat && templateKernel, false);
    T* p = mat->p;
    
    int tkHalfHeight = templateKernel->h >> 1;
    int tkHalfWidth  = templateKernel->w >> 1;
    printf("%d, %d\n", tkHalfHeight, tkHalfWidth);
    T sum;
    for(int tkRow = tkHalfHeight, matRowSpan = 0; tkRow < mat->h - tkHalfHeight; tkRow++)
    {
        matRowSpan = (tkRow - 1) * mat->w; 
        for(int tkCol = tkHalfWidth; tkCol < mat->w - tkHalfWidth; tkCol++)
        {
            /* ��ģ�������˲���� */
            sum = 0;
            for(int r = 0; r < templateKernel->h; r++)
            {
                for(int c = 0; c < templateKernel->w; c++)
                {
                    sum += mat->p[(tkRow - tkHalfHeight + r) * mat->w + (tkCol - tkHalfWidth) + c] * templateKernel->p[r * templateKernel->w + c];
                    printf("(%d, %d):[%d, %d]\n", (tkRow - tkHalfHeight + r) * mat->w + (tkCol - tkHalfWidth) + c, r * templateKernel->w + c,
                    mat->p[(tkRow - tkHalfHeight + r) * mat->w + (tkCol - tkHalfWidth) + c] , templateKernel->p[r * templateKernel->w + c]);
                }
            }
            printf(" => %d, %d, %d\n", tkRow, tkCol, sum);
            /* ���ֵ */
            p[matRowSpan + tkCol] = sum / templateKernel->length; 
        }
    }
    return true;
}



#endif
