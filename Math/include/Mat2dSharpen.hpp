/*
 * @Description: ͼ���� : ΢������, �ݶ���, ��Ե��� 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-29 07:02:46
 * @LastEditors: like
 * @LastEditTime: 2021-12-03 07:31:11
 */
#ifndef MAT2D_SHARPEN_HPP
#define MAT2D_SHARPEN_HPP

#ifndef MAT2D_SHARPEN_DEBUG
#define MAT2D_SHARPEN_DEBUG
#undef MAT2D_SHARPEN_DEBUG
#endif

#include <Mat2dSmoothing.hpp>
#include <map>

/**
 * # ͼ����
 * ## ��Ե����
 * - ����״
 * | black | white |
 *  �Ҷ�:
 *          /----
 *      ---/
 *  һ�׵���(���):
 * 
 *           ||
 *      ----/  \----
 *  ���׵���:
 *          /\
 *      ----  \  ----
 *             \/
 *  - ����״
 *  | black | white | black |
 *  �Ҷ�:
 *           /----\
 *      ----/      \-----
 *  һ�׵���(���):
 * 
 *           /--\
 *      ----/    \  ----
 *                \/
 *  ���׵���:
 *          /\
 *      ----  \  ----
 *             \/
 * ## ΢���㷨
 * - һ�� / ����΢������
 * dx = f(x + 1) - f(x)
 * dx^2 = f(x + 1) + f(x - 1) - 2f(x)
 *
 * - ����΢������
 *  h(r, c) = f(r, c + 1) - f(r, c)
 *  v(r, c) = f(r + 1, c) - f(r, c)
 * 
 * - ˫��һ��΢��
 *  g(r, c) = h(r, c)^2 + v(r, c)^2
 * 
 * ## ��ͨ�˲���, �ݶ��� kernel ֮һ��΢��
 * - robert kernel : ����΢��
 * ���� x, y ����� kernel :
 * [-1, 0]   [0, -1]
 * [ 0, 1] & [1,  0]
 * ->
 * rob(x) = | f(1, 1) - f(0, 0) |
 * rob(y) = | f(1, 0) - f(0, 1) |
 * ��f    = rob(x) + rob(y)
 * 
 * - sobel kernel      
 * ���� x, y ����� kernel : 
 * [-1, -2, -1]   [-1, 0, -1]
 * [0 , 0 , 0 ] & [-2, 0, -2]
 * [1 , 2 , 1 ]   [-1, 0, -1]
 * ��Ե�����Ͻ����˼�Ȩƽ��
 * ->
 * sob(x) = -f(0, 0) - 2f(0, 1) - f(0, 2) + f(2, 0) + 2f(2, 1) + f(2, 2)
 * sob(y) = -f(0, 0) - 2f(1, 0) - f(2, 0) + f(0, 2) + 2f(1, 2) + f(2, 2)
 *  ��f   = sob(x) + sob(y)
 * 
 * - prewitt kernel
 * ���� x, y ����� kernel : 
 * [-1, -1, -1]   [-1, 0, -1]
 * [0 , 0 , 0 ] & [-1, 0, -1]
 * [1 , 1 , 1 ]   [-1, 0, -1]
 * �� sobel ����ڱ�Ե������ȡ���˼�Ȩƽ��
 * 
 * ## ��ͨ�˲���, �ݶ��� kernel ֮����΢��
 * - laplcian kernel
 * ǿ����Ƶ, ������Ƶ, ����ԭͼ��Ƶ��Ϣ��ʧ��
 * 
 * x, y �����ݶ�
 * ->
 * dx-lap(x, y) = | f(x + 1, y) + f(x - 1, y) - 2f(x, y) |
 * dy-lap(x, y) = | f(x, y + 1) + f(x, y - 1) - 2f(x, y) |
 * -> ���������ݶ� 
 * ��^2f(x, y) =  dy-lap(x, y) + dx-lap(x, y) 
 * -> ���ݹ�ʽ�ɹ��� kernel :
 * [0,  1, 0] laplcian  [1,  1, 1]
 * [1, -4, 1] ���Ƹ�˹�� [1, -8, 1]
 * [0,  1, 0] ����չΪ   [1,  1, 1]
 * -> ���������������ֵ����, �ɽ� kernel ����ȡ�෴��
 * [ 0, -1,  0]     [-1, -1, -1]
 * [-1,  4, -1]     [-1,  8, -1]
 * [ 0, -1,  0]     [-1, -1, -1]
 * 
 * - ���ԭͼ��Ƶ��ʧ�ķ���
 * ԭͼ�� laplcian ͼ����е���, ǿ����Ƶ��Ϣ, ������Ƶ��Ϣ��
 *        |- �� * f(x, y) + ��^2f(x, y) : ����ϵ��Ϊ��
 * g(x) = |
 *        |- �� * f(x, y) - ��^2f(x, y) : ����ϵ��Ϊ��
 * 
 * ͨ����, �� �� = 1 ->
 * g(x) = f(x, y) - | f(x + 1, y) + f(x - 1, y) - 2f(x, y) + f(x, y + 1) + f(x, y - 1) - 2f(x, y)) |
 *      = 5f(x, y) - f(x + 1, y) - f(x - 1, y) - f(x, y + 1) - f(x, y - 1)
 * -> ���������ԭͼ��Ϣ�� kernel
 * [ 0, -1,  0]     [-1, -1, -1]
 * [-1,  5, -1] ����[-1,  9, -1]  
 * [ 0, -1,  0]     [-1, -1, -1]
 * 
 * - gauss & laplcian kernel 
 * Ч�����õı�Ե�����, �Ѹ�˹ƽ����������˹�񻯽������, ��ȥ�����б�Ե��⡣
 * ->
 * [-2, -4, -4, -4, -2]
 * [-4,  0,  8,  0, -4]
 * [-4,  8, 24,  8, -4]
 * [-4,  0,  8,  0, -4]
 * [-2, -4, -4, -4, -2]
 * 
 * - kirsch kernel
 * �涨��һ�� kernel ��������Ӧ���
 * [-1 0 1] [-1 -1 -1]  [ 0  1  1]      [0 -1 -1]
 * [-1 0 1] [ 0  0  0]  [-1  0  1]      [1  0 -1]
 * [-1 0 1] [ 1  1  1]  [-1 -1  0]      [1  1  0]
 * ˮƽ����   ��ֱ����  45�ȷ�����ұ߽�   135�ȷ�����ұ߽�
 * 
 * ��� kernel ���ξ��, �ĸ� kernel ���������, ��Ϊ�ڸ÷������Ǳ�Ե
 * 
 * 
 */


