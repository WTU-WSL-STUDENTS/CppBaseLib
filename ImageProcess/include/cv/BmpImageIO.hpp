/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-04 20:53:44
 * @LastEditors: like
 * @LastEditTime: 2021-10-12 15:35:50
 */
#ifndef BMP_IMAGE_IO_HPP
#define BMP_IMAGE_IO_HPP

#define _HAS_STD_BYTE 0
#define _CRT_SECURE_NO_WARNINGS
#include <ImageMat.hpp>
#include <string>
#include <iostream>  
#include <fstream> 
#include <sstream>
/* https://zhuanlan.zhihu.com/p/111635571 */
#define SET_PALETTE_16_565(pbi) \
    pbi->palette[2] = {0,   0xf8,   0, 0}; \
    pbi->palette[1] = {0xe0,0x07,   0, 0}; \
    pbi->palette[0] = {0x1f,0,      0, 0}; \
    pbi->palette[3] = {0,   0,      0, 0}; \

#define SET_PALETTE_255(pbi) \
    pbi->palette[  0] = {  0,   0,   0, 0}; \
    pbi->palette[  1] = {  0,   0, 128, 0}; \
    pbi->palette[  2] = {  0, 128,   0, 0}; \
    pbi->palette[  3] = {  0, 128, 128, 0}; \
    pbi->palette[  4] = {128,   0,   0, 0}; \
    pbi->palette[  5] = {128,   0, 128, 0}; \
    pbi->palette[  6] = {128, 128,   0, 0}; \
    pbi->palette[  7] = {192, 192, 192, 0}; \
    pbi->palette[  8] = {192, 220, 192, 0}; \
    pbi->palette[  9] = {240, 202, 166, 0}; \
    pbi->palette[ 10] = {  0,  32,  64, 0}; \
    pbi->palette[ 11] = {  0,  32,  96, 0}; \
    pbi->palette[ 12] = {  0,  32, 128, 0}; \
    pbi->palette[ 13] = {  0,  32, 160, 0}; \
    pbi->palette[ 14] = {  0,  32, 192, 0}; \
    pbi->palette[ 15] = {  0,  32, 224, 0}; \
    pbi->palette[ 16] = {  0,  64,   0, 0}; \
    pbi->palette[ 17] = {  0,  64,  32, 0}; \
    pbi->palette[ 18] = {  0,  64,  64, 0}; \
    pbi->palette[ 19] = {  0,  64,  96, 0}; \
    pbi->palette[ 20] = {  0,  64, 128, 0}; \
    pbi->palette[ 21] = {  0,  64, 160, 0}; \
    pbi->palette[ 22] = {  0,  64, 192, 0}; \
    pbi->palette[ 23] = {  0,  64, 224, 0}; \
    pbi->palette[ 24] = {  0,  96,   0, 0}; \
    pbi->palette[ 25] = {  0,  96,  32, 0}; \
    pbi->palette[ 26] = {  0,  96,  64, 0}; \
    pbi->palette[ 27] = {  0,  96,  96, 0}; \
    pbi->palette[ 28] = {  0,  96, 128, 0}; \
    pbi->palette[ 29] = {  0,  96, 160, 0}; \
    pbi->palette[ 30] = {  0,  96, 192, 0}; \
    pbi->palette[ 31] = {  0,  96, 224, 0}; \
    pbi->palette[ 32] = {  0, 128,   0, 0}; \
    pbi->palette[ 33] = {  0, 128,  32, 0}; \
    pbi->palette[ 34] = {  0, 128,  64, 0}; \
    pbi->palette[ 35] = {  0, 128,  96, 0}; \
    pbi->palette[ 36] = {  0, 128, 128, 0}; \
    pbi->palette[ 37] = {  0, 128, 160, 0}; \
    pbi->palette[ 38] = {  0, 128, 192, 0}; \
    pbi->palette[ 39] = {  0, 128, 224, 0}; \
    pbi->palette[ 40] = {  0, 160,   0, 0}; \
    pbi->palette[ 41] = {  0, 160,  32, 0}; \
    pbi->palette[ 42] = {  0, 160,  64, 0}; \
    pbi->palette[ 43] = {  0, 160,  96, 0}; \
    pbi->palette[ 44] = {  0, 160, 128, 0}; \
    pbi->palette[ 45] = {  0, 160, 160, 0}; \
    pbi->palette[ 46] = {  0, 160, 192, 0}; \
    pbi->palette[ 47] = {  0, 160, 224, 0}; \
    pbi->palette[ 48] = {  0, 192,   0, 0}; \
    pbi->palette[ 49] = {  0, 192,  32, 0}; \
    pbi->palette[ 50] = {  0, 192,  64, 0}; \
    pbi->palette[ 51] = {  0, 192,  96, 0}; \
    pbi->palette[ 52] = {  0, 192, 128, 0}; \
    pbi->palette[ 53] = {  0, 192, 160, 0}; \
    pbi->palette[ 54] = {  0, 192, 192, 0}; \
    pbi->palette[ 55] = {  0, 192, 224, 0}; \
    pbi->palette[ 56] = {  0, 224,   0, 0}; \
    pbi->palette[ 57] = {  0, 224,  32, 0}; \
    pbi->palette[ 58] = {  0, 224,  64, 0}; \
    pbi->palette[ 59] = {  0, 224,  96, 0}; \
    pbi->palette[ 60] = {  0, 224, 128, 0}; \
    pbi->palette[ 61] = {  0, 224, 160, 0}; \
    pbi->palette[ 62] = {  0, 224, 192, 0}; \
    pbi->palette[ 63] = {  0, 224, 224, 0}; \
    pbi->palette[ 64] = { 64,   0,   0, 0}; \
    pbi->palette[ 65] = { 64,   0,  32, 0}; \
    pbi->palette[ 66] = { 64,   0,  64, 0}; \
    pbi->palette[ 67] = { 64,   0,  96, 0}; \
    pbi->palette[ 68] = { 64,   0, 128, 0}; \
    pbi->palette[ 69] = { 64,   0, 160, 0}; \
    pbi->palette[ 70] = { 64,   0, 192, 0}; \
    pbi->palette[ 71] = { 64,   0, 224, 0}; \
    pbi->palette[ 72] = { 64,  32,   0, 0}; \
    pbi->palette[ 73] = { 64,  32,  32, 0}; \
    pbi->palette[ 74] = { 64,  32,  64, 0}; \
    pbi->palette[ 75] = { 64,  32,  96, 0}; \
    pbi->palette[ 76] = { 64,  32, 128, 0}; \
    pbi->palette[ 77] = { 64,  32, 160, 0}; \
    pbi->palette[ 78] = { 64,  32, 192, 0}; \
    pbi->palette[ 79] = { 64,  32, 224, 0}; \
    pbi->palette[ 80] = { 64,  64,   0, 0}; \
    pbi->palette[ 81] = { 64,  64,  32, 0}; \
    pbi->palette[ 82] = { 64,  64,  64, 0}; \
    pbi->palette[ 83] = { 64,  64,  96, 0}; \
    pbi->palette[ 84] = { 64,  64, 128, 0}; \
    pbi->palette[ 85] = { 64,  64, 160, 0}; \
    pbi->palette[ 86] = { 64,  64, 192, 0}; \
    pbi->palette[ 87] = { 64,  64, 224, 0}; \
    pbi->palette[ 88] = { 64,  96,   0, 0}; \
    pbi->palette[ 89] = { 64,  96,  32, 0}; \
    pbi->palette[ 90] = { 64,  96,  64, 0}; \
    pbi->palette[ 91] = { 64,  96,  96, 0}; \
    pbi->palette[ 92] = { 64,  96, 128, 0}; \
    pbi->palette[ 93] = { 64,  96, 160, 0}; \
    pbi->palette[ 94] = { 64,  96, 192, 0}; \
    pbi->palette[ 95] = { 64,  96, 224, 0}; \
    pbi->palette[ 96] = { 64, 128,   0, 0}; \
    pbi->palette[ 97] = { 64, 128,  32, 0}; \
    pbi->palette[ 98] = { 64, 128,  64, 0}; \
    pbi->palette[ 99] = { 64, 128,  96, 0}; \
    pbi->palette[100] = { 64, 128, 128, 0}; \
    pbi->palette[101] = { 64, 128, 160, 0}; \
    pbi->palette[102] = { 64, 128, 192, 0}; \
    pbi->palette[103] = { 64, 128, 224, 0}; \
    pbi->palette[104] = { 64, 160,   0, 0}; \
    pbi->palette[105] = { 64, 160,  32, 0}; \
    pbi->palette[106] = { 64, 160,  64, 0}; \
    pbi->palette[107] = { 64, 160,  96, 0}; \
    pbi->palette[108] = { 64, 160, 128, 0}; \
    pbi->palette[109] = { 64, 160, 160, 0}; \
    pbi->palette[110] = { 64, 160, 192, 0}; \
    pbi->palette[111] = { 64, 160, 224, 0}; \
    pbi->palette[112] = { 64, 192,   0, 0}; \
    pbi->palette[113] = { 64, 192,  32, 0}; \
    pbi->palette[114] = { 64, 192,  64, 0}; \
    pbi->palette[115] = { 64, 192,  96, 0}; \
    pbi->palette[116] = { 64, 192, 128, 0}; \
    pbi->palette[117] = { 64, 192, 160, 0}; \
    pbi->palette[118] = { 64, 192, 192, 0}; \
    pbi->palette[119] = { 64, 192, 224, 0}; \
    pbi->palette[120] = { 64, 224,   0, 0}; \
    pbi->palette[121] = { 64, 224,  32, 0}; \
    pbi->palette[122] = { 64, 224,  64, 0}; \
    pbi->palette[123] = { 64, 224,  96, 0}; \
    pbi->palette[124] = { 64, 224, 128, 0}; \
    pbi->palette[125] = { 64, 224, 160, 0}; \
    pbi->palette[126] = { 64, 224, 192, 0}; \
    pbi->palette[127] = { 64, 224, 224, 0}; \
    pbi->palette[128] = {128,   0,   0, 0}; \
    pbi->palette[129] = {128,   0,  32, 0}; \
    pbi->palette[130] = {128,   0,  64, 0}; \
    pbi->palette[131] = {128,   0,  96, 0}; \
    pbi->palette[132] = {128,   0, 128, 0}; \
    pbi->palette[133] = {128,   0, 160, 0}; \
    pbi->palette[134] = {128,   0, 192, 0}; \
    pbi->palette[135] = {128,   0, 224, 0}; \
    pbi->palette[136] = {128,  32,   0, 0}; \
    pbi->palette[137] = {128,  32,  32, 0}; \
    pbi->palette[138] = {128,  32,  64, 0}; \
    pbi->palette[139] = {128,  32,  96, 0}; \
    pbi->palette[140] = {128,  32, 128, 0}; \
    pbi->palette[141] = {128,  32, 160, 0}; \
    pbi->palette[142] = {128,  32, 192, 0}; \
    pbi->palette[143] = {128,  32, 224, 0}; \
    pbi->palette[144] = {128,  64,   0, 0}; \
    pbi->palette[145] = {128,  64,  32, 0}; \
    pbi->palette[146] = {128,  64,  64, 0}; \
    pbi->palette[147] = {128,  64,  96, 0}; \
    pbi->palette[148] = {128,  64, 128, 0}; \
    pbi->palette[149] = {128,  64, 160, 0}; \
    pbi->palette[150] = {128,  64, 192, 0}; \
    pbi->palette[151] = {128,  64, 224, 0}; \
    pbi->palette[152] = {128,  96,   0, 0}; \
    pbi->palette[153] = {128,  96,  32, 0}; \
    pbi->palette[154] = {128,  96,  64, 0}; \
    pbi->palette[155] = {128,  96,  96, 0}; \
    pbi->palette[156] = {128,  96, 128, 0}; \
    pbi->palette[157] = {128,  96, 160, 0}; \
    pbi->palette[158] = {128,  96, 192, 0}; \
    pbi->palette[159] = {128,  96, 224, 0}; \
    pbi->palette[160] = {128, 128,   0, 0}; \
    pbi->palette[161] = {128, 128,  32, 0}; \
    pbi->palette[162] = {128, 128,  64, 0}; \
    pbi->palette[163] = {128, 128,  96, 0}; \
    pbi->palette[164] = {128, 128, 128, 0}; \
    pbi->palette[165] = {128, 128, 160, 0}; \
    pbi->palette[166] = {128, 128, 192, 0}; \
    pbi->palette[167] = {128, 128, 224, 0}; \
    pbi->palette[168] = {128, 160,   0, 0}; \
    pbi->palette[169] = {128, 160,  32, 0}; \
    pbi->palette[170] = {128, 160,  64, 0}; \
    pbi->palette[171] = {128, 160,  96, 0}; \
    pbi->palette[172] = {128, 160, 128, 0}; \
    pbi->palette[173] = {128, 160, 160, 0}; \
    pbi->palette[174] = {128, 160, 192, 0}; \
    pbi->palette[175] = {128, 160, 224, 0}; \
    pbi->palette[176] = {128, 192,   0, 0}; \
    pbi->palette[177] = {128, 192,  32, 0}; \
    pbi->palette[178] = {128, 192,  64, 0}; \
    pbi->palette[179] = {128, 192,  96, 0}; \
    pbi->palette[180] = {128, 192, 128, 0}; \
    pbi->palette[181] = {128, 192, 160, 0}; \
    pbi->palette[182] = {128, 192, 192, 0}; \
    pbi->palette[183] = {128, 192, 224, 0}; \
    pbi->palette[184] = {128, 224,   0, 0}; \
    pbi->palette[185] = {128, 224,  32, 0}; \
    pbi->palette[186] = {128, 224,  64, 0}; \
    pbi->palette[187] = {128, 224,  96, 0}; \
    pbi->palette[188] = {128, 224, 128, 0}; \
    pbi->palette[189] = {128, 224, 160, 0}; \
    pbi->palette[190] = {128, 224, 192, 0}; \
    pbi->palette[191] = {128, 224, 224, 0}; \
    pbi->palette[192] = {192,   0,   0, 0}; \
    pbi->palette[193] = {192,   0,  32, 0}; \
    pbi->palette[194] = {192,   0,  64, 0}; \
    pbi->palette[195] = {192,   0,  96, 0}; \
    pbi->palette[196] = {192,   0, 128, 0}; \
    pbi->palette[197] = {192,   0, 160, 0}; \
    pbi->palette[198] = {192,   0, 192, 0}; \
    pbi->palette[199] = {192,   0, 224, 0}; \
    pbi->palette[200] = {192,  32,   0, 0}; \
    pbi->palette[201] = {192,  32,  32, 0}; \
    pbi->palette[202] = {192,  32,  64, 0}; \
    pbi->palette[203] = {192,  32,  96, 0}; \
    pbi->palette[204] = {192,  32, 128, 0}; \
    pbi->palette[205] = {192,  32, 160, 0}; \
    pbi->palette[206] = {192,  32, 192, 0}; \
    pbi->palette[207] = {192,  32, 224, 0}; \
    pbi->palette[208] = {192,  64,   0, 0}; \
    pbi->palette[209] = {192,  64,  32, 0}; \
    pbi->palette[210] = {192,  64,  64, 0}; \
    pbi->palette[211] = {192,  64,  96, 0}; \
    pbi->palette[212] = {192,  64, 128, 0}; \
    pbi->palette[213] = {192,  64, 160, 0}; \
    pbi->palette[214] = {192,  64, 192, 0}; \
    pbi->palette[215] = {192,  64, 224, 0}; \
    pbi->palette[216] = {192,  96,   0, 0}; \
    pbi->palette[217] = {192,  96,  32, 0}; \
    pbi->palette[218] = {192,  96,  64, 0}; \
    pbi->palette[219] = {192,  96,  96, 0}; \
    pbi->palette[220] = {192,  96, 128, 0}; \
    pbi->palette[221] = {192,  96, 160, 0}; \
    pbi->palette[222] = {192,  96, 192, 0}; \
    pbi->palette[223] = {192,  96, 224, 0}; \
    pbi->palette[224] = {192, 128,   0, 0}; \
    pbi->palette[225] = {192, 128,  32, 0}; \
    pbi->palette[226] = {192, 128,  64, 0}; \
    pbi->palette[227] = {192, 128,  96, 0}; \
    pbi->palette[228] = {192, 128, 128, 0}; \
    pbi->palette[229] = {192, 128, 160, 0}; \
    pbi->palette[230] = {192, 128, 192, 0}; \
    pbi->palette[231] = {192, 128, 224, 0}; \
    pbi->palette[232] = {192, 160,   0, 0}; \
    pbi->palette[233] = {192, 160,  32, 0}; \
    pbi->palette[234] = {192, 160,  64, 0}; \
    pbi->palette[235] = {192, 160,  96, 0}; \
    pbi->palette[236] = {192, 160, 128, 0}; \
    pbi->palette[237] = {192, 160, 160, 0}; \
    pbi->palette[238] = {192, 160, 192, 0}; \
    pbi->palette[239] = {192, 160, 224, 0}; \
    pbi->palette[240] = {192, 192,   0, 0}; \
    pbi->palette[241] = {192, 192,  32, 0}; \
    pbi->palette[242] = {192, 192,  64, 0}; \
    pbi->palette[243] = {192, 192,  96, 0}; \
    pbi->palette[244] = {192, 192, 128, 0}; \
    pbi->palette[245] = {192, 192, 160, 0}; \
    pbi->palette[246] = {240, 251, 255, 0}; \
    pbi->palette[247] = {164, 160, 160, 0}; \
    pbi->palette[248] = {128, 128, 128, 0}; \
    pbi->palette[249] = {  0,   0, 255, 0}; \
    pbi->palette[250] = {  0, 255,   0, 0}; \
    pbi->palette[251] = {  0, 255, 255, 0}; \
    pbi->palette[252] = {255,   0,   0, 0}; \
    pbi->palette[253] = {255,   0, 255, 0}; \
    pbi->palette[254] = {255, 255,   0, 0}; \
    pbi->palette[255] = {255, 255, 255, 0};

