/*
 * @Description: 图像边缘提取 & 锐化 测试用例
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-12-01 07:12:30
 * @LastEditors: like
 * @LastEditTime: 2021-12-02 22:00:58
 */

#include <Mat2dSharpen.hpp>
#define MAT_WIDTH 15
#define MAT_HEIGHT 12

int Mat2dCaculateDifferentiate1_Test(FilterType type)
{
    Mat<unsigned char>* mat = new Mat<unsigned char>(MAT_WIDTH, MAT_HEIGHT);
    memset(mat->p, 4, mat->length);
    for(int r = 3; r < MAT_HEIGHT - 3; r++)
    {
        memset(mat->p + r * MAT_WIDTH + 3, 99, MAT_WIDTH - 6);
    }
    Mat<unsigned char>* out = Mat2dCaculateDifferentiate1<unsigned char, unsigned char>(mat, type);
    if(NULL == out)
    {
        printf("一阶微分边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\n一阶微分-阶梯状-边界结果:\n%s", out->ToMatString().c_str());
    delete out;
    return 0;
}
int Mat2dRobertSobelPrewitt_Test()
{
    Mat<unsigned char>* mat = new Mat<unsigned char>(MAT_WIDTH, MAT_HEIGHT);
    memset(mat->p, 4, mat->length);
    for(int r = 3; r < MAT_HEIGHT - 3; r++)
    {
        memset(mat->p + r * MAT_WIDTH + 3, 99, MAT_WIDTH - 6);
    }
    Mat<unsigned char>* out = Mat2dRobert<unsigned char, unsigned char>(mat);
    if(NULL == out)
    {
        printf("Robert 边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\nRobert-阶梯状-边界结果:\n%s", out->ToMatString().c_str());
    delete out;

    out = Mat2dSobel<unsigned char, unsigned char>(mat);
    if(NULL == out)
    {
        printf("Sobel 边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\nSobel-阶梯状-边界结果:\n%s", out->ToMatString().c_str());
    
    delete out;
    out = Mat2dPrewitt<unsigned char, unsigned char>(mat);
    if(NULL == out)
    {
        printf("Prewitt 边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\nPrewitt-阶梯状-边界结果:\n%s", out->ToMatString().c_str());

    delete out;
    out = Mat2dKrisch<unsigned char, unsigned char>(mat);
    if(NULL == out)
    {
        printf("Krisch 边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\nKrisch-阶梯状-边界结果:\n%s", out->ToMatString().c_str());

    delete out;
    delete mat;
    return 0;
}

int Mat2dLaplcian()
{
    /*
        -------------------------
        laplcian4-阶梯状-边界结果:
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 0 161 161 161 161 161 161 161 161 161 0 0 0
        0 0 161 190 95 95 95 95 95 95 95 190 161 0 0   
        0 0 161 95 0 0 0 0 0 0 0 95 161 0 0
        0 0 161 95 0 0 0 0 0 0 0 95 161 0 0
        0 0 161 95 0 0 0 0 0 0 0 95 161 0 0
        0 0 161 95 0 0 0 0 0 0 0 95 161 0 0
        0 0 161 190 95 95 95 95 95 95 95 190 161 0 0   
        0 0 0 161 161 161 161 161 161 161 161 161 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        -------------------------
        laplcian4WithOriginMat-阶梯状-边界结果:        
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 4 4 4 4 4 4 4 4 4 4 4 4 4 0
        0 4 4 91 91 91 91 91 91 91 91 91 4 4 0
        0 4 91 33 194 194 194 194 194 194 194 33 91 4 0
        0 4 91 194 99 99 99 99 99 99 99 194 91 4 0
        0 4 91 194 99 99 99 99 99 99 99 194 91 4 0
        0 4 91 194 99 99 99 99 99 99 99 194 91 4 0
        0 4 91 194 99 99 99 99 99 99 99 194 91 4 0
        0 4 91 33 194 194 194 194 194 194 194 33 91 4 0
        0 4 4 91 91 91 91 91 91 91 91 91 4 4 0
        0 4 4 4 4 4 4 4 4 4 4 4 4 4 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        -------------------------
        laplcian8-阶梯状-边界结果:
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 161 66 227 227 227 227 227 227 227 66 161 0 0
        0 0 66 219 29 29 29 29 29 29 29 219 66 0 0
        0 0 227 29 0 0 0 0 0 0 0 29 227 0 0
        0 0 227 29 0 0 0 0 0 0 0 29 227 0 0
        0 0 227 29 0 0 0 0 0 0 0 29 227 0 0
        0 0 227 29 0 0 0 0 0 0 0 29 227 0 0
        0 0 66 219 29 29 29 29 29 29 29 219 66 0 0
        0 0 161 66 227 227 227 227 227 227 227 66 161 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        -------------------------
        laplcian8WithOriginMat-阶梯状-边界结果:
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 4 4 4 4 4 4 4 4 4 4 4 4 4 0
        0 4 91 186 25 25 25 25 25 25 25 186 91 4 0
        0 4 186 62 128 128 128 128 128 128 128 62 186 4 0
        0 4 25 128 99 99 99 99 99 99 99 128 25 4 0
        0 4 25 128 99 99 99 99 99 99 99 128 25 4 0
        0 4 25 128 99 99 99 99 99 99 99 128 25 4 0
        0 4 25 128 99 99 99 99 99 99 99 128 25 4 0
        0 4 186 62 128 128 128 128 128 128 128 62 186 4 0
        0 4 91 186 25 25 25 25 25 25 25 186 91 4 0
        0 4 4 4 4 4 4 4 4 4 4 4 4 4 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        -------------------------
        Mat2dLaplcianOfGauss-阶梯状-边界结果:
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 182 58 182 240 240 240 240 240 182 58 182 0 0
        0 0 58 42 166 240 240 240 240 240 166 42 58 0 0
        0 0 182 166 34 240 240 240 240 240 34 166 182 0 0
        0 0 240 240 240 0 0 0 0 0 240 240 240 0 0
        0 0 240 240 240 0 0 0 0 0 240 240 240 0 0
        0 0 182 166 34 240 240 240 240 240 34 166 182 0 0
        0 0 58 42 166 240 240 240 240 240 166 42 58 0 0
        0 0 182 58 182 240 240 240 240 240 182 58 182 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        Sucess Exit
    */
    Mat<unsigned char>* mat = new Mat<unsigned char>(MAT_WIDTH, MAT_HEIGHT);
    memset(mat->p, 4, mat->length);
    for(int r = 3; r < MAT_HEIGHT - 3; r++)
    {
        memset(mat->p + r * MAT_WIDTH + 3, 99, MAT_WIDTH - 6);
    }
    Mat<unsigned char>* out = Mat2dLaplcian<unsigned char, unsigned char>(mat, laplcian4);
    if(NULL == out)
    {
        printf("laplcian4 边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\nlaplcian4-阶梯状-边界结果:\n%s", out->ToMatString().c_str());

    delete out; 
    out = Mat2dLaplcian<unsigned char, unsigned char>(mat, laplcian4WithOriginMat);
    if(NULL == out)
    {
        printf("laplcian4WithOriginMat 边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\nlaplcian4WithOriginMat-阶梯状-边界结果:\n%s", out->ToMatString().c_str());

    delete out; 
    out = Mat2dLaplcian<unsigned char, unsigned char>(mat, laplcian8);
    if(NULL == out)
    {
        printf("laplcian8 边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\nlaplcian8-阶梯状-边界结果:\n%s", out->ToMatString().c_str());

    delete out; 
    out = Mat2dLaplcian<unsigned char, unsigned char>(mat, laplcian8WithOriginMat);
    if(NULL == out)
    {
        printf("laplcian8WithOriginMat 边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\nlaplcian8WithOriginMat-阶梯状-边界结果:\n%s", out->ToMatString().c_str());
    
    delete out; 
    out = Mat2dLaplcianOfGauss<unsigned char, unsigned char>(mat);
    if(NULL == out)
    {
        printf("Mat2dLaplcianOfGauss 边界提取失败\n");
        delete mat;
        return 1;
    }
    printf("-------------------------\nMat2dLaplcianOfGauss-阶梯状-边界结果:\n%s", out->ToMatString().c_str());

    delete out;
    delete mat;
    return 0;
}

int main()
{
    // printf("-------------------------\n水平方向一阶微分\n");
    // Mat2dCaculateDifferentiate1_Test(FilterType::x);
    // printf("-------------------------\n竖直方向一阶微分\n");
    // Mat2dCaculateDifferentiate1_Test(FilterType::y);
    // printf("-------------------------\n双向一阶微分之距离\n");
    // Mat2dCaculateDifferentiate1_Test(FilterType::sum_sqrt);
    // printf("-------------------------\n双向一阶微分之取绝对值\n");
    // Mat2dCaculateDifferentiate1_Test(FilterType::sum_abs);
    // Mat2dRobertSobelPrewitt_Test();
    Mat2dLaplcian();
    printf("Sucess Exit\n");
    return 0;
}