/**
 * @brief ����һ�� 2 x 2 �� robert kernel , ���ڳ���Ϊż�� , �������Ľ�����λ����ʵ��ֵƫ���� 0.5 ����λ
 * refrence : 
 *  http://blog.sina.com.cn/s/blog_6833a4df0101ielv.html
 * 
 * @tparam T 
 * @param horizonKernel 
 * @param verticalKernel 
 * @return true 
 * @return false 
 */
template<typename T>
inline bool CreateMat2dRobertKernel(Mat<T>* horizonKernel, Mat<T>* verticalKernel)
{
    const T horizon[] = 
    {
        -1, 0,
         0, 1
    };
    const T vertical[] = 
    {
        0, -1,
        1,  0
    };
    horizonKernel = new Mat<T>*(2, 2, horizon);
    verticalKernel = new Mat<T>*(2, 2, vertical);
    return true;
}
/**
 * @brief ����һ�� 3 x 3 �� sobel kernel , �ڱ�Ե�����ϻ���и�˹ƽ��
 * 
 * @tparam T 
 * @param horizonKernel sobel ˮƽ kernel
 * @param verticalKernel sobel ��ֱ kernel
 * @return true 
 */
template<typename T>
inline bool CreateMat2dSobelKernel(Mat<T>* horizonKernel, Mat<T>* verticalKernel)
{
    const T sobelHorizon[] = 
    {
        -1, -2, -1, 
         0,  0,  0, 
         1,  2,  1
    };
    const T sobelVertical[] = 
    {
        -1, 0, 1, 
        -2, 0, 2, 
        -1, 0, 1
    };
    horizonKernel = new Mat<T>*(3, 3, sobelHorizon);
    verticalKernel = new Mat<T>*(3, 3, sobelVertical);
    return true;
}
/**
 * @brief ����һ�� 3 x 3 �� prewitt kernel , sobel ������, û�б�Ե�����ϵ�ƽ������
 * 
 * @tparam T 
 * @param horizonKernel sobel ˮƽ kernel
 * @param verticalKernel sobel ��ֱ kernel
 * @return true 
 */
