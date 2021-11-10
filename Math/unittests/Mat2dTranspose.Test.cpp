/*
 * @Description: 原地矩阵转置算法测试用例
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-09 07:31:58
 * @LastEditors: like
 * @LastEditTime: 2021-11-09 07:41:17
 */
#include <Mat2dTranspose.hpp>

int main()
{
    double src[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    Mat<double>* mat = new Mat<double>(4, 3, src);
    printf("origin Mat : \n%s\n", mat->ToMatString().c_str());
    if(!SelfTranspose(mat))
    {
        printf("Transpose Failed\n");
        delete mat;
        mat = NULL;
        return 1;
    }
    printf("Transposed Mat : \n%s\n", mat->ToMatString().c_str());
    delete mat;
    mat = NULL;
    return 0;
}