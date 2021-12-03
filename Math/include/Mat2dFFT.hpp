/*
 * @Description: 二维矩阵的快速傅里叶变换 & 逆变换 及频域滤波
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-12-03 07:48:47
 * @LastEditors: like
 * @LastEditTime: 2021-12-03 09:01:27
 */
#ifndef MAT2D_FFT_HPP
#define MAT2D_FFT_HPP

#include <Mat.hpp>
#include <complex.h>

template<typename T, typename TOut>
Mat<TOut>* FFT(Mat<T>* mat)
{
    VALRET_ASSERT(mat, NULL);
    size_t  N = mat->length;
    int     L = mat->h;
    int     M = mat->w;
    Mat<T>* x2 = new Mat<T>(M, L);
    memset(x2->p, 0, sizeof(T) * x2->length);
    for(int r = 0; r < L; r++)
    {
        for(int c = 0; c < M; c++)
        {
            // x2->p[r * L + c] = mat->p[r * M + c]
        }
    }
}

#endif
