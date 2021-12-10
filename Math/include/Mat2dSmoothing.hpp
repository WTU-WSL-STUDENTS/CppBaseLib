/*
 * @Description: 矩阵元素平滑, 削弱高频, 强化低频，一般通过邻域加权实现
 * 随机噪声 : 高斯滤波
 * 椒盐噪声 : 中值滤波
 * 图像平滑 : 均值滤波
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-19 07:00:03
 * @LastEditors: like
 * @LastEditTime: 2021-12-09 21:17:58
 */
#ifndef MAT2D_SMOOTHING_HPP
#define MAT2D_SMOOTHING_HPP

#ifndef MAT2D_SMOOTHING_DEBUG
#define MAT2D_SMOOTHING_DEBUG
#undef MAT2D_SMOOTHING_DEBUG
#endif
#include <Mat.hpp>
#include <Mat2dMakeBorder.hpp>

/**
 * @brief 创建自定义滤波器, Kernel大小建议为正方形，边长必须为基数, 如果有特殊需求也可任意定义模板大小及加权参数分布, Kernel元素个数 >= 2
 * 
 * @tparam T 
 * @param w kernel 宽度
 * @param h kernel 高度
 * @param vecData kernel 的值, 一维向量的形式输入
 * @return Mat<T>* kernel指针, 如果创建失败返回空指针 
 */
template<typename T>
inline Mat<T>* CreateMatTemplateKernel(int w, int h, T* vecData)
{
    VALRET_ASSERT((0 == w % 2) && (0 == h % 2) && (3 < (w + h)) && vecData, NULL);/* Kernel必须包含周围 1 个及以上的邻居像素 */
    return new Mat<T>(w, h, vecData);
}
/**
 * @brief 创建 均值滤波 kernel
 * 
 * @tparam T 
 * @param w kernel 宽
 * @param h kernel 高
 * @return Mat<T>* kernel 指针
 */
template<typename T>
Mat<T>* CreateMatMeanKernel(int w, int h)
{
    VALRET_ASSERT((w & 1) && (h & 1) && (3 < (w + h)), NULL);/* 模板必须包含周围 1 个及以上的邻居像素 */
    Mat<T>* k = new Mat<T>(w, h);
    T val = 1;
    for(size_t i = 0; i < k->length; i++)
    {
        k->p[i] = val / k->length;
    }
    return k;
}
/**
 * @brief 一维高斯分布
 * f(x) = 1 / (sqrt(2 * Π) * σ) * exp(- x^2 / (2 * σ^2))
 * 
 * @param x 偏移量
 * @param sigma 标准差
 * @return double (0, 1)返回值
 */
inline double GaussDistribution1d(int x, double sigma)
{
    return exp(-1 * (x * x) / 2 * sigma * sigma) / (sqrt(2 * M_PI) * sigma);
}
/**
 * @brief 一维高斯分布, 忽略系数
 * f(x) = 1 / (sqrt(2 * Π) * σ) * exp(- x^2 / (2 * σ^2))
 * 
 * @param x 偏移量
 * @param sigma 标准差
 * @return double (0, 1)返回值
 */
inline double GaussDistribution1dWithoutConstRaito(int x, double sigma)
{
    return exp(-(x * x) / (2 * sigma * sigma));
}
/**
 * @brief 创建一维高斯卷积 Kernel
 * 
 * @param w kernel 长度, 必须为奇数
 * @param sigma 正态分布标准差
 * @return double* kernel 向量的指针, 需要调用free释放内存, 输入参数错误会返回 NULL
 */
double* CreateMatGauss1dKernel(int len, double sigma)
{
    VALRET_ASSERT((len & 1) && 2 < len, NULL);
    double* p = (double*)malloc(sizeof(double) * len);
    double sum = 0;
    int half  = len >> 1;
    for(int i = 0; i < len; i++)
    {
        p[i] = GaussDistribution1dWithoutConstRaito(i - half, sigma);
        sum += p[i];
    }
    for(int i = 0; i < len; i++) /* 归一化 */
    {
        p[i] /= sum;
    }
    return p;
}
/**
 * @brief 计算基于距离的二维高斯分布
 * sample: 
 * 3 x 3 kernel 高斯模板, x, y 取值
 * (-1, 1) (0, 1) (1, 1)
 * (-1, 0) (0, 0) (1, 0)
 * (-1, -1)(0, -1)(1, -1)
 * 公式:
 * f(x, y) = 1 / (2 * Π * σ^2) * exp(- (x^2 + y^2) / (2 * σ^2))
 * 结论:
 * σ越大，边缘越模糊
 * 
 * @param x 相对中心的 x 偏移量 
 * @param y 相对中心的 y 偏移量
 * @param sigma 标准差
 * @return double (0, 1)
 */