template<typename T>
inline bool CreateMat2dPrewittKernel(Mat<T>* horizonKernel, Mat<T>* verticalKernel)
{
    const T prewittHorizon[] = 
    {
        -1, -1, -1, 
         0,  0,  0, 
         1,  1,  1
    };
    const T prewittVertical[] = 
    {
        -1, 0, 1, 
        -1, 0, 1, 
        -1, 0, 1
    };
    horizonKernel = new Mat<T>*(3, 3, prewittHorizon);
    verticalKernel = new Mat<T>*(3, 3, prewittVertical);
    return true;
}

typedef enum
{
    x,          /* ˮƽ����һ��΢��           */
    y,          /* ��ֱ����һ��΢��           */
    max_xy,     /* ˮƽ�������ֱ����ȡ���ֵ  */
    sum_sqrt,   /* ˫��һ��΢��֮ȡ����        */
    sum_abs,    /* ˫��һ��΢��֮ȡ����ֵ      */
    robert,     /* robert ����΢��            */
    prewitt,    /* prewitt ����*/
    sobel,      /* sobel ����, prewitt ���ӻ�����ƽ���˱�Ե */
    krisch,     /* 8 ��������ȡ���Ž���Ϊ��Ե */
    laplcian4,  /* ������˹����, ����΢��, ����ֵΪ 4 */
    laplcian4WithOriginMat,  /* laplcian4 �����ϱ���ԭʼ������Ϣ */
    laplcian8,  /* ����ֵΪ 8 */
    laplcian8WithOriginMat,  /* laplcian8 �����ϱ���ԭʼ������Ϣ */
    LOG,        /* ��˹ƽ��ȥ���, ����������˹ */
    FilterTypeCount
}FilterType;
/**
 * @brief һ��΢���㷨ʵ��
 * 
 * @tparam T 
 * @tparam TOut 
 */
template<typename T, typename TOut>
class Mat2dDifferentiate1
{
private:
    Mat2dDifferentiate1() : laplcianOriginMatAerfa(1)
    {
    }
public:  
    /* laplcian4WithOriginMat , laplcian8WithOriginMat ������ԭʼ�����ǿ��ϵ��, Ĭ��ֵΪ 1 */
    double laplcianOriginMatAerfa;
    
    typedef TOut (*FilterCaculateType)(Mat<T>* mat, int row, int col);
    typedef std::map<FilterType, FilterCaculateType> MapFilterCaculateCollections;
    MapFilterCaculateCollections MapFilterCaculate = 
    {
        {sum_sqrt   , SumSqrt   },
        {sum_abs    , SumAbs    },
        {x          , X         },
        {y          , Y         },
        {max_xy     , XYMax     },
        {robert     , Roberts   },
        {sobel      , Sobel     },
        {prewitt    , Prewitt   },
        {krisch     , Krisch    },
        {laplcian4  , Laplcian4 },
        {laplcian8  , Laplcian8 },
        {laplcian4WithOriginMat, Laplcian4WithOriginMat },
        {laplcian8WithOriginMat, Laplcian8WithOriginMat },
        {LOG        , LaplicanOfGauss                   }
    };
    static Mat2dDifferentiate1 SingleObj()
    {
        static Mat2dDifferentiate1 instance;
        return instance;
    }

