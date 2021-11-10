/*
 * @Description: æÿ’Û
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-14 21:10:54
 * @LastEditors: like
 * @LastEditTime: 2021-11-09 05:48:10
 */
#ifndef MAT_HPP
#define MAT_HPP

#define _CRT_SECURE_NO_WARNINGS
#include <CompliedEntry.h>
#include <stdlib.h>
#include <string>

using namespace std;

template<typename TKernel>
inline std::string ToString(const TKernel* t)
{
    return to_string(*t);
}

template<typename TKernel>
class Mat
{
public:
    TKernel * p;
    int w;
    int h;
    size_t length;
    Mat() : p(NULL), w(0), h(0), length(0){}
    Mat(int width, int height) : w(width), h(height),length(width * height), p((TKernel*)calloc(length, sizeof(TKernel))){}
    Mat(int width, int height, const TKernel* source) : Mat(width, height){memcpy(p, source, length * sizeof(TKernel));}
    ~Mat()
    {
        if(p)
        {
            free(p);
        }
    }
    void Init(const TKernel& val)
    {
        for(int i = 0; i < w * h; i++)
        {
            p[i] = val;
        }
    }
    TKernel* RowAt(int index)
    {
        return p + index * w; 
    }
    TKernel* operator[](int i)
    {
        return p + i;
    }
    string ToMatString(string strSplit = " ")
    {
        TKernel* scan0 = p;
        string str = ToString<TKernel>(scan0++);
        for(size_t i = 1; i < length; i++, scan0++)
        {     
            if(i % w)
            {
                str += strSplit;
            }  
            else
            {
                str += "\n";
            }    
            str += ToString<TKernel>(scan0);
        }
        str += "\n";
        return str;
    }
};

#endif