inline double GaussDistribution2d(int x, int y, double sigma)
{
    double sigmaSqrt2 = 2 * sigma * sigma;
    return exp(-1 * (x * x + y * y) / sigmaSqrt2) / (sigmaSqrt2 * M_PI);
}
/**
 * @brief 计算基于距离的二维高斯分布, 忽略 1 / (2 * Π * σ^2) 系数的计算， 在多次调用且系数不会改变的场景下, 可以将系数作为公因数提出, 最后再计算。
 * 
 * @param x 相对中心的 x 偏移量 
 * @param y 相对中心的 y 偏移量
 * @param sigma 标准差
 * @return double (0, 1) 
 */
inline double GaussDistribution2dWithoutConstRaito(int x, int y, double sigma)
{
    return exp(-1 * (x * x + y * y) / 2 * sigma * sigma);
}
/**
 * @brief 创建二维高斯卷积 Kernel
 * 
 * @param w kernel 宽度, 必须为奇数
 * @param h kernel 高度, 必须为奇数
 * @param sigma 正态分布标准差
 * @return Mat<double>* kernel 矩阵的指针, 输入参数错误会返回 NULL
 */
Mat<double>* CreateMatGauss2dKernel(int w, int h, double sigma)
{
    VALRET_ASSERT((w & 1) && (h & 1) && (3 < (w + h)), NULL);/* 模板必须包含周围 1 个及以上的邻居像素 */
    Mat<double>* k = new Mat<double>(w, h);
    double* p = k->p;
    double sum = 0;
    int halfWidth  = w >> 1;
    int halfHeight = h >> 1;
    for(int r = halfHeight; r >= -1 * halfHeight; r--)
    {
        for(int c = -1 * halfWidth; c <= halfWidth; c++, p++)
        {
            *p = GaussDistribution2dWithoutConstRaito(c, r, sigma);
            sum += *p;
        }
    }
    for(size_t i = 0; i < k->length; i++)/* 归一化 */
    {
        k->p[i] /= sum;
    }
    return k;
}
/**
 * @brief 积分图递归第 [0, res->w) 行和第 [0, res->h)列积分图, 递归入口
 * 1. 先列后行
 * 2. 第 0 列和第 0 行用于递归结构的构造
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat 输入矩阵
 * @param matSpan 输入矩阵指针偏移
 * @param out 输出积分图矩阵
 * @param outSpan 输出矩阵指针偏移
 * @param currentCol 当前列数
 */
template<typename T, typename TOut>
void _CreateIntegralMap(Mat<T>* mat, size_t& matSpan, Mat<TOut>* out, size_t& outSpan, int& currentCol)
{
    out->p[outSpan] = mat->p[matSpan] + out->p[outSpan - out->w] + out->p[outSpan - 1] - out->p[outSpan - out->w - 1];
    matSpan += mat->w;
    outSpan += out->w;
    if(matSpan < mat->length)
    {
        _CreateIntegralMap(mat, matSpan, out, outSpan, currentCol);
    }
    else if(++currentCol < mat->w)
    {
        matSpan = currentCol;
        outSpan = out->w + currentCol + 1;
        _CreateIntegralMap(mat, matSpan, out, outSpan, currentCol);
    }
}
/**
 * @brief 创建积分图, 用积分图可优化均值滤波效率。
 * 积分图矩阵, 长宽是原始矩阵加一。
 * 第 0 列和第 0 行作为递归结构的构造, 不参与计算。
 * refrence : https://blog.csdn.net/xiaowei_cqu/article/details/17928733
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat 原始矩阵
 * @return Mat<TOut>* 积分图矩阵
 */