    static inline TOut SumSqrt(Mat<T>* mat, int row, int col)
    {
        int span = row * mat->w + col;
        T dx = mat->p[span + 1] - mat->p[span];
        T dy = mat->p[span + mat->w] - mat->p[span]; 
        return (TOut)(sqrt(dx * dx + dy * dy) / 4);
    }
    static inline TOut SumAbs(Mat<T>* mat, int row, int col)
    {
        int span = row * mat->w + col;
        T dx = mat->p[span + 1] - mat->p[span];
        T dy = mat->p[span + mat->w] - mat->p[span]; 
        return (TOut)(abs(dx) + abs(dy)) / 4;
    }
    static inline TOut X(Mat<T>* mat, int row, int col)
    {
        int span = row * mat->w + col;
        T dx = mat->p[span + 1] - mat->p[span];
        return (TOut)abs(dx);
    }
    static inline TOut Y(Mat<T>* mat, int row, int col)
    {
        int span = row * mat->w + col;
        T dy = mat->p[span + mat->w] - mat->p[span]; 
        return (TOut)abs(dy);
    }
    static inline TOut XYMax(Mat<T>* mat, int row, int col)  
    {
        int span = row * mat->w + col;
        T dx = mat->p[span + 1] - mat->p[span];
        T dy = mat->p[span + mat->w] - mat->p[span]; 
        return (TOut)max(abs(dx), abs(dy));
    }
    static inline TOut Roberts(Mat<T>* mat, int row, int col)  
    {
        int span = row * mat->w + col;
        T dx = mat->p[span + 1 + mat->w] - mat->p[span];
        T dy = mat->p[span + mat->w] - mat->p[span + 1]; 
        return (TOut)(abs(dx) + abs(dy));
    }
    static inline TOut Sobel(Mat<T>* mat, int row, int col)  
    {
        int span = row * mat->w + col;
        int topSpan = span - mat->w;
        int bottomSpan = span + mat->w;
        T dx = 
            (mat->p[bottomSpan - 1] + 2 * mat->p[bottomSpan]+ mat->p[bottomSpan + 1]) - 
            (mat->p[topSpan - 1]    + 2 * mat->p[topSpan]   + mat->p[topSpan + 1]);
        T dy = 
            (mat->p[topSpan + 1]    + 2 * mat->p[span + 1]  + mat->p[bottomSpan + 1]) - 
            (mat->p[topSpan - 1]    + 2 * mat->p[span - 1]  + mat->p[bottomSpan - 1]);
        return (TOut)(abs(dx) + abs(dy));
    }
    static inline TOut Prewitt(Mat<T>* mat, int row, int col)  
    {
        int span = row * mat->w + col;
        int topSpan = span - mat->w;
        int bottomSpan = span + mat->w;
        T dx = 
            (mat->p[bottomSpan - 1] + mat->p[bottomSpan]+ mat->p[bottomSpan + 1]) - 
            (mat->p[topSpan - 1]    + mat->p[topSpan]   + mat->p[topSpan + 1]);
        T dy = 
            (mat->p[topSpan + 1]    + mat->p[span + 1]  + mat->p[bottomSpan + 1]) - 
            (mat->p[topSpan - 1]    + mat->p[span - 1]  + mat->p[bottomSpan - 1]);
        return (TOut)(abs(dx) + abs(dy));
    }
    static inline TOut Krisch(Mat<T>* mat, int row, int col)
    {
        int span = row * mat->w + col;
        int topSpan = span - mat->w;
        int bottomSpan = span + mat->w;
        T dMax;
        /**
         *  5,  5,  5
         * -3,  0, -3
         * -3, -3, -3
         */
        T d0 = 
            5 * (mat->p[topSpan - 1] + mat->p[topSpan]+ mat->p[topSpan + 1]) - 
            3 * (mat->p[span - 1] + mat->p[span + 1] + mat->p[bottomSpan - 1] + mat->p[bottomSpan] + mat->p[bottomSpan + 1]);
        dMax = d0;
        /**
         * -3,  5,  5
         * -3,  0,  5
         * -3, -3, -3
         */
        T d1 = 
            5 * (mat->p[topSpan]+ mat->p[topSpan + 1] + mat->p[span + 1]) - 
            3 * (mat->p[topSpan - 1] + mat->p[span - 1] + mat->p[bottomSpan - 1] + mat->p[bottomSpan] + mat->p[bottomSpan + 1]);
        if(dMax < d1)
            dMax = d1;
        /**
         * -3, -3,  5
         * -3,  0,  5
         * -3, -3,  5
         */
        T d2 = 
            5 * (mat->p[topSpan + 1] + mat->p[span + 1] + mat->p[bottomSpan + 1]) - 
            3 * (mat->p[topSpan - 1] + mat->p[topSpan] + mat->p[span - 1] + mat->p[bottomSpan - 1] + mat->p[bottomSpan]);
        if(dMax < d2)
            dMax = d2;
        /**
         * -3, -3, -3
         * -3,  0,  5
         * -3,  5,  5
         */
        T d3 = 
            5 * (mat->p[span + 1] + mat->p[bottomSpan] + mat->p[bottomSpan + 1]) - 
            3 * (mat->p[topSpan - 1] + mat->p[topSpan] + mat->p[topSpan + 1] + mat->p[span - 1] + mat->p[bottomSpan - 1]);
        if(dMax < d3)
            dMax = d3;
        /**
         * -3, -3, -3
         * -3,  0, -3
         *  5,  5,  5
         */
        T d4 = 
            5 * (mat->p[bottomSpan - 1] + mat->p[bottomSpan] + mat->p[bottomSpan + 1]) - 
            3 * (mat->p[span + 1] + mat->p[topSpan - 1] + mat->p[topSpan] + mat->p[topSpan + 1] + mat->p[span - 1]);
        if(dMax < d4)
            dMax = d4;
        /**
         * -3, -3, -3
         *  5,  0, -3
         *  5,  5, -3
         */
        T d5 = 
            5 * (mat->p[bottomSpan - 1] + mat->p[bottomSpan] + mat->p[span - 1]) - 
            3 * (mat->p[bottomSpan + 1] + mat->p[span + 1] + mat->p[topSpan - 1] + mat->p[topSpan] + mat->p[topSpan + 1]);
        if(dMax < d5)
            dMax = d5;
        /**
         *  5, -3, -3
         *  5,  0, -3
         *  5, -3, -3
         */
        T d6 = 
            5 * (mat->p[bottomSpan - 1] + mat->p[span - 1] + mat->p[topSpan - 1]) - 
            3 * (mat->p[bottomSpan] + mat->p[bottomSpan + 1] + mat->p[span + 1] + mat->p[topSpan] + mat->p[topSpan + 1]);
        if(dMax < d6)
            dMax = d6;
        /**
         *  5,  5, -3
         *  5,  0, -3
         * -3, -3, -3
         */
        T d7 = 
            5 * (mat->p[span - 1] + mat->p[topSpan - 1] + mat->p[topSpan]) - 
            3 * (mat->p[bottomSpan - 1] + mat->p[bottomSpan] + mat->p[bottomSpan + 1] + mat->p[span + 1] + mat->p[topSpan + 1]);
        if(dMax < d7)
            dMax = d7;

        return (TOut)(abs(dMax));

    }
    static inline TOut Laplcian4(Mat<T>* mat, int row, int col)  
    {
        int span = row * mat->w + col;
        int topSpan = span - mat->w;
        int bottomSpan = span + mat->w;
        T ddf = 4 * mat->p[span] -
            (mat->p[topSpan] + mat->p[bottomSpan] + mat->p[span - 1] + mat->p[span + 1]);
        return (TOut)abs(ddf);
    }
    static inline TOut Laplcian4WithOriginMat(Mat<T>* mat, int row, int col)  
    {
        int span = row * mat->w + col;
        int topSpan = span - mat->w;
        int bottomSpan = span + mat->w;
        double ddf = (SingleObj().laplcianOriginMatAerfa + 4) * mat->p[span] -
            (mat->p[topSpan] + mat->p[bottomSpan] + mat->p[span - 1] + mat->p[span + 1]);
        return (TOut)abs(ddf);
    }
    static inline TOut Laplcian8(Mat<T>* mat, int row, int col)  
    {
        int span = row * mat->w + col;
        int topSpan = span - mat->w;
        int bottomSpan = span + mat->w;
        T ddf = 8 * mat->p[span] -
            (mat->p[topSpan - 1] + mat->p[topSpan] + mat->p[topSpan + 1] + mat->p[bottomSpan - 1] + mat->p[bottomSpan] + mat->p[bottomSpan + 1] + mat->p[span - 1] + mat->p[span + 1]);
        return (TOut)abs(ddf);
    }
    static inline TOut Laplcian8WithOriginMat(Mat<T>* mat, int row, int col)  
    {
        int span = row * mat->w + col;
        int topSpan = span - mat->w;
        int bottomSpan = span + mat->w;
        double ddf = (SingleObj().laplcianOriginMatAerfa + 8) * mat->p[span] -
            (mat->p[topSpan - 1] + mat->p[topSpan] + mat->p[topSpan + 1] + mat->p[bottomSpan - 1] + mat->p[bottomSpan] + mat->p[bottomSpan + 1] + mat->p[span - 1] + mat->p[span + 1]);
        return (TOut)abs(ddf);
    }
    static inline TOut LaplicanOfGauss(Mat<T>* mat, int row, int col)  
    {
        //  [-2, -4, -4, -4, -2]
        //  [-4,  0,  8,  0, -4]
        //  [-4,  8, 24,  8, -4]
        //  [-4,  0,  8,  0, -4]
        //  [-2, -4, -4, -4, -2]
        int row2 = row * mat->w + col;
        int row1 = row2 - mat->w;
        int row0 = row1 - mat->w;
        int row3 = row2 + mat->w;
        int row4 = row3 + mat->w;
        
        double ddf = 24 * mat->p[row2]
        + 8 * (mat->p[row1] + mat->p[row2 - 1] + mat->p[row2 + 1] + mat->p[row3])
        - 2 * (mat->p[row0 - 2] + mat->p[row0 + 2] + mat->p[row4 - 2] + mat->p[row4 + 2])
        - 4 * 
        (
            mat->p[row0 - 1] + mat->p[row0]     + mat->p[row0 + 1] + 
            mat->p[row1 - 2] + mat->p[row1 + 2] + 
            mat->p[row2 - 2] + mat->p[row2 + 2] +
            mat->p[row3 - 2] + mat->p[row3 + 2] +
            mat->p[row4 - 1] + mat->p[row4]     + mat->p[row4 + 1]
        );
        return (TOut)abs(ddf);
    }
};
/**
 * @brief �������һ��΢��, ��ȡ��Ե��Ϣ
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat ԭʼ����
 * @param type �������㷽��
 * @return Mat<TOut>* ����һ�׵�������Ľ��, �����ԭʼ��������� 1 , �����������к�������
 */
