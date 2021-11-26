/*
 * @Description: 矩阵平滑测试用例
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-19 09:18:00
 * @LastEditors: like
 * @LastEditTime: 2021-11-26 06:54:54
 */
// #define MAT2D_SMOOTHING_DEBUG
#define SMOOTHING_WIDTH 9
#define SMOOTHING_HEIGHT 9
#include <Mat2dSmoothing.hpp>

/**
 * @brief 通过输入不同 kernel , 进行卷积运算, 实现图像的光滑(均值 kernel) / 图像的去随机噪声(gauss kernel)
 * 
 * @param kernel 
 * @return int 
 */
int SmoothingTest(Mat<double>* kernel)
{
    unsigned char matData[SMOOTHING_WIDTH * SMOOTHING_HEIGHT] = {0};
    for(int r = 0; r < SMOOTHING_HEIGHT; r++)
    {
        memset(matData + (r * SMOOTHING_WIDTH), r * 2 + 1, SMOOTHING_WIDTH);
    }
    Mat<unsigned char>* mat = new Mat<unsigned char>(SMOOTHING_WIDTH, SMOOTHING_HEIGHT, matData);
    Mat<double>* matConvolut = new Mat<double>(SMOOTHING_WIDTH, SMOOTHING_HEIGHT);
    memset(matConvolut->p, 0, matConvolut->length);
    printf("----------------------------\norigin mat\n%s", mat->ToMatString().c_str());
    if(!Convolution(mat, kernel, matConvolut))
    {
        printf("Convolution Failed\n");    
        delete matConvolut;
        matConvolut = NULL;
        delete mat;
        mat = NULL;
        delete kernel;
        kernel = NULL;
        return 1;
    }
    printf("----------------------------\nConvolutted mat\n%s", matConvolut->ToMatString().c_str());
    
    delete matConvolut;
    matConvolut = NULL;
    delete mat;
    mat = NULL;
    return 0;
}
/**
 * @brief 积分图测试 测试用例
 * 
 * @return int 
 */
int CreateIntegralMapTest()
{
    unsigned char matData[SMOOTHING_WIDTH * SMOOTHING_HEIGHT];
    for(int r = 0; r < SMOOTHING_HEIGHT; r++)
    {
        for(int c = 0; c < SMOOTHING_WIDTH; c++)
        {
            matData[r * SMOOTHING_WIDTH + c] = r + c;
        }
    }
    Mat<unsigned char>* mat = new Mat<unsigned char>(SMOOTHING_WIDTH, SMOOTHING_HEIGHT, matData);
    printf("----------------------------\norigin mat\n%s", mat->ToMatString().c_str());

    Mat<unsigned int>* integralMap = CreateIntegralMap<unsigned char, unsigned int>(mat);
    if(NULL == integralMap)
    {
        delete mat;
        mat = NULL;
        printf("Create Integral Map Failed \n");
        return 1;
    }
    printf("----------------------------\n Integral Map\n%s", integralMap->ToMatString().c_str());
    
    delete integralMap;
    integralMap = NULL;
    delete mat;
    mat = NULL;
    return 0;
}
/**
 * @brief 均值滤波 测试用例
 * 
 * @return int 
 */
int Mat2dMeanFilter()
{
    unsigned char matData[SMOOTHING_WIDTH * SMOOTHING_HEIGHT];
    for(int r = 0; r < SMOOTHING_HEIGHT; r++)
    {
        memset(matData + (r * SMOOTHING_WIDTH), r * 2 + 1, SMOOTHING_WIDTH);
    }
    Mat<unsigned char>* mat = new Mat<unsigned char>(SMOOTHING_WIDTH, SMOOTHING_HEIGHT, matData);
    Mat<unsigned char>* out = new Mat<unsigned char>(SMOOTHING_WIDTH, SMOOTHING_HEIGHT);
    // memcpy(out->p, mat->p, mat->length * sizeof(unsigned char));
    memset(out->p, 0, out->length * sizeof(unsigned char));
    printf("----------------------------\norigin mat\n%s", mat->ToMatString().c_str());
    
    if(!Mat2dMean(mat, 3, 3, out))
    {
        printf("Mean Filter Failed\n");    
        delete out;
        out = NULL;
        delete mat;
        mat = NULL;
        return 1;
    }
    printf("----------------------------\n Mean mat Result\n%s", out->ToMatString().c_str());
    delete out;
    out = NULL;
    delete mat;
    mat = NULL;
    return 0;
}
/**
 * @brief 中值滤波 测试用例
 * 
 * @return int 
 */
