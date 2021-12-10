/*
 * @Description: ����Ԫ��ƽ��, ������Ƶ, ǿ����Ƶ��һ��ͨ�������Ȩʵ��
 * ������� : ��˹�˲�
 * �������� : ��ֵ�˲�
 * ͼ��ƽ�� : ��ֵ�˲�
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
 * @brief �����Զ����˲���, Kernel��С����Ϊ�����Σ��߳�����Ϊ����, �������������Ҳ�����ⶨ��ģ���С����Ȩ�����ֲ�, KernelԪ�ظ��� >= 2
 * 
 * @tparam T 
 * @param w kernel ���
 * @param h kernel �߶�
 * @param vecData kernel ��ֵ, һά��������ʽ����
 * @return Mat<T>* kernelָ��, �������ʧ�ܷ��ؿ�ָ�� 
 */
template<typename T>
inline Mat<T>* CreateMatTemplateKernel(int w, int h, T* vecData)
{
    VALRET_ASSERT((0 == w % 2) && (0 == h % 2) && (3 < (w + h)) && vecData, NULL);/* Kernel���������Χ 1 �������ϵ��ھ����� */
    return new Mat<T>(w, h, vecData);
}
/**
 * @brief ���� ��ֵ�˲� kernel
 * 
 * @tparam T 
 * @param w kernel ��
 * @param h kernel ��
 * @return Mat<T>* kernel ָ��
 */
template<typename T>
Mat<T>* CreateMatMeanKernel(int w, int h)
{
    VALRET_ASSERT((w & 1) && (h & 1) && (3 < (w + h)), NULL);/* ģ����������Χ 1 �������ϵ��ھ����� */
    Mat<T>* k = new Mat<T>(w, h);
    T val = 1;
    for(size_t i = 0; i < k->length; i++)
    {
        k->p[i] = val / k->length;
    }
    return k;
}
/**
 * @brief һά��˹�ֲ�
 * f(x) = 1 / (sqrt(2 * ��) * ��) * exp(- x^2 / (2 * ��^2))
 * 
 * @param x ƫ����
 * @param sigma ��׼��
 * @return double (0, 1)����ֵ
 */
inline double GaussDistribution1d(int x, double sigma)
{
    return exp(-1 * (x * x) / 2 * sigma * sigma) / (sqrt(2 * M_PI) * sigma);
}
/**
 * @brief һά��˹�ֲ�, ����ϵ��
 * f(x) = 1 / (sqrt(2 * ��) * ��) * exp(- x^2 / (2 * ��^2))
 * 
 * @param x ƫ����
 * @param sigma ��׼��
 * @return double (0, 1)����ֵ
 */
inline double GaussDistribution1dWithoutConstRaito(int x, double sigma)
{
    return exp(-(x * x) / (2 * sigma * sigma));
}
/**
 * @brief ����һά��˹��� Kernel
 * 
 * @param w kernel ����, ����Ϊ����
 * @param sigma ��̬�ֲ���׼��
 * @return double* kernel ������ָ��, ��Ҫ����free�ͷ��ڴ�, �����������᷵�� NULL
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
    for(int i = 0; i < len; i++) /* ��һ�� */
    {
        p[i] /= sum;
    }
    return p;
}
/**
 * @brief ������ھ���Ķ�ά��˹�ֲ�
 * sample: 
 * 3 x 3 kernel ��˹ģ��, x, y ȡֵ
 * (-1, 1) (0, 1) (1, 1)
 * (-1, 0) (0, 0) (1, 0)
 * (-1, -1)(0, -1)(1, -1)
 * ��ʽ:
 * f(x, y) = 1 / (2 * �� * ��^2) * exp(- (x^2 + y^2) / (2 * ��^2))
 * ����:
 * ��Խ�󣬱�ԵԽģ��
 * 
 * @param x ������ĵ� x ƫ���� 
 * @param y ������ĵ� y ƫ����
 * @param sigma ��׼��
 * @return double (0, 1)
 */
