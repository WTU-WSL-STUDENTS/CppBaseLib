/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-04 14:34:19
 * @LastEditors: like
 * @LastEditTime: 2021-10-04 16:09:02
 */
#include <stdio.h>
#include <ImageMat.hpp>
int main()
{
    int width   = 3;
    int height  = 2;
    MatGray* gray = new MatGray(width, height);
    printf("gray val : \n%s\n", gray->ToMatString().c_str());
    MatBGR* bgr = new MatBGR(width, height);
    printf("rgb val : \n%s\n", bgr->ToMatString().c_str());
    bgr->Init({0xff, 0, 0xff});
    printf("inited rgb val : \n%s\n", bgr->ToMatString().c_str());
    delete gray;
    gray = NULL;
    delete bgr;
    bgr = NULL;
    return 0;
}