template<typename T, typename TOut>
Mat<TOut>* CreateIntegralMap(Mat<T>* mat)
{
    VALRET_ASSERT(mat && mat->w && mat->h, NULL);
    Mat<TOut>* res = new Mat<TOut>(mat->w + 1, mat->h + 1);
    memset(res->p, 0, res->length * sizeof(TOut));
    /* 第 [0, res->w) 行 或第 [0, res->h)列积分图 */
    size_t matVerticalSpan = 0;
    size_t outVerticalSpan = res->w + 1;
    int currentCol = 0;
    _CreateIntegralMap(mat, matVerticalSpan, res, outVerticalSpan, currentCol);
    return res; 
}
/**
 * @brief 任意 kernel 与 mat 计算卷积得到 out 新矩阵。
 * 利用 kernel 对矩阵进行加权计算, 从左到右 直到右侧无法移动会进行换行, 从上到下进行 kernel 移动 , 直到 kernel 下侧触底会结束计算。
 * Tips : 
 * 1. 边界不会做任何处理
 * 
 * @tparam T 
 * @tparam TKernel 
 * @tparam TOut
 * @param mat 输入矩阵
 * @param templateKernel 滤波器 
 * @param out 输出矩阵
 * @return true 计算成功
 * @return false 1. mat 和 out 矩阵长宽不同; 2. 传入空指针
 */
template<typename T, typename TKernel, typename TOut>
bool Convolution(Mat<T>* mat, Mat<TKernel>* templateKernel, Mat<TOut>* out)
{
    VALRET_ASSERT(mat && out &&templateKernel && out->w == mat->w && out->h == mat->h, false);

    int tkHalfHeight = templateKernel->h >> 1;
    int tkHalfWidth  = templateKernel->w >> 1;
    int top, left;
#ifdef MAT2D_SMOOTHING_DEBUG
    printf("%d, %d\n", tkHalfHeight, tkHalfWidth);
#endif
    TOut sum;
    for(int tkRow = tkHalfHeight; tkRow < mat->h - tkHalfHeight; tkRow++)
    {
        for(int tkCol = tkHalfWidth; tkCol < mat->w - tkHalfWidth; tkCol++) /* 掩模与矩阵相乘并求和 */
        {
            sum = 0;
            top = tkRow - tkHalfHeight;
            left = tkCol - tkHalfWidth;
            for(int r = 0; r < templateKernel->h; r++)
            {
                for(int c = 0; c < templateKernel->w; c++)
                {
                    // sum += templateKernel->p[r * templateKernel->w + c] *  mat->p[(tkRow - tkHalfHeight + r) * mat->w + (tkCol - tkHalfWidth) + c];
                    sum += templateKernel->p[r * templateKernel->w + c] * mat->p[(top + r) * mat->w + left + c];
#ifdef MAT2D_SMOOTHING_DEBUG
                    printf("(%d, %d):[%d, %f]\n", (tkRow - tkHalfHeight + r) * mat->w + (tkCol - tkHalfWidth) + c, r * templateKernel->w + c,
                    mat->p[(tkRow - tkHalfHeight + r) * mat->w + (tkCol - tkHalfWidth) + c] , templateKernel->p[r * templateKernel->w + c]);
#endif
                }
            }
#ifdef MAT2D_SMOOTHING_DEBUG
            printf(" => %d, %d, %f\n", tkRow, tkCol, sum);
#endif
            out->p[tkRow * out->w + tkCol] = sum;
        }
    }
    return true;
}
/**
 * @brief 基于积分图的均值滤波, 效率比传入均值 kernel 再调用卷积 Convolution 接口, 效率略高
 *  top & left \ -------top------
 *            | \----------------|
 *            | |\\\ |           |
 *            | |----------------|
 *      left  | |    |     *     | 
 *            | |----------------|
 *                              all
 * all area :
 *  \----------------|
 *  |\\\             |
 *  |                |
 *  |           *    | 
 *  |----------------|
 * top area :
 *  \----------------|
 *  |\\\             |
 *  |----------------|
 * left area :
 *  \-----
 *  |\\\ |
 *  |    -
 *  |    |
 *  |-----
 * top & left area :
 *  \-----
 *  |\\\ |
 *  |-----
 * 
 * S(*) = S(all) - S(top) - S(left) + S(top & left)
 * 设F(row, col) 为灰度积分图
 * S(all)       = F(bottom, right)
 * S(top)       = F(top, right)
 * S(left)      = F(bottom, left)
 * S(top & left)= F(top, left)
 * =>
 * F(*) = F(all) - F(top) - F(left) + F(top & left)
 * 
 * @tparam T
 * @tparam TOut
 * @param mat 输入矩阵
 * @param kernelWidth kernel Width, 必须为奇数
 * @param kernelHeight kernel Height, 必须为奇数
 * @param out 输出矩阵
 * @return true 
 * @return false 1. mat 和 out 矩阵长宽不同; 2. 传入空指针; 3. kernelWidth 或 kernelHeight 不为奇数
 */