inline double GaussDistribution2d(int x, int y, double sigma)
{
    double sigmaSqrt2 = 2 * sigma * sigma;
    return exp(-1 * (x * x + y * y) / sigmaSqrt2) / (sigmaSqrt2 * M_PI);
}
/**
 * @brief ������ھ���Ķ�ά��˹�ֲ�, ���� 1 / (2 * �� * ��^2) ϵ���ļ��㣬 �ڶ�ε�����ϵ������ı�ĳ�����, ���Խ�ϵ����Ϊ���������, ����ټ��㡣
 * 
 * @param x ������ĵ� x ƫ���� 
 * @param y ������ĵ� y ƫ����
 * @param sigma ��׼��
 * @return double (0, 1) 
 */
inline double GaussDistribution2dWithoutConstRaito(int x, int y, double sigma)
{
    return exp(-1 * (x * x + y * y) / 2 * sigma * sigma);
}
/**
 * @brief ������ά��˹��� Kernel
 * 
 * @param w kernel ���, ����Ϊ����
 * @param h kernel �߶�, ����Ϊ����
 * @param sigma ��̬�ֲ���׼��
 * @return Mat<double>* kernel �����ָ��, �����������᷵�� NULL
 */
Mat<double>* CreateMatGauss2dKernel(int w, int h, double sigma)
{
    VALRET_ASSERT((w & 1) && (h & 1) && (3 < (w + h)), NULL);/* ģ����������Χ 1 �������ϵ��ھ����� */
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
    for(size_t i = 0; i < k->length; i++)/* ��һ�� */
    {
        k->p[i] /= sum;
    }
    return k;
}
/**
 * @brief ����ͼ�ݹ�� [0, res->w) �к͵� [0, res->h)�л���ͼ, �ݹ����
 * 1. ���к���
 * 2. �� 0 �к͵� 0 �����ڵݹ�ṹ�Ĺ���
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat �������
 * @param matSpan �������ָ��ƫ��
 * @param out �������ͼ����
 * @param outSpan �������ָ��ƫ��
 * @param currentCol ��ǰ����
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
 * @brief ��������ͼ, �û���ͼ���Ż���ֵ�˲�Ч�ʡ�
 * ����ͼ����, ������ԭʼ�����һ��
 * �� 0 �к͵� 0 ����Ϊ�ݹ�ṹ�Ĺ���, ��������㡣
 * refrence : https://blog.csdn.net/xiaowei_cqu/article/details/17928733
 * 
 * @tparam T 
 * @tparam TOut 
 * @param mat ԭʼ����
 * @return Mat<TOut>* ����ͼ����
 */
template<typename T, typename TOut>
Mat<TOut>* CreateIntegralMap(Mat<T>* mat)
{
    VALRET_ASSERT(mat && mat->w && mat->h, NULL);
    Mat<TOut>* res = new Mat<TOut>(mat->w + 1, mat->h + 1);
    memset(res->p, 0, res->length * sizeof(TOut));
    /* �� [0, res->w) �� ��� [0, res->h)�л���ͼ */
    size_t matVerticalSpan = 0;
    size_t outVerticalSpan = res->w + 1;
    int currentCol = 0;
    _CreateIntegralMap(mat, matVerticalSpan, res, outVerticalSpan, currentCol);
    return res; 
}
/**
 * @brief ���� kernel �� mat �������õ� out �¾���
 * ���� kernel �Ծ�����м�Ȩ����, ������ ֱ���Ҳ��޷��ƶ�����л���, ���ϵ��½��� kernel �ƶ� , ֱ�� kernel �²ഥ�׻�������㡣
 * Tips : 
 * 1. �߽粻�����κδ���
 * 
 * @tparam T 
 * @tparam TKernel 
 * @tparam TOut
 * @param mat �������
 * @param templateKernel �˲��� 
 * @param out �������
 * @return true ����ɹ�
 * @return false 1. mat �� out ���󳤿�ͬ; 2. �����ָ��
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
        for(int tkCol = tkHalfWidth; tkCol < mat->w - tkHalfWidth; tkCol++) /* ��ģ�������˲���� */
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
 * @brief ���ڻ���ͼ�ľ�ֵ�˲�, Ч�ʱȴ����ֵ kernel �ٵ��þ�� Convolution �ӿ�, Ч���Ը�
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
 * ��F(row, col) Ϊ�ҶȻ���ͼ
 * S(all)       = F(bottom, right)
 * S(top)       = F(top, right)
 * S(left)      = F(bottom, left)
 * S(top & left)= F(top, left)
 * =>
 * F(*) = F(all) - F(top) - F(left) + F(top & left)
 * 
 * @tparam T
 * @tparam TOut
 * @param mat �������
 * @param kernelWidth kernel Width, ����Ϊ����
 * @param kernelHeight kernel Height, ����Ϊ����
 * @param out �������
 * @return true 
 * @return false 1. mat �� out ���󳤿�ͬ; 2. �����ָ��; 3. kernelWidth �� kernelHeight ��Ϊ����
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
 * @brief ��ֵ�˲�, ͨ���������ָ�� kernel ��Χ�ڵ���ֵ��Ϊ��ǰ������ֵ��
 * �����Ͻǿ�ʼ, �����Һ������ƶ�
 * refrence :   
 *  https://blog.csdn.net/just_sort/article/details/87994573
 *  https://www.cnblogs.com/yoyo-sincerely/p/6058944.html
 *  https://files-cdn.cnblogs.com/files/Imageshop/MedianFilterinConstantTime.pdf
 * 
 * @param mat �������
 * @param kernelWidth  kernel ���
 * @param kernelHeight kernel �߶�
 * @param out �������
 * @return true ��ֵ�˲��ɹ�
 * @return false mat �� out Ϊ��ָ������ͼ���С�����, ���߲�Ϊ����
 */