template<typename T, typename TOut>
Mat<TOut>* Mat2dCaculateDifferentiate1(Mat<T>* mat, FilterType type = sum_sqrt)
{
    VALRET_ASSERT(mat && 1 < mat->w && 1 < mat->h, NULL);
    Mat<TOut>* out = new Mat<TOut>(mat->w , mat->h);
    memset(out->p, 0, sizeof(TOut) * out->length);
    for(int r = 0, outSpan = 0; r < (out->h - 1); r++, outSpan++)
    {
        for(int c = 0; c < (out->w - 1); c++, outSpan++)
        {
            out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::SingleObj().MapFilterCaculate[type](mat, r, c);
        }
    }
    return out;
}
/**
 * @brief ����Robert Kernel (����΢��) �ı�Ե��ȡ
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat ԭʼ����
 * @return Mat<TOut>* ��ȡ�ı�Ե����
 */
template<typename T, typename TOut>
Mat<TOut>* Mat2dRobert(Mat<T>* mat)
{
    VALRET_ASSERT(mat && 1 < mat->w && 1 < mat->h, NULL);
    Mat<TOut>* out = new Mat<TOut>(mat->w , mat->h);
    memset(out->p, 0, sizeof(TOut) * out->length);
    for(int r = 0, outSpan = 0; r < (out->h - 1); r++, outSpan++)
    {
        for(int c = 0; c < (out->w - 1); c++, outSpan++)
        {
            out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::Roberts(mat, r, c);
        }
    }
    return out;
}
/**
 * @brief ���� 3 x 3 Sobel Kernel �ı�Ե��ȡ
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat ԭʼ����
 * @return Mat<TOut>* ��ȡ�ı�Ե����
 */
