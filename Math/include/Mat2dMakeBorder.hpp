/*
 * @Description: ��ά����߽���չ
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-25 09:04:51
 * @LastEditors: like
 * @LastEditTime: 2021-11-26 15:46:13
 */
#ifndef MAT2D_MAKE_BORDER_HPP
#define MAT2D_MAKE_BORDER_HPP

#ifndef MAT2D_MAKE_BORDER_DEBUG
#define MAT2D_MAKE_BORDER_DEBUG
#undef MAT2D_MAKE_BORDER_DEBUG
#endif
#include <Mat.hpp>

typedef enum
{
    BORDER_CONSTANT     ,
    BORDER_REFLECT_101 
}BorderTypes;

/**
 * @brief �߽羵������, cv::copyMakeBorder �� BORDER_REFLECT_101 ʵ��, Mat2dGaussFilter ���ٸ�˹�˲��ӿ������ù���
 *  ---------------
 *  Origin Mat    
 *  0 1 2
 *  1 2 3
 *  2 3 4
 *  ---------------      
 *  Border Reflected Mat
 *  4 3* 2 3 4 *3 2        
 *  3 2* 1 2 3 *2 1    
 *  ---*-------*---    
 *  2 1* 0 1 2 *1 0        
 *  3 2* 1 2 3 *2 1        
 *  4 3* 2 3 4 *3 2
 *  ---*-------*---       
 *  3 2* 1 2 3 *2 1        
 *  2 1* 0 1 2 *1 0 
 * 
 * @tparam T 
 * @param mat ԭʼ����
 * @param top �Ϸ�������չ�߶�
 * @param left ��ྵ����չ���
 * @param bottom �·�������չ�߶�
 * @param right �Ҳྵ����չ���
 * @return Mat<T>* �����ľ���, �������ԳƳ��ȳ���ԭʼ����ĳ���, �᷵�ؿ�ָ��
 */
template<typename T>
Mat<T>* CopyMakeBorderReflect_101(Mat<T>* mat, int top, int left, int bottom, int right)
{
    VALRET_ASSERT(top < mat->h && bottom < mat->h && left < mat->w && right < mat->w, NULL);

    Mat<T>* dest = new Mat<T>(mat->w + left + right, mat->h + top + bottom);
    T* p = dest->p;
    for(int r = 0, destRow = top, matspan = 0, destspan = destRow * dest->w; r < mat->h; r++, destRow++, matspan += mat->w, destspan += dest->w)
    {
        /* ����ԭʼ���� */
        memcpy(p + destspan + left, mat->p + matspan, mat->w * sizeof(T));  
        /* ��ྵ��Գ� */
        for(int i = 0; i < left; i++)                           
        {
            p[destspan + i] = mat->p[matspan + left - i];
        }
        /* �Ҳྵ��Գ� */
        for(int i = 0; i < right; i++)                          
        {
            p[destspan + dest->w - 1 - i] = mat->p[matspan + mat->w - 1 - right + i];
        }
    }
    /* �ϲྵ��Գ� */
    for(int i = 0, matspan = top * mat->w, destspan = left; i < top; i++, matspan -= mat->w, destspan += dest->w)  
    {
        memcpy(p + destspan, mat->p + matspan, mat->w * sizeof(T));
    }
    /* �²ྵ��Գ� */
    for(int i = 0, matspan = (mat->h - 1 - bottom) * mat->w, destspan = (dest->h - 1) * dest->w + left; i < bottom; i++, matspan += mat->w, destspan -= dest->w)  
    {
        memcpy(p + destspan, mat->p + matspan, mat->w * sizeof(T));
    }
    /* top & left �� */
    for(int r = 0, matR = mat->h - 1, destR = 0; r < top; r++, matR--, destR++)
    {
        for(int c = 0, matC = mat->w - 1, destC = 0; c < left; c++, matC--, destC++)
        {
            p[dest->w * destR + destC] = mat->p[mat->w * matR + matC];
        }
    }
    /* top & right �� */
    for(int r = 0, matR = mat->h - 1, destR = 0; r < top; r++, matR--, destR++)
    {
        for(int c = 0, matC = 0, destC = dest->w - 1; c < right; c++, matC++, destC--)
        {
            p[dest->w * destR + destC] = mat->p[mat->w * matR + matC];
        }
    }
    /* bottom & left �� */
    for(int r = 0, matR = 0, destR = dest->h - 1; r < bottom; r++, matR++, destR--)
    {
        for(int c = 0, matC = mat->w - 1, destC = 0; c < left; c++, matC--, destC++)
        {
            p[dest->w * destR + destC] = mat->p[mat->w * matR + matC];
        }
    }
    /* bottom & right �� */
    for(int r = 0, matR = 0, destR = dest->h - 1; r < top; r++, matR++, destR--)
    {
        for(int c = 0, matC = 0, destC = dest->w - 1; c < right; c++, matC++, destC--)
        {
            p[dest->w * destR + destC] = mat->p[mat->w * matR + matC];
        }
    }
    return dest;
}
/**
 * @brief ��ԭʼ����ı߽����䲢���̶�ֵ
 * CopyMakeBorderConstant(mat, 1, 1, 1, 1, 1)
 * ---------------
 * Origin Mat    
 * 0 1 2
 * 1 2 3
 * 2 3 4
 * ---------------
 * Border Filled Constant Value Mat
 * 1 1 1 1 1
 * 1 0 1 2 1
 * 1 1 2 3 1
 * 1 2 3 4 1
 * 1 1 1 1 1
 * 
 * 
 * @tparam T 
 * @param mat ԭʼ����
 * @param top 
 * @param left 
 * @param bottom 
 * @param right 
 * @param val ����ֵ
 * @return Mat<T>* �����ľ���
 */
