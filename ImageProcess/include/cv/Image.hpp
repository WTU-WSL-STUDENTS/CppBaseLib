/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-07 11:01:34
 * @LastEditors: like
 * @LastEditTime: 2021-10-07 11:01:34
 */
/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-04 11:48:36
 * @LastEditors: like
 * @LastEditTime: 2021-10-07 10:35:06
 */
#ifndef BITMAP_STRUCTURE_HPP
#define BITMAP_STRUCTURE_HPP

#include <GdiImageIO.hpp>
#include <BmpImageIO.hpp>

template<typename ImageKernel>
class Image
{
public:
    PBitmapFileHeader pbf;
    PBitmapInfo pbi;
    Mat<ImageKernel>* mat;
    Image() : pbf(NULL), pbi(NULL), mat(NULL){}
    bool ReadImage(const char* filepath)
    {
        if(std::is_same<ImageKernel, MatKernel8Bit>::value)
        {
            return ReadImage(pbi, mat, filepath);
        }
        else if(std::is_same<ImageKernel, MatKernel24Bit>::value || (std::is_same<ImageKernel, MatKernel32Bit>::value)
        {
            return ReadImage(mat, filepath);
        }
        else
        {

        }
    }
    template<typename ImageKernelType/**/> 
    ImageKernelType* PraseTo(){ throw "Not Override";}
};
#endif