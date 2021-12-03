/*
 * @Description: 图像锐化 : 微分运算, 梯度锐化, 边缘检测 
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
 * # 图像锐化
 * ## 边缘类型
 * - 阶梯状
 * | black | white |
 *  灰度:
 *          /----
 *      ---/
 *  一阶导数(差分):
 * 
 *           ||
 *      ----/  \----
 *  二阶导数:
 *          /\
 *      ----  \  ----
 *             \/
 *  - 脉冲状
 *  | black | white | black |
 *  灰度:
 *           /----\
 *      ----/      \-----
 *  一阶导数(差分):
 * 
 *           /--\
 *      ----/    \  ----
 *                \/
 *  二阶导数:
 *          /\
 *      ----  \  ----
 *             \/
 * ## 微分算法
 * - 一阶 / 二阶微分运算
 * dx = f(x + 1) - f(x)
 * dx^2 = f(x + 1) + f(x - 1) - 2f(x)
 *
 * - 单向微分运算
 *  h(r, c) = f(r, c + 1) - f(r, c)
 *  v(r, c) = f(r + 1, c) - f(r, c)
 * 
 * - 双向一阶微分
 *  g(r, c) = h(r, c)^2 + v(r, c)^2
 * 
 * ## 高通滤波器, 梯度锐化 kernel 之一阶微分
 * - robert kernel : 交叉微分
 * 构造 x, y 方向的 kernel :
 * [-1, 0]   [0, -1]
 * [ 0, 1] & [1,  0]
 * ->
 * rob(x) = | f(1, 1) - f(0, 0) |
 * rob(y) = | f(1, 0) - f(0, 1) |
 * ▽f    = rob(x) + rob(y)
 * 
 * - sobel kernel      
 * 构造 x, y 方向的 kernel : 
 * [-1, -2, -1]   [-1, 0, -1]
 * [0 , 0 , 0 ] & [-2, 0, -2]
 * [1 , 2 , 1 ]   [-1, 0, -1]
 * 边缘方向上进行了加权平滑
 * ->
 * sob(x) = -f(0, 0) - 2f(0, 1) - f(0, 2) + f(2, 0) + 2f(2, 1) + f(2, 2)
 * sob(y) = -f(0, 0) - 2f(1, 0) - f(2, 0) + f(0, 2) + 2f(1, 2) + f(2, 2)
 *  ▽f   = sob(x) + sob(y)
 * 
 * - prewitt kernel
 * 构造 x, y 方向的 kernel : 
 * [-1, -1, -1]   [-1, 0, -1]
 * [0 , 0 , 0 ] & [-1, 0, -1]
 * [1 , 1 , 1 ]   [-1, 0, -1]
 * 与 sobel 相比在边缘方向上取消了加权平滑
 * 
 * ## 高通滤波器, 梯度锐化 kernel 之二阶微分
 * - laplcian kernel
 * 强化高频, 弱化低频, 导致原图低频信息丢失。
 * 
 * x, y 方向梯度
 * ->
 * dx-lap(x, y) = | f(x + 1, y) + f(x - 1, y) - 2f(x, y) |
 * dy-lap(x, y) = | f(x, y + 1) + f(x, y - 1) - 2f(x, y) |
 * -> 向量方向梯度 
 * ▽^2f(x, y) =  dy-lap(x, y) + dx-lap(x, y) 
 * -> 依据公式可构造 kernel :
 * [0,  1, 0] laplcian  [1,  1, 1]
 * [1, -4, 1] 类似高斯型 [1, -8, 1]
 * [0,  1, 0] 可扩展为   [1,  1, 1]
 * -> 由于最后结果被绝对值包裹, 可将 kernel 整体取相反数
 * [ 0, -1,  0]     [-1, -1, -1]
 * [-1,  4, -1]     [-1,  8, -1]
 * [ 0, -1,  0]     [-1, -1, -1]
 * 
 * - 解决原图低频消失的方法
 * 原图和 laplcian 图像进行叠加, 强化高频信息, 保留低频信息。
 *        |- α * f(x, y) + ▽^2f(x, y) : 中心系数为正
 * g(x) = |
 *        |- α * f(x, y) - ▽^2f(x, y) : 中心系数为负
 * 
 * 通常的, 令 α = 1 ->
 * g(x) = f(x, y) - | f(x + 1, y) + f(x - 1, y) - 2f(x, y) + f(x, y + 1) + f(x, y - 1) - 2f(x, y)) |
 *      = 5f(x, y) - f(x + 1, y) - f(x - 1, y) - f(x, y + 1) - f(x, y - 1)
 * -> 构造出包含原图信息的 kernel
 * [ 0, -1,  0]     [-1, -1, -1]
 * [-1,  5, -1] 或者[-1,  9, -1]  
 * [ 0, -1,  0]     [-1, -1, -1]
 * 
 * - gauss & laplcian kernel 
 * 效果更好的边缘检测器, 把高斯平滑和拉普拉斯锐化结合起来, 先去噪后进行边缘检测。
 * ->
 * [-2, -4, -4, -4, -2]
 * [-4,  0,  8,  0, -4]
 * [-4,  8, 24,  8, -4]
 * [-4,  0,  8,  0, -4]
 * [-2, -4, -4, -4, -2]
 * 
 * - kirsch kernel
 * 规定了一组 kernel 用于自适应检测
 * [-1 0 1] [-1 -1 -1]  [ 0  1  1]      [0 -1 -1]
 * [-1 0 1] [ 0  0  0]  [-1  0  1]      [1  0 -1]
 * [-1 0 1] [ 1  1  1]  [-1 -1  0]      [1  1  0]
 * 水平方向   竖直方向  45度方向查找边界   135度方向查找边界
 * 
 * 多个 kernel 求多次卷积, 哪个 kernel 卷积结果最大, 认为在该方向上是边缘
 * 
 * 
 */