#define IS_BMP(bf) (0x4d42 == bf.bfType)
bool ReadImage(MatGray* (&img), const char* filepath)
{
    ifstream fs;
    fs.open(filepath, ios::in | ios::binary);
    if(!fs.is_open())
    {
        printf("Read Image Failed, Open Image File Failed\n");
        fs.close();
        return false;
    }
    BitmapFileHeader bf = {0};

    fs.read((char*)&bf, sizeof(BitmapFileHeader));
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bf : %x, %d, %d, %d\n", bf.bfType, bf.bfSize, bf.bfReserved, bf.bfOffset);
#endif
    if(!IS_BMP(bf))
    {
        printf("Read Image Failed, File Type Not BM\n");
        return false;
    }
    unsigned long bfsize = bf.bfOffset - sizeof(BitmapFileHeader);
    PBitmapInfo pbi = (PBitmapInfo)malloc(bfsize);
    fs.read((char*)pbi, bfsize);
    size_t paletteCount = pbi->biClrUsed ;
    if(9 > pbi->biBitcount && 0 == pbi->biClrUsed)
    {
        paletteCount = 1 << pbi->biBitcount;
    }  
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bi : %d, %d, %d, %d, %d\n", pbi->biSize, pbi->biWidth, pbi->biHeight, pbi->biBitcount, pbi->biSizeImage);
    for (size_t i = 0; i < paletteCount; i++)
    {
        printf("pbi->palette[%3d] = {%3d, %3d, %3d, %d};\n",i, pbi->palette[i].B, pbi->palette[i].G, pbi->palette[i].R, pbi->palette[i].A);
    }
#endif
    if((sizeof(MatKernel8Bit) << 3) != pbi->biBitcount)
    {
        printf("Mat Kernel Not Match, Correct Bitcount:%d", pbi->biBitcount);
        return false;
    }
    img = new MatGray(pbi->biWidth, pbi->biHeight);
    int stride = ((pbi->biWidth * pbi->biBitcount + 31)>>5)<<2;
    free(pbi);
    int len = img->w * sizeof(MatKernel8Bit);
    int fit4bit = stride - len;
    char* p = (char*)img->p;
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    int oldLocation = fs.tellg();
    int newLocation = 0;
    for(int i = 0; i < img->h; i++)
    {
        fs.read(p, len);
        if(fit4bit)
            fs.seekg(fit4bit, ios::cur); 
        newLocation = fs.tellg(); 
        printf("%d, %d\n", i, newLocation - oldLocation);
        p += len;
        oldLocation = newLocation;
    }
#else
    for(int i = 0; i < img->h; i++)
    {
        fs.read(p, len);
        if(fit4bit)
            fs.seekg(fit4bit, ios::cur); 
        p += len;
    }
#endif
    fs.close();
    return true;
}
bool WriteImage(const MatGray* img, const char* filepath)
{
    if(NULL == img)
    {
        return false;
    }
    ofstream fs;
    fs.open(filepath, ios::out | ios::trunc | ios::binary);
    if(!fs.is_open())
    {
        printf("Write Image Failed, Write Image File Failed\n");
        fs.close();
        return false;
    }
    size_t bisize = 40 + sizeof(RGBAQuad) * (1 << (8 * sizeof(MatKernel8Bit)));
    int bitcount = 8 * sizeof(MatKernel8Bit);
    int stride      = ((img->w * bitcount + 31)>>5)<<2;
    int dataSize    = stride * img->h;
    int len         = img->w * sizeof(MatKernel8Bit);
    BitmapFileHeader bf;
    bf.bfType       = 0x4d42;
    bf.bfReserved   = 0;
    bf.bfOffset     = 14 + bisize;
    bf.bfSize       = bf.bfOffset + dataSize;
    if(!IS_BMP(bf))
    {
        printf("Write Image Failed, File Type Not BM\n");
        return false;
    }
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bf : %x, %d, %d, %d\n", bf.bfType, bf.bfSize, bf.bfReserved, bf.bfOffset);
#endif
    fs.write((char*)&bf, sizeof(BitmapFileHeader));
    PBitmapInfo pbi = (PBitmapInfo)malloc(bisize);
    pbi->biSize       = 40;
    pbi->biWidth      = img->w;
    pbi->biHeight     = img->h;
    pbi->biPlanes     = 1;
    pbi->biBitcount   = bitcount;
    pbi->biCompression= 0;
    pbi->biSizeImage  = dataSize;
    pbi->biXPelsPerMeter  = 0;
    pbi->biYPelsPerMeter  = 0;
    pbi->biClrUsed        = 0;
    pbi->biClrImportant   = 0;
    SET_PALETTE_255(pbi);
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bi : %d, %d, %d, %d, %d\n", pbi->biSize, pbi->biWidth, pbi->biHeight, pbi->biBitcount, pbi->biSizeImage);
#endif
    fs.write((char*)pbi, bisize);
    free(pbi);
    char* p = (char*)img->p;
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    int oldLocation = fs.tellp();
    int newLocation = 0;
    for(int i = 0; i < img->h - 1; i++)
    {
        fs.write(p, stride);
        newLocation = fs.tellp(); 
        printf("%d, %d\n", i, newLocation - oldLocation);
        p += len;
        oldLocation = newLocation;
    }
    fs.write(p, len);
    int offset = stride - len;
    if(offset)
    {
        char buffer[] = {0, 0, 0, 0};
        fs.write(buffer, offset);
    }
    newLocation = fs.tellp(); 
    printf("%d, %d\n", img->h - 1, newLocation - oldLocation);
#else
    for(int i = 0; i < img->h - 1; i++)
    {
        fs.write(p, stride);
        p += len;
    }
    fs.write(p, len);
    int offset = stride - len;
    if(offset)
    {
        char buffer[] = {0, 0, 0, 0};
        fs.write(buffer, offset);
    }
#endif
    fs.close();
    return true;
}