template<typename T, typename TOut>
Mat<TOut>* Mat2dSobel(Mat<T>* mat)
{
    VALRET_ASSERT(mat && 1 < mat->w && 1 < mat->h, NULL);
    Mat<TOut>* out = new Mat<TOut>(mat->w , mat->h);
    memset(out->p, 0, sizeof(TOut) * out->length);
    for(int r = 1, outSpan = out->w; r < (out->h - 1); r++)
    {
        outSpan++;
        for(int c = 1; c < (out->w - 1); c++, outSpan++)
        {
            out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::Sobel(mat, r, c);
        }
        outSpan++;
    }
    return out;
}
/**
 * @brief ���� 3 x 3 Prewitt Kernel �ı�Ե��ȡ
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat ԭʼ����
 * @return Mat<TOut>* ��ȡ�ı�Ե����
 */
template<typename T, typename TOut>
Mat<TOut>* Mat2dPrewitt(Mat<T>* mat)
{
    VALRET_ASSERT(mat && 1 < mat->w && 1 < mat->h, NULL);
    Mat<TOut>* out = new Mat<TOut>(mat->w , mat->h);
    memset(out->p, 0, sizeof(TOut) * out->length);
    for(int r = 1, outSpan = out->w; r < (out->h - 1); r++)
    {
        outSpan++;
        for(int c = 1; c < (out->w - 1); c++, outSpan++)
        {
            out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::Prewitt(mat, r, c);
        }
        outSpan++;
    }
    return out;
}
/**
 * @brief ���� 8 ����ͬ�Ƕȷ���� Kernel ȡ���Ž�ı�Ե��ȡ, �������Ƚϴ�
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat ԭʼ����
 * @return Mat<TOut>* ��ȡ�ı�Ե����
 */