template<typename T, typename TOut>
bool Mat2dMean(Mat<T>* mat, int kernelWidth, int kernelHeight, Mat<TOut>* out)
{
    VALRET_ASSERT(mat && out && out->w == mat->w && out->h == mat->h && (kernelWidth & 1) && (kernelHeight & 1), false);
    Mat<unsigned int>* integralMap = CreateIntegralMap<T, unsigned int>(mat);
#ifdef MAT2D_SMOOTHING_DEBUG
    printf("-----------------\nIntegral Map\n%s-----------------\n", integralMap->ToMatString().c_str());
#endif
    int tkHalfHeight = kernelHeight>> 1;
    int tkHalfWidth  = kernelWidth >> 1;
    int top, left, bottom, right;
    unsigned int sum;
    double rate = 1.0 / (kernelWidth * kernelHeight);
    for(int tkRow = tkHalfHeight, matRowSpan = 0; tkRow < mat->h - tkHalfHeight; tkRow++)
    {
        matRowSpan = tkRow * mat->w; 
        for(int tkCol = tkHalfWidth; tkCol < mat->w - tkHalfWidth; tkCol++)
        {
            top     = tkRow - tkHalfHeight;
            left    = tkCol - tkHalfWidth;
            bottom  = top   + kernelHeight;
            right   = left  + kernelWidth;
            sum = 
                integralMap->p[integralMap->w * bottom  + right]+   /* all */
                integralMap->p[integralMap->w * top     + left] -   /* top & left */
                integralMap->p[integralMap->w * top     + right]-   /* top */
                integralMap->p[integralMap->w * bottom  + left] ;   /* left */
#ifdef MAT2D_SMOOTHING_DEBUG
            printf("(%d, %d, %d, %d) : [%d, %d, %d, %d]\nresult=>%d\n", integralMap->w * (tkRow + tkHalfHeight + 1) + tkCol + tkHalfWidth + 1, 
                integralMap->w * (top + 0) + left + 0,
                integralMap->w * (top + 0) + tkCol + tkHalfWidth + 1,
                integralMap->w * (tkRow + tkHalfHeight + 1) + left + 0,
                integralMap->p[integralMap->w * (tkRow + tkHalfHeight + 1) + tkCol + tkHalfWidth + 1],
                integralMap->p[integralMap->w * (top + 0) + left + 0],
                integralMap->p[integralMap->w * (top + 0) + tkCol + tkHalfWidth + 1],
                integralMap->p[integralMap->w * (tkRow + tkHalfHeight + 1) + left + 0],
                sum
            );
#endif
            out->p[matRowSpan + tkCol] = (TOut)(rate * sum);
        }
    }
    return true;
}
/**
 * @brief 中值滤波, 通过排序计算指定 kernel 范围内的中值视为当前矩阵点的值。
 * 从左上角开始, 先向右后向下移动
 * refrence :   
 *  https://blog.csdn.net/just_sort/article/details/87994573
 *  https://www.cnblogs.com/yoyo-sincerely/p/6058944.html
 *  https://files-cdn.cnblogs.com/files/Imageshop/MedianFilterinConstantTime.pdf
 * 
 * @param mat 输入矩阵
 * @param kernelWidth  kernel 宽度
 * @param kernelHeight kernel 高度
 * @param out 输出矩阵
 * @return true 中值滤波成功
 * @return false mat 或 out 为空指针或二者图像大小不相等, 或宽高不为奇数
 */
