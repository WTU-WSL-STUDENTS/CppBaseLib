/*
 * @Description: æÿ’Û∆Ωª¨≤‚ ‘”√¿˝
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-19 09:18:00
 * @LastEditors: like
 * @LastEditTime: 2021-11-19 16:06:41
 */

#include <Mat2dSmoothing.hpp>

int SmoothingTest()
{
    unsigned char kernelVal[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Mat<unsigned char>* kernel; 
    if(NULL == (kernel = CreateMatTemplateKernel(3, 3, kernelVal)))
    {
        printf("Create Mat Kernel Failed \n");
        return 1;
    }
#define SMOOTHING_WIDTH 9
#define SMOOTHING_HEIGHT 9
    unsigned char matData[SMOOTHING_WIDTH * SMOOTHING_HEIGHT];
    for(int r = 0; r < SMOOTHING_HEIGHT; r++)
    {
        memset(matData + (r * SMOOTHING_WIDTH), r * 2 + 1, SMOOTHING_WIDTH);
    }
    Mat<unsigned char>* mat = new Mat<unsigned char>(SMOOTHING_WIDTH, SMOOTHING_HEIGHT, matData);
    printf("%s\n", mat->ToMatString().c_str());
    if(!Mean(mat, kernel))/* æ˘÷µ¬À≤® */
    {
        printf("Mean Failed\n");    
        delete mat;
        mat = NULL;
        delete kernel;
        kernel = NULL;
        return 1;
    }
    printf("%s", mat->ToMatString().c_str());

    delete mat;
    mat = NULL;
    delete kernel;
    kernel = NULL;
    return 0;
}

int main()
{
    SmoothingTest();
    return 0;
}