template<typename T, typename TOut>
Mat<TOut>* Mat2dKrisch(Mat<T>* mat)
{
    VALRET_ASSERT(mat && 1 < mat->w && 1 < mat->h, NULL);
    Mat<TOut>* out = new Mat<TOut>(mat->w , mat->h);
    memset(out->p, 0, sizeof(TOut) * out->length);
    for(int r = 1, outSpan = out->w; r < (out->h - 1); r++)
    {
        outSpan++;
        for(int c = 1; c < (out->w - 1); c++, outSpan++)
        {
            out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::Krisch(mat, r, c);
        }
        outSpan++;
    }
    return out;
}
/**
 * @brief ���� 3 x 3 Kernel ��������˹�㷨 ( ����΢�� ) , �������ڱ�Ե����, ������������
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat ԭʼ����
 * @param type ������˹�㷨����[laplcian4 - laplcian8WithOriginMat]
 * @return Mat<TOut>* ��ȡ�ı�Ե����
 */
template<typename T, typename TOut>
Mat<TOut>* Mat2dLaplcian(Mat<T>* mat, FilterType type)
{ 
    VALRET_ASSERT(mat && 1 < mat->w && 1 < mat->h && laplcian4 <= type && type <= laplcian8WithOriginMat, NULL);
    Mat<TOut>* out = new Mat<TOut>(mat->w , mat->h);
    memset(out->p, 0, sizeof(TOut) * out->length);
    /* ��ϣ���ڱ������������� */
    switch (type)
    {
    case laplcian4:
        for(int r = 1, outSpan = out->w; r < (out->h - 1); r++)
        {
            outSpan++;
            for(int c = 1; c < (out->w - 1); c++, outSpan++)
            {
                out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::Laplcian4(mat, r, c);
            }
            outSpan++;
        }
        break;
    case laplcian8:
        for(int r = 1, outSpan = out->w; r < (out->h - 1); r++)
        {
            outSpan++;
            for(int c = 1; c < (out->w - 1); c++, outSpan++)
            {
                out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::Laplcian8(mat, r, c);
            }
            outSpan++;
        }
        break;
    case laplcian4WithOriginMat:
        for(int r = 1, outSpan = out->w; r < (out->h - 1); r++)
        {
            outSpan++;
            for(int c = 1; c < (out->w - 1); c++, outSpan++)
            {
                out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::Laplcian4WithOriginMat(mat, r, c);
            }
            outSpan++;
        }
        break;
    case laplcian8WithOriginMat:
        for(int r = 1, outSpan = out->w; r < (out->h - 1); r++)
        {
            outSpan++;
            for(int c = 1; c < (out->w - 1); c++, outSpan++)
            {
                out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::Laplcian8WithOriginMat(mat, r, c);
            }
            outSpan++;
        }
        break;
    default:
        printf("Mat2dLaplcian Input FilterType Format Error\n");
        break;
    }
    return out;
}
/**
 * @brief ���� 5 x 5 Kernel �ĸ�˹������˹����, ����� laplcian4 / laplcian8 ����ƽ��ȥ�������
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat 
 * @return Mat<TOut>* 
 */
template<typename T, typename TOut>
Mat<TOut>* Mat2dLaplcianOfGauss(Mat<T>* mat)
{ 
    VALRET_ASSERT(mat && 1 < mat->w && 1 < mat->h, NULL);
    Mat<TOut>* out = new Mat<TOut>(mat->w , mat->h);
    memset(out->p, 0, sizeof(TOut) * out->length);
    for(int r = 2, outSpan = out->w * 2; r < (out->h - 2); r++)
    {
        outSpan += 2;
        for(int c = 2; c < (out->w - 2); c++, outSpan++)
        {
            out->p[outSpan] = Mat2dDifferentiate1<T, TOut>::LaplicanOfGauss(mat, r, c);
        }
        outSpan += 2;
    }
    return out;
}

#endif