bool Mat2dMeadianFilter(Mat<unsigned char>* mat, int kernelWidth, int kernelHeight, Mat<unsigned char>* out)
{
    VALRET_ASSERT(mat && out && out->w == mat->w && out->h == mat->h && (kernelWidth & 1) && (kernelHeight & 1), false);
/* 初始化最左侧的 kernel 的直方图*/
#define INIT_LEFT_BEGIN_HISTGRAM(mat, hist, top, left, bottom, right)   \
    do                                                                  \
    {                                                                   \
        memset(hist, 0, sizeof(hist));                                  \
        int beginHistogramSpan = top * mat->w + left;                   \
        for(int r = top; r < bottom; r++, beginHistogramSpan += mat->w) \
        {                                               \
            for(int c = left; c < right; c++)           \
            {                                           \
                hist[mat->p[beginHistogramSpan + c]]++; \
            }                                           \
        }                                               \
    }while(0)
/* 获取第 meadianSize 个元素的灰度值 */
#define GET_MEADIAN_VALUE(hist, meadianSize, val)\
    do                              \
    {                               \
        int count   = 0;            \
        int i       = 0;            \
        while(count < meadianSize)  \
        {                           \
            count += hist[i++];     \
        }                           \
        val = --i;                  \
    }while(0)
/* 直方图向右移动一格, 更新直方图 */
#define MOVE_HISTGRAM_RIGHT(mat, hist, top, left, bottom, right) \
    do  \
    {   \
        int span = top * mat->w;                \
        int leftHistogramSpan  = span + left;   \
        int rightHistogramSpan = span + right;  \
        for(int r = top; r < bottom; r++, leftHistogramSpan += mat->w, rightHistogramSpan += mat->w) \
        {                                       \
            hist[mat->p[leftHistogramSpan]]--;  \
            hist[mat->p[rightHistogramSpan]]++; \
        }                                       \
    }while(0)
/* 先从左到右，再从上到下 */
    unsigned int hist[256];
    int meadianSize = ((kernelWidth *  kernelHeight) >> 1) + 1;   /* 中值, Tips : 实际可以设定的取值范围[0, kernelWidth *  kernelHeight], 会有其他意义 */
    int tkHalfHeight = kernelHeight>> 1;
    int tkHalfWidth  = kernelWidth >> 1;
    for
    (
        int tkRow = tkHalfHeight, top = 0, bottom = kernelHeight;
        tkRow < mat->h - tkHalfHeight; 
        tkRow++, top++, bottom++
    )
    {
        INIT_LEFT_BEGIN_HISTGRAM(mat, hist, top, 0, bottom, kernelWidth);
        for
        (
            int tkCol = tkHalfWidth, left = 0, right = kernelWidth - 1; 
            tkCol < mat->w - tkHalfWidth; 
            tkCol++, left++, right++
        )
        {
            MOVE_HISTGRAM_RIGHT(mat, hist, top, left, bottom, right);
            GET_MEADIAN_VALUE(hist, meadianSize, out->p[tkRow * mat->w + tkCol]);
        }
    }
    return true;
}
/**
 * @brief 二维高斯滤波, 将二维高斯滤波拆分为水平一维高斯滤波和竖直一维高斯滤波, 相比于二维高斯 kernel 进行卷积运算, 效率稍高
 * 1. 原图长宽扩展半个 kernel 大小, 设为临时矩阵 buffer 。
 * 2. 水平一维高斯卷积运算, 将 buffer 中的值(即原始矩阵的值) 与 kernel 做卷积运算得到的结果放到 buffer 中。
 * 3. 竖直一维高斯卷积运算, 将 buffer 中的值 与 kernel 做卷积运算得到的结果放到 mat 中。
 * refrence : 
 * https://blog.csdn.net/qq_36359022/article/details/80188873?utm_medium=distribute.pc_aggpage_search_result.none-task-blog-2~aggregatepage~first_rank_ecpm_v1~rank_v31_ecpm-3-80188873.pc_agg_new_rank&utm_term=%E5%BF%AB%E9%80%9F%E9%AB%98%E6%96%AF%E6%BB%A4%E6%B3%A2&spm=1000.2123.3001.4430
 * [代码有bug] https://github.com/devWangBin/CV-image_processing/blob/master/Gauss%26mean_filtering/Gauss%26mean_filtering%20.cpp
 * 
 * @tparam T 
 * @param mat 输入矩阵 & 输出矩阵
 * @param kernelWidth kernel 宽度
 * @param kernelHeight kernel 高度
 * @param sigma 标准差
 * @return true 
 * @return false 
 */