template<typename T>
Mat<T>* CopyMakeBorderConstant(const Mat<T>* mat, int top, int left, int bottom, int right, const T& val)
{
    VALRET_ASSERT(0 < top && 0 < bottom && 0 < left && 0 < right, NULL);

    Mat<T>* dest = new Mat<T>(mat->w + left + right, mat->h + top + bottom);
    T* p = dest->p;
    size_t pTopOrBottomLen  = dest->w * sizeof(T);
    size_t pLeftLen         = left * sizeof(T);
    size_t pRightLen        = right * sizeof(T);
    T* pTopOrBottom  = (T*)malloc(pTopOrBottomLen);
    T* pLeft         = (T*)malloc(pLeftLen);
    T* pRight        = (T*)malloc(pRightLen);
    for(int i = 0; i < dest->w; i++) /* top & bottom */
    {
        pTopOrBottom[i] = val;
    }
    for(int i = 0; i < left; i++) /* left */
    {
        pLeft[i] = val;
    }
    for(int i = 0; i < right; i++) /* right */
    {
        pRight[i] = val;
    }
    bottom = dest->h - 1 - bottom;
    for(int r = 0, rightBegin = dest->w - right, destSpan = 0, matSpan = 0, stride = mat->w * sizeof(T);r < dest->h; r++, destSpan += dest->w, rightBegin += dest->w)
    {
        if(r < top || r > bottom)
        {
            memcpy(p + destSpan, pTopOrBottom, pTopOrBottomLen);
        }
        else
        {
            memcpy(p + destSpan,        pLeft,              pLeftLen);
            memcpy(p + destSpan + left, mat->p + matSpan,   stride);
            memcpy(p + rightBegin,      pRight,             pRightLen);
            matSpan += mat->w;
        }
    }
    if(pTopOrBottom)
    {
        free(pTopOrBottom);
    }
    if(pLeft)
    {
        free(pLeft);
    }
    if(pRight)
    {
        free(pRight);
    }
    return dest;
}
#endif