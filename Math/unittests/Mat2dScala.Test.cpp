/*
 * @Description: 矩阵元素拉伸变换 测试用例
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-18 09:23:47
 * @LastEditors: like
 * @LastEditTime: 2021-11-19 06:44:43
 */

#include <Mat2dScala.hpp>

inline void Linear1(unsigned char& val)
{
    val = 0;
}
inline void Linear2(unsigned char& val)
{
    // val *= 2;
}
inline void Linear3(unsigned char& val)
{
    val -= 60;
}

/* 自定义分段线性函数， 测试用例 */
int PiecewiseLinearScalaTest()
{
#define PLS_WIDTH 10
#define PLS_HEIGHT 120
    unsigned char buffer[PLS_WIDTH * PLS_HEIGHT] = {0};
    for(int r = 0; r < PLS_HEIGHT; r++)/* 竖直方向上 0 - 100 渐变的值*/
    {
        memset(buffer + (r * PLS_WIDTH), r, PLS_WIDTH);
    }
    Mat<unsigned char>* mat = new Mat<unsigned char>(PLS_WIDTH, PLS_HEIGHT, buffer);
    PiecewiseScalaOperate::PiecewiseLinearMap map = 
    {
        {30,  Linear1}, /* [0, 30]   ->  0      */
        {60,  Linear2}, /* (30, 60]  ->  selef  */
        {0xff, Linear3} /* (60, 255] ->  - 60   */
    };
    PiecewiseScalaOperate::PiecewiseLinearMap::iterator it = map.begin();
    PiecewiseScalaOperate scala(mat, map);
    if(!scala.DoFunc())
    {
        printf("Do Piecewise Linear Scala Failed\n");
        delete mat;
        mat = NULL;
        return 1;
    }
    printf("%s", mat->ToMatString().c_str());
    delete mat;
    mat = NULL;
    return 0;
}

int main()
{
    PiecewiseLinearScalaTest();
    printf("Mat2d Scala Test Sucess Exit\n");
    return 0;
}