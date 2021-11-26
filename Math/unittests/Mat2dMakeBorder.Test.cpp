/*
 * @Description: ±ﬂΩÁÃÓ≥‰≤‚ ‘”√¿˝
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-25 09:11:08
 * @LastEditors: like
 * @LastEditTime: 2021-11-26 15:45:28
 */
#define MAKE_BORDER_WIDTH 3
#define MAKE_BORDER_HEIGHT 3
#include <Mat2dMakeBorder.hpp>

/**
 * @brief ????, ??????????
 * 
 * @return int 
 */
int BorderReflect_101_Test()
{
    Mat<unsigned char>* mat = new Mat<unsigned char>(MAKE_BORDER_WIDTH, MAKE_BORDER_HEIGHT);
    for(int r = 0; r < MAKE_BORDER_HEIGHT; r++)
    {
        for(int c = 0; c < MAKE_BORDER_WIDTH; c++)
        {
            mat->p[r * MAKE_BORDER_WIDTH + c] = r + c;
        }
    }
    printf("---------------\n Origin Mat\n%s", mat->ToMatString().c_str());
    Mat<unsigned char>* dest = CopyMakeBorderReflect_101(mat, 2, 2, 2, 2);
    if(NULL == dest)
    {
        delete mat;
        return 1;
    }
    printf("---------------\n Border Reflected Mat\n%s", dest->ToMatString().c_str());
    delete dest;
    delete mat;
    return 0;
}
/**
 * @brief ??????????
 * 
 * @return int 
 */
int CopyMakeBorderConstant_Test()
{
    Mat<unsigned char>* mat = new Mat<unsigned char>(MAKE_BORDER_WIDTH, MAKE_BORDER_HEIGHT);
    for(int r = 0; r < MAKE_BORDER_HEIGHT; r++)
    {
        for(int c = 0; c < MAKE_BORDER_WIDTH; c++)
        {
            mat->p[r * MAKE_BORDER_WIDTH + c] = r + c;
        }
    }
    printf("---------------\n Origin Mat\n%s", mat->ToMatString().c_str());
    unsigned char val = 1;
    Mat<unsigned char>* dest = CopyMakeBorderConstant(mat, 1, 1, 1, 1, val);
    if(NULL == dest)
    {
        printf("CopyMakeBorderConstant Failed \n");
        delete mat;
        return 1;
    }
    printf("---------------\n Border Filled Constant Value Mat\n%s", dest->ToMatString().c_str());
    delete dest;
    delete mat;
    return 0;

}
int main()
{
    // BorderReflect_101_Test();
    CopyMakeBorderConstant_Test();
    return 0;
}