/* https://docs.microsoft.com/zh-cn/windows/win32/directshow/working-with-16-bit-rgb */
bool ReadImage(Mat<MatKernel16Bit>* (&img), const char* filepath)
{
    ifstream fs;
    fs.open(filepath, ios::in | ios::binary);
    if(!fs.is_open())
    {
        printf("Read Image Failed, Open Image File Failed\n");
        fs.close();
        return false;
    }
    BitmapFileHeader bf = {0};
    fs.read((char*)&bf, sizeof(BitmapFileHeader));
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bf : %x, %d, %d, %d\n", bf.bfType, bf.bfSize, bf.bfReserved, bf.bfOffset);
#endif
    if(!IS_BMP(bf))
    {
        printf("Read Image Failed, File Type Not BM\n");
        return false;
    }
    unsigned long bfsize = bf.bfOffset - sizeof(BitmapFileHeader);
    PBitmapInfo pbi = (PBitmapInfo)malloc(bfsize);
    fs.read((char*)pbi, bfsize);
    size_t paletteCount = pbi->biClrUsed;
    if(9 > pbi->biBitcount && 0 == pbi->biClrUsed)
    {
        paletteCount = 1 << pbi->biBitcount;
    }  
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bi : %d, %d, %d, %d, %d\n", pbi->biSize, pbi->biWidth, pbi->biHeight, pbi->biBitcount, pbi->biSizeImage);
    for (size_t i = 0; i < paletteCount; i++)
    {
        printf("%d,%d,%d,%d\n",pbi->palette[i].R, pbi->palette[i].G, pbi->palette[i].B, pbi->palette[i].A);
    }
#endif
    if((sizeof(MatKernel16Bit) << 3) != pbi->biBitcount)
    {
        printf("Mat Kernel Not Match, Correct Bitcount:%d", pbi->biBitcount);
        return false;
    }
    img = new Mat<MatKernel16Bit>(pbi->biWidth, pbi->biHeight);
    int stride = ((pbi->biWidth * pbi->biBitcount + 31)>>5)<<2;
    free(pbi);
    int len = img->w * sizeof(MatKernel16Bit);
    int fit4bit = stride - len;
    char* p = (char*)img->p;
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    int oldLocation = fs.tellg();
    int newLocation = 0;
    for(int i = 0; i < img->h; i++)
    {
        fs.read(p, len);
        if(fit4bit)
            fs.seekg(fit4bit, ios::cur); 
        newLocation = fs.tellg(); 
        printf("%d, %d\n", i, newLocation - oldLocation);
        p += len;
        oldLocation = newLocation;
    }
#else
    for(int i = 0; i < img->h; i++)
    {
        fs.read(p, len);
        if(fit4bit)
            fs.seekg(fit4bit, ios::cur); 
        p += len;
    }
#endif
    fs.close();
    return true;
}
bool WriteImage(const Mat<MatKernel16Bit>* img, const char* filepath)
{
    if(NULL == img)
    {
        return false;
    }
    ofstream fs;
    fs.open(filepath, ios::out | ios::trunc | ios::binary);
    if(!fs.is_open())
    {
        printf("Write Image Failed, Open Image File Failed\n");
        fs.close();
        return false;
    }
#ifndef ENABLE_MAT_KERNEL_565
    size_t bisize = 40;
#else
    size_t bisize = 40 + sizeof(RGBAQuad) * 4;
#endif
    int bitcount = 8 * sizeof(MatKernel16Bit);
    int stride      = ((img->w * bitcount + 31)>>5)<<2;
    int dataSize    = stride * img->h;
    int len         = img->w * sizeof(MatKernel16Bit);
    BitmapFileHeader bf;
    bf.bfType       = 0x4d42;
    bf.bfReserved   = 0;
    bf.bfOffset     = 14 + bisize;
    bf.bfSize       = bf.bfOffset + dataSize;
    if(!IS_BMP(bf))
    {
        printf("Write Image Failed, File Type Not BM\n");
        return false;
    }
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bf : %x, %d, %d, %d\n", bf.bfType, bf.bfSize, bf.bfReserved, bf.bfOffset);
#endif
    fs.write((char*)&bf, sizeof(BitmapFileHeader));
    PBitmapInfo pbi = (PBitmapInfo)malloc(bisize);
    pbi->biSize       = 40;
    pbi->biWidth      = img->w;
    pbi->biHeight     = img->h;
    pbi->biPlanes     = 1;
    pbi->biBitcount   = bitcount;
#ifndef ENABLE_MAT_KERNEL_565
    pbi->biCompression= 0;
#else
    pbi->biCompression= 3;
#endif
    pbi->biSizeImage  = dataSize;
    pbi->biXPelsPerMeter  = 0;
    pbi->biYPelsPerMeter  = 0;
    pbi->biClrUsed        = 0;
    pbi->biClrImportant   = 0;
    SET_PALETTE_255(pbi);
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bi : %d, %d, %d, %d, %d\n", pbi->biSize, pbi->biWidth, pbi->biHeight, pbi->biBitcount, pbi->biSizeImage);
#endif
    fs.write((char*)pbi, pbi->biSize);
    char* p = (char*)img->p;
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    int oldLocation = fs.tellp();
    int newLocation = 0;
    for(int i = 0; i < img->h - 1; i++)
    {
        fs.write(p, stride);
        newLocation = fs.tellp(); 
        printf("%d, %d\n", i, newLocation - oldLocation);
        p += len;
        oldLocation = newLocation;
    }
    fs.write(p, len);
    int offset = stride - len;
    if(offset)
    {
        char buffer[] = {0, 0, 0, 0};
        fs.write(buffer, offset);
    }
    newLocation = fs.tellp(); 
    printf("%d, %d\n", img->h - 1, newLocation - oldLocation);
#else
    for(int i = 0; i < img->h - 1; i++)
    {
        fs.write(p, stride);
        p += len;
    }
    fs.write(p, len);
    int offset = stride - len;
    if(offset)
    {
        char buffer[] = {0, 0, 0, 0};
        fs.write(buffer, offset);
    }
#endif
    fs.close();
    return true;
}

