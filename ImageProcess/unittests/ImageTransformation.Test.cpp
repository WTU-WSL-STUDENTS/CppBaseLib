/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-09 20:57:38
 * @LastEditors: like
 * @LastEditTime: 2021-10-10 10:43:12
 */
#include <cv/ImageTransformation.hpp>

int main()
{
    /**
     * [1, 2, 3 ] \/ [1, 1]    [1 * 1 + 2 * 0 + 3 * 0, 1 * 1 + 2 * 0 + 3 * 0]
     * [4, 5, 6 ] /\ [0, 0] -> [4 * 1 + 5 * 0 + 6 * 0, 4 * 1 + 5 * 0 + 6 * 0]
     *               [0, 0]
    */
    int left[6] = {1, 2, 3, 4, 5, 6};
    Mat<int>* matL = new Mat<int>(3, 2, left);
    int right[6] = {1, 1, 0, 0, 0, 0};
    Mat<int>* matR = new Mat<int>(2, 3, right);
    Mat<int>* res = Mult<int>(matL, matR);
    printf("%s\n", res->ToMatString().c_str());
    delete res;
    delete matR;
    delete matL;
    return 0;
}