/**
 * @brief 创建一个 2 x 2 的 robert kernel , 由于长度为偶数 , 卷积运算的结果存放位置与实际值偏移了 0.5 个单位
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
 * @brief 创建一个 3 x 3 的 sobel kernel , 在边缘方向上会进行高斯平滑
 * 
 * @tparam T 
 * @param horizonKernel sobel 水平 kernel
 * @param verticalKernel sobel 竖直 kernel
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
 * @brief 创建一个 3 x 3 的 prewitt kernel , sobel 削弱版, 没有边缘方向上的平滑处理
 * 
 * @tparam T 
 * @param horizonKernel sobel 水平 kernel
 * @param verticalKernel sobel 竖直 kernel
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
    x,          /* 水平方向一次微分           */
    y,          /* 竖直方向一次微分           */
    max_xy,     /* 水平方向或竖直方向取最大值  */
    sum_sqrt,   /* 双向一次微分之取距离        */
    sum_abs,    /* 双向一次微分之取绝对值      */
    robert,     /* robert 交叉微分            */
    prewitt,    /* prewitt 算子*/
    sobel,      /* sobel 算子, prewitt 算子基础上平滑了边缘 */
    krisch,     /* 8 个方向上取最优解作为边缘 */
    laplcian4,  /* 拉普拉斯算子, 二阶微分, 中心值为 4 */
    laplcian4WithOriginMat,  /* laplcian4 基础上保留原始矩阵信息 */
    laplcian8,  /* 中心值为 8 */
    laplcian8WithOriginMat,  /* laplcian8 基础上保留原始矩阵信息 */
    LOG,        /* 高斯平滑去噪后, 调用拉普拉斯 */
    FilterTypeCount
}FilterType;
/**
 * @brief 一阶微分算法实现
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
    /* laplcian4WithOriginMat , laplcian8WithOriginMat 算子中原始矩阵的强化系数, 默认值为 1 */
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
 * @brief 计算矩阵一阶微分, 获取边缘信息
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat 原始矩阵
 * @param type 导数计算方法
 * @return Mat<TOut>* 返回一阶导数计算的结果, 长宽和原始矩阵相比少 1 , 忽略了最右列和最下行
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
 * @brief 基于Robert Kernel (交叉微分) 的边缘提取
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat 原始矩阵
 * @return Mat<TOut>* 提取的边缘矩阵
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
 * @brief 基于 3 x 3 Sobel Kernel 的边缘提取
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat 原始矩阵
 * @return Mat<TOut>* 提取的边缘矩阵
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
 * @brief 基于 3 x 3 Prewitt Kernel 的边缘提取
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat 原始矩阵
 * @return Mat<TOut>* 提取的边缘矩阵
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
 * @brief 基于 8 个不同角度方向的 Kernel 取最优解的边缘提取, 计算量比较大
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat 原始矩阵
 * @return Mat<TOut>* 提取的边缘矩阵
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
 * @brief 基于 3 x 3 Kernel 的拉普拉斯算法 ( 二阶微分 ) , 不依赖于边缘方向, 但对噪声敏感
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat 原始矩阵
 * @param type 拉普拉斯算法类型[laplcian4 - laplcian8WithOriginMat]
 * @return Mat<TOut>* 提取的边缘矩阵
 */
template<typename T, typename TOut>
Mat<TOut>* Mat2dLaplcian(Mat<T>* mat, FilterType type)
{ 
    VALRET_ASSERT(mat && 1 < mat->w && 1 < mat->h && laplcian4 <= type && type <= laplcian8WithOriginMat, NULL);
    Mat<TOut>* out = new Mat<TOut>(mat->w , mat->h);
    memset(out->p, 0, sizeof(TOut) * out->length);
    /* 寄希望于编译器进行内联 */
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
 * @brief 基于 5 x 5 Kernel 的高斯拉普拉斯算子, 相比于 laplcian4 / laplcian8 具有平滑去噪的能力
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