bool ReadImage(MatBGR* (&img), const char* filepath)
{
    ifstream fs;
    fs.open(filepath, ios::in | ios::binary);
    if(!fs.is_open())
    {
        printf("Read Image Failed, Open Image File Failed\n");
        fs.close();
        return false;
    }
    BitmapFileHeader bf = {0};
    fs.read((char*)&bf, sizeof(BitmapFileHeader));
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bf : %x, %d, %d, %d\n", bf.bfType, bf.bfSize, bf.bfReserved, bf.bfOffset);
#endif
    if(!IS_BMP(bf))
    {
        printf("Read Image Failed, File Type Not BM\n");
        return false;
    }
    unsigned long bfsize = bf.bfOffset - sizeof(BitmapFileHeader);
    PBitmapInfo pbi = (PBitmapInfo)malloc(bfsize);
    fs.read((char*)pbi, bfsize);
    size_t paletteCount = pbi->biClrUsed ;
    if(9 > pbi->biBitcount && 0 == pbi->biClrUsed)
    {
        paletteCount = 1 << pbi->biBitcount;
    }  
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bi : %d, %d, %d, %d, %d\n", pbi->biSize, pbi->biWidth, pbi->biHeight, pbi->biBitcount, pbi->biSizeImage);
    for (size_t i = 0; i < paletteCount; i++)
    {
        printf("%d,%d,%d,%d\n",pbi->palette[i].R, pbi->palette[i].G, pbi->palette[i].B, pbi->palette[i].A);
    }
#endif
    if((sizeof(MatKernel24Bit) << 3) != pbi->biBitcount)
    {
        printf("Mat Kernel Not Match, Correct Bitcount:%d", pbi->biBitcount);
        return false;
    }
    img = new MatBGR(pbi->biWidth, pbi->biHeight);
    int stride = ((pbi->biWidth * pbi->biBitcount + 31)>>5)<<2;
    free(pbi);
    int len = img->w * sizeof(MatKernel24Bit);
    int fit4bit = stride - len;
    char* p = (char*)img->p;
    int oldLocation = fs.tellg();
    int newLocation = 0;
    if(fit4bit)
    {
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
        for(int i = 0; i < img->h; i++)
        {
            fs.read(p, len);
            fs.seekg(fit4bit, ios::cur); 
            newLocation = fs.tellg(); 
            printf("%d, %d\n", i, newLocation - oldLocation);
            p += len;
            oldLocation = newLocation;
        }
#else
        for(int i = 0; i < img->h; i++)
        {
            fs.read(p, len);
            fs.seekg(fit4bit, ios::cur); 
            p += len;
        }
#endif
    }
    else
    {
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
        for(int i = 0; i < img->h; i++)
        {
            fs.read(p, len);
            newLocation = fs.tellg(); 
            printf("%d, %d\n", i, newLocation - oldLocation);
            p += len;
            oldLocation = newLocation;
        }
#else
        for(int i = 0; i < img->h; i++)
        {
            fs.read(p, len);
            p += len;
        }
#endif  
    }
    fs.close();
    return true;
}
bool WriteImage(const MatBGR* img, const char* filepath)
{
    if(NULL == img)
    {
        return false;
    }
    ofstream fs;
    fs.open(filepath, ios::out | ios::trunc | ios::binary);
    if(!fs.is_open())
    {
        printf("Write Image Failed, Open Image File Failed\n");
        fs.close();
        return false;
    }
    int bitcount = 8 * sizeof(MatKernel24Bit);
    int stride      = ((img->w * bitcount + 31)>>5)<<2;
    int dataSize    = stride * img->h;
    int len         = img->w * sizeof(MatKernel24Bit);
    BitmapFileHeader bf;
    bf.bfType       = 0x4d42;
    bf.bfReserved   = 0;
    bf.bfOffset     = 14 + 40;
    bf.bfSize       = bf.bfOffset + dataSize;
    if(!IS_BMP(bf))
    {
        printf("Write Image Failed, File Type Not BM\n");
        return false;
    }
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bf : %x, %d, %d, %d\n", bf.bfType, bf.bfSize, bf.bfReserved, bf.bfOffset);
#endif
    fs.write((char*)&bf, sizeof(BitmapFileHeader));
    BitmapInfo bi;
    bi.biSize       = 40;
    bi.biWidth      = img->w;
    bi.biHeight     = img->h;
    bi.biPlanes     = 1;
    bi.biBitcount   = bitcount;
    bi.biCompression= 0;
    bi.biSizeImage  = dataSize;
    bi.biXPelsPerMeter  = 0;
    bi.biYPelsPerMeter  = 0;
    bi.biClrUsed        = 0;
    bi.biClrImportant   = 0;
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bi : %d, %d, %d, %d, %d\n", bi.biSize, bi.biWidth, bi.biHeight, bi.biBitcount, bi.biSizeImage);
#endif
    fs.write((char*)&bi, bi.biSize);
    char* p = (char*)img->p;
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    int oldLocation = fs.tellp();
    int newLocation = 0;
    for(int i = 0; i < img->h - 1; i++)
    {
        fs.write(p, stride);
        newLocation = fs.tellp(); 
        printf("%d, %d\n", i, newLocation - oldLocation);
        p += len;
        oldLocation = newLocation;
    }
    fs.write(p, len);
    int offset = stride - len;
    if(offset)
    {
        char buffer[] = {0, 0, 0, 0};
        fs.write(buffer, offset);
    }
    newLocation = fs.tellp(); 
    printf("%d, %d\n", img->h - 1, newLocation - oldLocation);
#else
    for(int i = 0; i < img->h - 1; i++)
    {
        fs.write(p, stride);
        p += len;
    }
    fs.write(p, len);
    int offset = stride - len;
    if(offset)
    {
        char buffer[] = {0, 0, 0, 0};
        fs.write(buffer, offset);
    }
#endif
    fs.close();
    return true;
}