template<typename T>
bool Mat2dGaussFilter(Mat<T>* mat, int kernelWidth, int kernelHeight, double sigma)
{                                                      
    VALRET_ASSERT(mat && (kernelWidth & 1) && (kernelHeight & 1) && 0 < sigma, false);
    double* horizonGauss = CreateMatGauss1dKernel(kernelWidth, sigma);
    double* verticalGauss = kernelWidth == kernelHeight ? horizonGauss: CreateMatGauss1dKernel(kernelHeight, sigma);
    VALRET_ASSERT(horizonGauss && verticalGauss, false);
#ifdef MAT2D_SMOOTHING_DEBUG
    printf("----------------------------\n Horizon Gauss Kernel\n");
    for(int i = 0; i < kernelWidth; i++)
    {
        printf("%f, ", horizonGauss[i]);
    }
    printf("\n----------------------------\n Vertical Gauss Kernel\n");
    for(int i = 0; i < kernelHeight; i++)
    {
        printf("%f, ", verticalGauss[i]);
    }
    printf("\n----------------------------\n");
#endif

    int tkHalfHeight = kernelHeight>> 1;
    int tkHalfWidth  = kernelWidth >> 1;
    Mat<T>* borderGroupedMat = CopyMakeBorderReflect_101(mat, tkHalfHeight, tkHalfWidth, tkHalfHeight, tkHalfWidth);
    VALRET_ASSERT(borderGroupedMat, false);    
#ifdef MAT2D_SMOOTHING_DEBUG
    printf("borderGroupedMat width, height = %d, %d\n%s---------------------------\n", borderGroupedMat->w, borderGroupedMat->h, borderGroupedMat->ToMatString().c_str());
#endif
    double sum;      
    /* 水平方向上一维高斯卷积 */                                                    
    for(int tkRow = tkHalfHeight; tkRow < borderGroupedMat->h - tkHalfHeight; tkRow++)                                                               
    {                                             
        for(int tkCol = tkHalfWidth; tkCol < borderGroupedMat->w - tkHalfWidth; tkCol++)                                                           
        {                                                           
            sum = 0;                                                
            for(int i = -tkHalfWidth; i <= tkHalfWidth; i++)                            
            {                                                       
                sum += horizonGauss[i + tkHalfWidth] * borderGroupedMat->p[borderGroupedMat->w * tkRow + tkCol + i];    
            }    
            borderGroupedMat->p[tkRow * borderGroupedMat->w + tkCol] = (T)sum;                   
        }                                                           
    }  
    /* 竖直方向上一维高斯卷积 */                                                    
    for(int tkRow = tkHalfHeight; tkRow < borderGroupedMat->h - tkHalfHeight; tkRow++)                                                               
    {                                                                 
        for(int tkCol = tkHalfWidth; tkCol < borderGroupedMat->w - tkHalfWidth; tkCol++)                                                           
        {                                                           
            sum = 0;                                                
            for(int i = -tkHalfWidth; i <= tkHalfWidth; i++)                            
            {                                                       
                sum += horizonGauss[i] * borderGroupedMat->p[borderGroupedMat->w * (tkRow + i) + tkCol];    
            }     
            mat->p[(tkRow - tkHalfHeight) * mat->w + tkCol - tkHalfWidth] = (T)sum;                 
        }                                                           
    }  
    delete borderGroupedMat;
    /* free gauss kernel */
    if(verticalGauss == horizonGauss)
    {
        free(horizonGauss);
    }
    else
    {
        free(horizonGauss);
        free(verticalGauss);
    }
    return true;
}

#endif
