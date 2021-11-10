/*
 * @Description: 直方图统计 / 直方图均衡化 测试用例
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-09 08:07:31
 * @LastEditors: like
 * @LastEditTime: 2021-11-09 08:59:36
 */
#include <Mat2dHistogram.hpp>

int main()
{
    unsigned char src[25];
    for(int i = 0; i < sizeof(src)/sizeof(unsigned char); i++)
    {
        src[i] = i;
    }
    Mat<unsigned char>* mat = new Mat<unsigned char>(5, 5, src);
    double histEqualized[256];
    EqualizeHistogram(mat, histEqualized);
    for(int i = 0; i < 256; i++)
    {
        printf("%d, %f\n", i, histEqualized[i]);
    }
    delete mat;
    mat = NULL;
    return 0;
}