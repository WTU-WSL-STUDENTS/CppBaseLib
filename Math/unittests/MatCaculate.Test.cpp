/*
 * @Description: ∂˛Œ¨æÿ’Û‘ÀÀ„µƒ≤‚ ‘
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-09 05:32:54
 * @LastEditors: like
 * @LastEditTime: 2021-11-09 09:00:00
 */

#include <MatCaculate.hpp>

int main()
{
    double x = 100;
    double y = 50;
    double x1, y1;
    printf("Befor Transformat : %f, %f\n", x, y);
    Mat<double>* matTransformat = GenMat2dIdentity<double>();
    if(NULL == matTransformat)
    {
        printf("Create Mat2d Identity Failed\n");
        delete matTransformat;
        matTransformat = NULL;
        return 1;
    }
    else
    {
        printf("Matrix is %d X %d\n", matTransformat->w, matTransformat->h);
        printf("%s\n", matTransformat->ToMatString().c_str());
    }
    Mat2dRotate<double>(matTransformat, DegreeToRadian(0));
    printf("%s\n", matTransformat->ToMatString().c_str());
    Mat2dTranslate<double>(matTransformat, 100, 50);
    printf("%s\n", matTransformat->ToMatString().c_str());
    Mat2dMult<double>(x, y, matTransformat, x1, y1);
    printf("After Transformat : %f, %f\n", x1, y1);

    Mat<double>* matScala = GenMat2dIdentity<double>();
    Mat2dScala<double>(matScala, 0.5, 0.5);
    printf("%s\n", matScala->ToMatString().c_str());
    Mat<double>* matTranslateWithScala = Mult(matTransformat, matScala);
    printf("%s\n", matTranslateWithScala->ToMatString().c_str());
    Mat2dMult<double>(x, y, matTranslateWithScala, x1, y1);
    printf("After Translate With Scala : %f, %f\n", x1, y1);
    
    delete matTranslateWithScala;
    matTranslateWithScala = NULL;
    delete matScala;
    matScala = NULL;
    delete matTransformat;
    matTransformat = NULL;
    return 0;
}