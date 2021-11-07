/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-09 19:47:07
 * @LastEditors: like
 * @LastEditTime: 2021-11-08 05:03:50
 */
#ifndef MAT_CACULATE_HPP
#define MAT_CACULATE_HPP

#include <Mat.hpp>
#include <math.h>

template<typename T>
bool Add(Mat<T>* left, const Mat<T>* right)
{
    VALRET_ASSERT(left->w == right->w && left->h == right->h, false);
    for(int i = 0; i < left->length; i++)
    {
        left->p[i] += right->p[i];
    }    
    return true;
}
template<typename T>
bool Add(Mat<T>* left, const T& right)
{
    VALRET_ASSERT(left->w == right->w && left->h == right->h, false);
    for(int i = 0; i < left->length; i++)
    {
        left->p[i] += right;
    }    
    return true;
}
template<typename T>
bool Sub(Mat<T>* left, const Mat<T>* right)
{
    VALRET_ASSERT(left->w == right->w && left->h == right->h, false);
    for(int i = 0; i < left->length; i++)
    {
        left->p[i] -= right->p[i];
    }    
    return true;
}
template<typename T>
bool Sub(Mat<T>* (&left), const T& right)
{
    for(int i = 0; i < left->length; i++)
    {
        left->p[i] -= right;
    }    
    return true;
}
template<typename T>
Mat<T>* Mult(Mat<T>* left, Mat<T>* right)/* https://baike.baidu.com/item/%E7%9F%A9%E9%98%B5%E4%B9%98%E6%B3%95/5446029 */
{
    VALRET_ASSERT(left->w == right->h, NULL);
    Mat<T>* res = new Mat<T>(right->w, left->h);
    T* p = res->p;
    T* selectRow    = NULL;
    T* selectColumn = NULL;
    for(int r = 0; r < res->h; r++)
    {
        for(int c = 0; c < res->w; c++)
        {
#ifdef MAT_CACULATE_DEBUG
            printf("%d, %d\n", r, c);
#endif
            selectRow    = right->p + c;
            selectColumn = left->RowAt(r);
            for(int i = 0; i < left->w; i++, selectRow += right->w, selectColumn++)
            {
                *p += (*selectColumn * *selectRow);
#ifdef MAT_CACULATE_DEBUG
                printf("    %d\n", (*selectColumn * *selectRow));
#endif
            }
            p++;
        }
    }    
    return res;
}
template<typename T>
inline Mat<T>* GenMat2dIdentity()
{
    T src[] = 
    {
        1, 0, 0, 
        0, 1, 0,
        0, 0, 1
    };
    return new Mat<T>(src, 3, 3);
}
/**
 * ƽ�Ƶı任��ʽ
 * X = x + dx
 * Y = y + dy
 * =>���ھ���A��ʹ���ұ任����
 * [x, y, 1] * A = [X, Y, 1]
 * =>
 * [x, y, 1] * A = [x + dx, y + dy, 1]
 * =>ͨ�����飬�������Aʹ�����ҳ���
 * [x, y, 1] * A = [1 * x + 1 * dx, 1 * y + 1 * dy, 1]
 * =>
 *              [1, 0, 0]
 * [x, y, 1] *  [0, 1, 0] = [1 * x + 1 * dx, 1 * y + 1 * dy, 1]
 *              [dx, dy, 1]
 */
template<typename T>
inline bool Mat2dTranslate(Mat<T>* mat/* 3 x 3 */, T dx, T dy)/* ����ָ��x,yƫ�����ı任����*/
{
    VALRET_ASSERT(mat && 3 == mat->h && 3 == mat->w, false);
    mat->p + 6 = dx;
    mat->p + 7 = dy;
    return true;
}
/**
 * ���ŵı任��ʽ
 * X = x * sx
 * Y = y * sy
 * =>���ھ���A��ʹ���ұ任����
 * [x, y, 1] * A = [X, Y, 1]
 * =>
 * [x, y, 1] * A = [x * sx, y * sy, 1]
 * =>ͨ�����飬�������Aʹ�����ҳ���
 *              [sx, 0, 0]
 * [x, y, 1] *  [0, sy, 0] = [x * sx, y * sy, 1]
 *              [0, 0, 1]
 */
template<typename T>
inline bool Mat2dScala(Mat<T>* mat/* 3 x 3 */, T sx/* x������������ */, T sy)
{
    VALRET_ASSERT(mat && 3 == mat->h && 3 == mat->w, false);
    mat->p = sx;
    mat->p + 4 = sy;
    return true;
}
/**
 * �ڰ뾶Ϊr��԰��, ��ת�ı任��ʽ
 * x = r * cos(��)
 * y = r * sin(��)
 * ��ת�ȽǶȺ�
 * X = r * cos(�� + ��)
 * Y = r * sin(�� + ��)
 * =>
 * X = r * (cos(��) * cos(��) - sin(��) * sin(��))
 * Y = r * (sin(��) * cos(��) + cos(��) * sin(��))
 * =>
 * X = x * cos(��) - y * sin(��)
 * Y = y * cos(��) + x * sin(��)
 * =>���ھ���A��ʹ���ұ任����
 * [x, y, 1] * A = [X, Y, 1]
 * =>
 * [x, y, 1] * A = [x * cos(��) - y * sin(��), y * cos(��) + x * sin(��), 1]
 * =>ͨ�����飬�������Aʹ�����ҳ���
 *              [cos(��) , sin(��), 0]
 * [x, y, 1] *  [-sin(��), cos(��), 0] = [x * cos(��) - y * sin(��), y * cos(��) + x * sin(��), 1]
 *              [0      , 0     , 1]
 */
template<typename T>
inline bool Mat2dRotate(Mat<T>* mat/* 3 x 3 */, double radians)/* ����ָ��x,yƫ�����ı任����*/
{
    VALRET_ASSERT(mat && 3 == mat->h && 3 == mat->w, false);
    mat->p     = (T)cos(radians);
    mat->p + 1 = (T)sin(radians);
    mat->p + 3 = -1 * (T)sin(radians);
    mat->p + 4 = (T)cos(radians);
    return true;
}

template<typename T>
inline Mat<T>* GenMat3dIdentity()
{
    T src[] = 
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    return new Mat<T>(src, 4, 4);
}
#endif