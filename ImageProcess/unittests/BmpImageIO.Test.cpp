/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-05 07:19:05
 * @LastEditors: like
 * @LastEditTime: 2021-11-08 05:08:50
 */
// #define PRINTF_BMP_IMAGE_IO_DEBUG
#include <System.Debug.Dump.hpp>
#include <cv/BmpImageIO.hpp>

int TryLoopRead255(const char* imageSrcPath)
{
    MatBGR* mat = NULL;
    for(int i = 0; i < 255; i++)
    {
        if(!ReadImage(mat, imageSrcPath))
        {
            printf("Read Image Failed\n");
            return 1;
        }
        else
        {
            printf("Read Image Sucess\n");
        }
        delete mat;
        mat = NULL;
    }
    return 0;
}
int TryWriteTest(const char* imageDestPath)
{
    MatBGR* mat = new MatBGR(4, 4);
    mat->Init({255, 0, 0});
    if(!WriteImage(mat, imageDestPath))
    {
        printf("Write Image Failed\n");
        return 1;
    }
    else
    {
        printf("Write Image Sucess\n");
    }
    delete mat;
    mat = NULL;
    if(!ReadImage(mat, imageDestPath))
    {
        printf("Read Image Failed\n");
        return 1;
    }
    else
    {
        printf("Read Image Sucess\n");
    }
    delete mat;
    mat = NULL;
    return 0;
}
template<typename T>
int TryReadAndWrite(const char* imageSrcPath, const char* imageDestPath)
{
    Mat<T>* mat = NULL;
    if(!ReadImage(mat, imageSrcPath))
    {
        printf("Read Image Failed\n");
        return 1;
    }
    else
    {
        printf("Read Image Sucess\n");
    }
    if(!WriteImage(mat, imageDestPath))
    {
        printf("Write Image Failed\n");
        return 1;
    }
    else
    {
        printf("Write Image Sucess\n");
    }
    // ifstream fssrc;
    // fssrc.open(imageSrcPath);
    // ifstream fsdest;
    // fsdest.open(imageSrcPath);
    // int src = 0;
    // int dest= 0;
    // for(int i = 0, src = 0, dest = 0;;i++)
    // {
    //     src = fssrc.get();
    //     dest = fsdest.get();
    //     if(src != dest)
    //     {
    //         printf("Compare Read To Write , Not Same %d, %x, %x\n", i, src, dest);
    //         break;
    //     }
    //     if(EOF == src)
    //     {
    //         printf("Compare Read To Write Sucess %d\n", i);
    //         break;
    //     }
    // }
    // fssrc.close();
    // fsdest.close();
    
    delete mat;
    mat = NULL;
    return 0;
}
int main(int argc, char* argv[])
{
    SetUnhandledExceptionFilter(ExceptionFilter);
	(strrchr(argv[0], '\\'))[0] = 0; // 删除文件名，只获得路径字串//
    char inPath[255] = {0};
    char outPath[255] = {0};

    // getchar();

    /* TryReadAndWrite.300_256_32 */
    strcat(outPath, argv[0]);
    strcat(outPath, "\\BmpImageIO.Test.TryReadAndWrite.300_256_32.bmp");
    memcpy(inPath, argv[1], strlen(argv[1]));
    strcat(inPath, "\\300_256_32.bmp");
    TryReadAndWrite<MatKernel32Bit>(inPath, outPath);
    memset(inPath, 0, sizeof(inPath));
    memset(outPath, 0, sizeof(outPath));

    // /* TryReadAndWrite.3000_2000_24 */
    // strcat(outPath, argv[0]);
    // strcat(outPath, "\\BmpImageIO.Test.TryReadAndWrite.3000_2000_24.bmp");
    // memcpy(inPath, argv[1], strlen(argv[1]));
    // strcat(inPath, "\\3000_2000_24.bmp");
    // TryReadAndWrite<MatKernel24Bit>(inPath, outPath);
    // memset(inPath, 0, sizeof(inPath));
    // memset(outPath, 0, sizeof(outPath));
    
    /* TryReadAndWrite.300_200_24 */
    strcat(outPath, argv[0]);
    strcat(outPath, "\\BmpImageIO.Test.TryReadAndWrite.300_200_24.bmp");
    memcpy(inPath, argv[1], strlen(argv[1]));
    strcat(inPath, "\\300_200_24.bmp");
    TryReadAndWrite<MatKernel24Bit>(inPath, outPath);
    memset(inPath, 0, sizeof(inPath));
    memset(outPath, 0, sizeof(outPath));
    
    /* TryReadAndWrite.300_256_16 */
    strcat(outPath, argv[0]);
    strcat(outPath, "\\BmpImageIO.Test.TryReadAndWrite.300_256_16.bmp");
    memcpy(inPath, argv[1], strlen(argv[1]));
    strcat(inPath, "\\300_256_16.bmp");
    TryReadAndWrite<MatKernel16Bit>(inPath, outPath);
    memset(inPath, 0, sizeof(inPath));
    memset(outPath, 0, sizeof(outPath));

    // /* TryReadAndWrite.637_475_8 */
    // strcat(outPath, argv[0]);
    // strcat(outPath, "\\BmpImageIO.Test.TryReadAndWrite.637_475_8.bmp");
    // memcpy(inPath, argv[1], strlen(argv[1]));
    // strcat(inPath, "\\637_475_8.bmp");
    // TryReadAndWrite<MatKernel8Bit>(inPath, outPath);
    // memset(inPath, 0, sizeof(inPath));
    // memset(outPath, 0, sizeof(outPath));
    
    /* TryWriteTest_4_4_24 */
    strcat(outPath, argv[0]);
    strcat(outPath, "\\BmpImageIO.Test.TryWriteTest_4_4_24.bmp");
    TryWriteTest(outPath);
    printf("Sucess Exit");
    // getchar();
    return 0;
}