/*
 * @Description: 矩阵元素平滑, 削弱高频, 强化低频，一般通过邻域加权实现
 * 随机噪声 :
 * 椒盐噪声 :
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-19 07:00:03
 * @LastEditors: like
 * @LastEditTime: 2021-11-19 16:05:51
 */
#ifndef MAT2D_SMOOTHING_HPP
#define MAT2D_SMOOTHING_HPP

#include <Mat.hpp>

/* 模板大小建议为正方形，边长必须为基数, 如果有特殊需求也可任意定义模板大小及加权参数分布, 模板元素个数 >= 2 */
template<typename T>
Mat<T>* CreateMatTemplateKernel(int w, int h, T* vecData)
{
    VALRET_ASSERT((0 == w % 2) && (0 == h % 2) && (3 < (w + h)) && vecData, NULL);/* 模板必须包含周围 1 个及以上的邻居像素 */
    return new Mat<T>(w, h, vecData);
}
/* 利用掩模对矩阵进行加权计算, 从左到右 直到右侧无法移动会进行换行, 从上到下进行掩模移动 , 直到掩模下侧触底会结束计算 */
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
            /* 掩模与矩阵相乘并求和 */
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
            /* 求均值 */
            p[matRowSpan + tkCol] = sum / templateKernel->length; 
        }
    }
    return true;
}



#endif
