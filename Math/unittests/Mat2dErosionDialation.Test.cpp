/*
 * @Description: ∏Ø ¥≈Ú’Õ≤‚ ‘”√¿˝
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-12-07 07:57:53
 * @LastEditors: like
 * @LastEditTime: 2021-12-10 08:30:32
 */

#define MAT_WIDTH 10
#define MAT_HEIGHT 10
#include <Mat2dErosionDialation.hpp>

int Erosion_Test()
{
    unsigned char matData[MAT_WIDTH * MAT_HEIGHT] = {0};
    for(int r = 0; r < MAT_HEIGHT; r++)
    {
        memset(matData + (r * MAT_WIDTH), r * 2 + 1, MAT_WIDTH);
    }
    Mat<unsigned char>* mat = new Mat<unsigned char>(MAT_WIDTH, MAT_HEIGHT, matData);
    Mat<unsigned char>* out = Erosion<unsigned char>(mat, 3, 3);
    printf("Origion Mat\n--------------------\n%s", mat->ToMatString().c_str());
    if(NULL == out)
    {
        printf("Erosion Failed\n");
        return 1;
    }
    printf("Erosion Sucess\n");
    printf("Erosion\n---------------------\n%s", out->ToMatString().c_str());
    
    delete out;
    out = Dialation<unsigned char>(mat, 3, 3);
    if(NULL == out)
    {
        printf("Dialation Failed\n");
        return 1;
    }
    printf("Dialation Sucess\n");
    printf("Dialation\n---------------------\n%s", out->ToMatString().c_str());
    delete out;
    delete mat;
    return 0;
}

int main()
{
    Erosion_Test();
    printf("Sucess Exit\n");
    system("pause");
    return 0;
}