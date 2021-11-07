/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-13 15:48:39
 * @LastEditors: like
 * @LastEditTime: 2021-10-14 17:28:42
 */
#include <Rle.hpp>

Mat<MatKernel8Bit>* DecomposeBinaryMat(Mat<MatKernel1Bit>* bin)
{
    Mat<MatKernel8Bit>* mat = new Mat<MatKernel8Bit>(bin->w, bin->h);
    MatKernel8Bit* p = mat->p;
    MatKernel1Bit* b = bin->p;
    int bitIndex;
    for(size_t i = 0; i < bin->length; i++)
    {
        bitIndex = i % 32; 
        if(0 == bitIndex)
        {
            b++;
            *p++ = b->GetBit(0);
            continue;
        }
        *p++ = b->GetBit(bitIndex);
    }
    return mat;
}
int main()
{
    Mat<MatKernel8Bit>* mat;
    Mat<MatKernel1Bit>* bin = new Mat<MatKernel1Bit>(31, 3);
    bin->Init({0xaaaaaaaa});//({0xffffffff});
    mat = DecomposeBinaryMat(bin);
    MatKernel8Bit* p = mat->p;
    for(size_t i = 0; i < mat->length; i++, p++)
    {
        printf("%d-%d\n", i, *p);
    }
    printf("%d, %d\n", mat->w, mat->h);
    // mat = new Mat<MatKernel8Bit>(10, 10);
    // mat->Init(0xf0);
    // size_t rleLen;
    // BinaryRle* rle = RleBitCompress(mat, rleLen);
    // for(size_t i = 0; i < rleLen; i++)
    // {
    //     printf("%u, %d\n", (*rle) >> (8 * sizeof(BinaryRle) - 1), 0x7fffffff & *rle);
    // }
    // free(rle);
    delete bin;
    delete mat;
    printf("Sucess Exit");
    return 0;
}