bool ReadImage(MatBGRA* (&img), const char* filepath)
{
    ifstream fs;
    fs.open(filepath, ios::in | ios::binary);
    if(!fs.is_open())
    {
        printf("Read Image Failed, Open Image File Failed\n");
        fs.close();
        return false;
    }
    BitmapFileHeader bf = {0};
    fs.read((char*)&bf, sizeof(BitmapFileHeader));
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bf : %x, %d, %d, %d\n", bf.bfType, bf.bfSize, bf.bfReserved, bf.bfOffset);
#endif
    if(!IS_BMP(bf))
    {
        printf("Read Image Failed, File Type Not BM\n");
        return false;
    }
    unsigned long bfsize = bf.bfOffset - sizeof(BitmapFileHeader);
    PBitmapInfo pbi = (PBitmapInfo)malloc(bfsize);
    fs.read((char*)pbi, bfsize);
    size_t paletteCount = pbi->biClrUsed ;
    if(9 > pbi->biBitcount && 0 == pbi->biClrUsed)
    {
        paletteCount = 1 << pbi->biBitcount;
    }  
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bi : %d, %d, %d, %d, %d\n", pbi->biSize, pbi->biWidth, pbi->biHeight, pbi->biBitcount, pbi->biSizeImage);
    for (size_t i = 0; i < paletteCount; i++)
    {
        printf("%d,%d,%d,%d\n",pbi->palette[i].R, pbi->palette[i].G, pbi->palette[i].B, pbi->palette[i].A);
    }
#endif
    if((sizeof(MatKernel32Bit) << 3) != pbi->biBitcount)
    {
        printf("Mat Kernel Not Match, Correct Bitcount:%d", pbi->biBitcount);
        return false;
    }
    img = new MatBGRA(pbi->biWidth, pbi->biHeight);
    int stride = ((pbi->biWidth * pbi->biBitcount + 31)>>5)<<2;
    free(pbi);
    int len = img->w * sizeof(MatKernel32Bit);
    int fit4bit = stride - len;
    char* p = (char*)img->p;
    if(fit4bit)
    {
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
        int oldLocation = fs.tellg();
        int newLocation = 0;
        for(int i = 0; i < img->h; i++)
        {
            fs.read(p, len);
            fs.seekg(fit4bit, ios::cur); 
            newLocation = fs.tellg(); 
            printf("%d, %d\n", i, newLocation - oldLocation);
            p += len;
            oldLocation = newLocation;
        }
#else
        for(int i = 0; i < img->h; i++)
        {
            fs.read(p, len);
            fs.seekg(fit4bit, ios::cur); 
            p += len;
        }
#endif
    }
    else
    {
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
        int oldLocation = fs.tellg();
        int newLocation = 0;
        for(int i = 0; i < img->h; i++)
        {
            fs.read(p, len);
            newLocation = fs.tellg(); 
            printf("%d, %d\n", i, newLocation - oldLocation);
            p += len;
            oldLocation = newLocation;
        }
#else
        for(int i = 0; i < img->h; i++)
        {
            fs.read(p, len);
            p += len;
        }
#endif
    }
    fs.close();
    return true;
}
bool WriteImage(const MatBGRA* img, const char* filepath)
{
    if(NULL == img)
    {
        return false;
    }
    ofstream fs;
    fs.open(filepath, ios::out | ios::trunc | ios::binary);
    if(!fs.is_open())
    {
        printf("Write Image Failed, Open Image File Failed\n");
        fs.close();
        return false;
    }
    int bitcount = 8 * sizeof(MatKernel32Bit);
    int stride      = ((img->w * bitcount + 31)>>5)<<2;
    int dataSize    = stride * img->h;
    int len         = img->w * sizeof(MatKernel32Bit);
    BitmapFileHeader bf;
    bf.bfType       = 0x4d42;
    bf.bfReserved   = 0;
    bf.bfOffset     = 14 + 40;
    bf.bfSize       = bf.bfOffset + dataSize;
    if(!IS_BMP(bf))
    {
        printf("Write Image Failed, File Type Not BM\n");
        return false;
    }
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bf : %x, %d, %d, %d\n", bf.bfType, bf.bfSize, bf.bfReserved, bf.bfOffset);
#endif
    fs.write((char*)&bf, sizeof(BitmapFileHeader));
    BitmapInfo bi;
    bi.biSize       = 40;
    bi.biWidth      = img->w;
    bi.biHeight     = img->h;
    bi.biPlanes     = 1;
    bi.biBitcount   = bitcount;
    bi.biCompression= 0;
    bi.biSizeImage  = dataSize;
    bi.biXPelsPerMeter  = 0;
    bi.biYPelsPerMeter  = 0;
    bi.biClrUsed        = 0;
    bi.biClrImportant   = 0;
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    printf("bi : %d, %d, %d, %d, %d\n", bi.biSize, bi.biWidth, bi.biHeight, bi.biBitcount, bi.biSizeImage);
#endif
    fs.write((char*)&bi, bi.biSize);
    char* p = (char*)img->p;
#ifdef PRINTF_BMP_IMAGE_IO_DEBUG
    int oldLocation = fs.tellp();
    int newLocation = 0;
    for(int i = 0; i < img->h - 1; i++)
    {
        fs.write(p, stride);
        newLocation = fs.tellp(); 
        printf("%d, %d\n", i, newLocation - oldLocation);
        p += len;
        oldLocation = newLocation;
    }
    fs.write(p, len);
    int offset = stride - len;
    if(offset)
    {
        char buffer[] = {0, 0, 0, 0};
        fs.write(buffer, offset);
    }
    newLocation = fs.tellp(); 
    printf("%d, %d\n", img->h - 1, newLocation - oldLocation);
#else
    for(int i = 0; i < img->h - 1; i++)
    {
        fs.write(p, stride);
        p += len;
    }
    fs.write(p, len);
    int offset = stride - len;
    if(offset)
    {
        char buffer[] = {0, 0, 0, 0};
        fs.write(buffer, offset);
    }
#endif
    fs.close();
    return true;
}

#endif