bool Mat2dMeadianFilter(Mat<unsigned char>* mat, int kernelWidth, int kernelHeight, Mat<unsigned char>* out)
{
    VALRET_ASSERT(mat && out && out->w == mat->w && out->h == mat->h && (kernelWidth & 1) && (kernelHeight & 1), false);
/* ��ʼ�������� kernel ��ֱ��ͼ*/
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
/* ��ȡ�� meadianSize ��Ԫ�صĻҶ�ֵ */
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
/* ֱ��ͼ�����ƶ�һ��, ����ֱ��ͼ */
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
/* �ȴ����ң��ٴ��ϵ��� */
    unsigned int hist[256];
    int meadianSize = ((kernelWidth *  kernelHeight) >> 1) + 1;   /* ��ֵ, Tips : ʵ�ʿ����趨��ȡֵ��Χ[0, kernelWidth *  kernelHeight], ������������ */
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
 * @brief ��ά��˹�˲�, ����ά��˹�˲����Ϊˮƽһά��˹�˲�����ֱһά��˹�˲�, ����ڶ�ά��˹ kernel ���о������, Ч���Ը�
 * 1. ԭͼ������չ��� kernel ��С, ��Ϊ��ʱ���� buffer ��
 * 2. ˮƽһά��˹�������, �� buffer �е�ֵ(��ԭʼ�����ֵ) �� kernel ���������õ��Ľ���ŵ� buffer �С�
 * 3. ��ֱһά��˹�������, �� buffer �е�ֵ �� kernel ���������õ��Ľ���ŵ� mat �С�
 * refrence : 
 * https://blog.csdn.net/qq_36359022/article/details/80188873?utm_medium=distribute.pc_aggpage_search_result.none-task-blog-2~aggregatepage~first_rank_ecpm_v1~rank_v31_ecpm-3-80188873.pc_agg_new_rank&utm_term=%E5%BF%AB%E9%80%9F%E9%AB%98%E6%96%AF%E6%BB%A4%E6%B3%A2&spm=1000.2123.3001.4430
 * [������bug] https://github.com/devWangBin/CV-image_processing/blob/master/Gauss%26mean_filtering/Gauss%26mean_filtering%20.cpp
 * 
 * @tparam T 
 * @param mat ������� & �������
 * @param kernelWidth kernel ���
 * @param kernelHeight kernel �߶�
 * @param sigma ��׼��
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
    /* ˮƽ������һά��˹��� */                                                    
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
    /* ��ֱ������һά��˹��� */                                                    
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
