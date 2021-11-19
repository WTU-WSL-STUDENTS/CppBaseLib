/*
 * @Description: 阈值分割算法测试用例 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-17 06:27:50
 * @LastEditors: like
 * @LastEditTime: 2021-11-18 06:46:43
 */

// #define MAT2D_THRESHOLD_DEBUG
// #define MAT2D_RLES_DEBUG

#include <System.Debug.HardwareResources.hpp>
#include <System.Debug.Dump.hpp>
#include <Mat2dThreshold.hpp>
#include <Mat2dRLE.hpp>

#define ERROR_RET(str) \
    printf(str);\
    delete mat; \
    mat = NULL; \
    return 1;   

typedef unsigned char CV_BYTE;

int AutoThresholdLowPass(int w, int h, const CV_BYTE* src)
{
    // PrintHardwareUsage();
    Mat<CV_BYTE>* mat = new Mat<CV_BYTE>(w, h, src);
    int bestThresholdVal = 0;
    printf("mat width, height %d, %d\n", mat->w, mat->h);
    if(!AutoThreshold(mat, bestThresholdVal))
    {
        ERROR_RET("AutoThreshold Failed\n");
    }
    printf("AutoThreshold's Best Value : %d\n", bestThresholdVal);
    if(!ThresholdLowPass(mat, bestThresholdVal))
    {
        ERROR_RET("ThresholdLowPass Failed\n");
    }
    // printf("%s", mat->ToMatString().c_str());
    delete mat;
    mat = NULL;
    // PrintHardwareUsage();
    return 0;
}
/* 矩阵转二值游程编码 */
int Mat2dToRLE(int w, int h, const CV_BYTE* src)
{
    printf("Begin Mat2d To RLE Test\n");
    Mat<CV_BYTE>* mat = new Mat<CV_BYTE>(w, h, src);
    RLES* rles = TransToRLE(mat);
    if(!rles)
    {
        ERROR_RET("Trans To RLE Failed\n");
    }
    printf("mat width : %d , mat height : %d , rle count : %d\nLabel, Length\n", mat->w, mat->h, rles->Count());
    BinaryRle rle;
    for(size_t i = 0; i < rles->Count(); i++)
    {
        rle = (*rles)[i];
        printf("%d,%8lld\n", GET_BINARY_RLE_LABEL(rle), GET_BINARY_RLE_LENGTH(rle));
    }
    delete mat;
    mat = NULL;
    printf("End Mat2d To RLE Test\n");
    return 0;
}
int main()
{
    SetUnhandledExceptionFilter(ExceptionFilter);
#define SRC_WDITH 40
#define SRC_HEIGHT 30
    CV_BYTE src[SRC_WDITH * SRC_HEIGHT];
    for(size_t i = 0; i < sizeof(src); i++)/* 假设数据整体趋势为 0 - 3 暗画面 */
    {
        src[i] = i % 4;
    }
    for(CV_BYTE i = 0; i < 5; i++)/* 手动设置少量 亮点 噪点 */
    {
        src[i] = ~i;
    }
    AutoThresholdLowPass(SRC_WDITH, SRC_HEIGHT, src);

    memset(src, 0, sizeof(src));
    for(int i = 0; i < SRC_HEIGHT; i++) /* 水平方向 1 : 4 比例的 0 - 1 分布 */
    {
        memset(src + (SRC_WDITH * i), 1, 10);
    }
    Mat2dToRLE(SRC_WDITH, SRC_HEIGHT, src);
    printf("Sucess Exit\n");
    return 0;
}