int Mat2dMeadianFilter()
{
    unsigned char matData[SMOOTHING_WIDTH * SMOOTHING_HEIGHT] = {0};
    for(int r = 0; r < SMOOTHING_HEIGHT; r++)
    {
        matData[r * SMOOTHING_WIDTH + r] = 255;
    }
    Mat<unsigned char>* mat = new Mat<unsigned char>(SMOOTHING_WIDTH, SMOOTHING_HEIGHT, matData);
    Mat<unsigned char>* out = new Mat<unsigned char>(SMOOTHING_WIDTH, SMOOTHING_HEIGHT);
    memset(out->p, 1, out->length * sizeof(unsigned char));
    printf("----------------------------\n Origin Mat\n%s", mat->ToMatString().c_str());
    
    if(!Mat2dMeadianFilter(mat, 3, 3, out))
    {
        printf("Meadian Filter Failed\n");    
        delete out;
        out = NULL;
        delete mat;
        mat = NULL;
        return 1;
    }
    printf("----------------------------\n Meadian mat Result\n%s", out->ToMatString().c_str());
    delete out;
    out = NULL;
    delete mat;
    mat = NULL;
    return 0;
}
/**
 * @brief 高斯滤波 测试用例
 * 
 * @return int 
 */
int Mat2dGaussFilter()
{
    double matData[SMOOTHING_WIDTH * SMOOTHING_HEIGHT] = {0};
    for(int r = 0; r < SMOOTHING_HEIGHT; r++)
    {
        for(int c = 0; c < SMOOTHING_WIDTH; c++)
        {
            matData[r * SMOOTHING_WIDTH + c] = r * 2 + 1;
        }
    }
    Mat<double>* mat = new Mat<double>(SMOOTHING_WIDTH, SMOOTHING_HEIGHT, matData);
    printf("----------------------------\norigin mat\n%s", mat->ToMatString().c_str());
    if(!Mat2dGaussFilter(mat, 3, 3, 1.5))
    {
        printf("Gauss Filter Failed\n");    
        delete mat;
        mat = NULL;
        return 1;
    }
    printf("----------------------------\n Mat After Gauss Filter \n%s", mat->ToMatString().c_str());
    
    // /* 通过卷积实现高斯滤波 */
    // Mat<double>* kernel;
    // printf("----------------------------\nGauss Kernel\n%s", (kernel = CreateMatGauss2dKernel(3, 3, 1.5))->ToMatString().c_str());
    // SmoothingTest(kernel);
    // delete kernel;
    // kernel = NULL;
    return 0;
}
int main()
{
    /* 0. 通过卷积 kernel 实现均值滤波和 gauss 滤波 */

    // double kernelVal[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; /* 任意加权 kernel */
    // if(NULL == (kernel = CreateMatTemplateKernel(3, 3, kernelVal)))
    // {
    //     printf("Create Mat Kernel Failed \n");
    //     return 1;
    // }

    // kernel = CreateMatMeanKernel<double>(3, 3);
    // SmoothingTest(kernel);/* 均值滤波 */
    // delete kernel;
    // /* 1. 积分图测试 */
    // CreateIntegralMapTest();
    // /* 2. 均值滤波*/
    // Mat2dMeanFilter();
    // /* 3. 中值滤波 */
    // Mat2dMeadianFilter();
    /* 4. 高斯滤波 */
    Mat2dGaussFilter();
    printf("Suess Exit\n");
    return 0;
}