/*
 * @Description: 矩阵元素的拉伸变换
 * ScalaOperateBase     : 拉伸变换基类, 通过重载 ScalaOperate 实现自定义变换；
 * PiecewiseScalaOperate: 分段拉伸操作, 继承 ScalaOperateBase；
 * LinearScala          : 线性拉伸算法实现
 * PiecewiseScala       : 分段拉伸算法实现。当传入Map只有一行且阈值为 255 , 即为 用户定义的任意拉伸变换
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-18 06:52:39
 * @LastEditors: like
 * @LastEditTime: 2021-11-19 06:57:47
 */
#ifndef MAT2D_SCALA_HPP
#define MAT2D_SCALA_HPP

#include <Mat.hpp>
#include <map>

template<typename T>
class ScalaOperateBase /* 通过继承该类，并重写虚函数 ScalaOperate 可以实现任意数据类型的值变换 */
{
private:
    Mat<T>* mat;
protected:
    ScalaOperateBase(Mat<T>* _mat) : mat(_mat){}
    virtual void ScalaOperate(int row, int col, T& val){throw " Can't Use ScalaOperate Without Override First ";}
public:
    /* 分段线性函数会用到的数据结构 */
    typedef void (*DoScalaOperate)(T& val);/* 泛型函数指针解决方案 */
    /**
     * PiecewiseLinearMap : 
     * 0 - T0 : Operate0
     * T0 - T1 : Operate1
    */
    typedef std::map<T, DoScalaOperate> PiecewiseLinearMap; 
    
    virtual bool DoFunc()
    {
        VALRET_ASSERT(mat, false);
        T* p = mat->p;
        for(int r = 0; r < mat->h; r++)
        {
            for(int c = 0; c < mat->w; c++, p++)
            {
                ScalaOperate(r, c, *p);
            }
        }
        return true;
    }
};
/* 分段线性函数 */
class PiecewiseScalaOperate : public ScalaOperateBase<unsigned char>
{
private:
    PiecewiseLinearMap plMap;
protected:
    void ScalaOperate(int r, int c, unsigned char& val) override
    {
        PiecewiseLinearMap::iterator it = plMap.begin();
        for(; it != plMap.end(); it++)
        {
            if(val <= it->first)
            {
                (*(it->second))(val);
                return;
            }
        }
    }
public:
    PiecewiseScalaOperate(Mat<unsigned char>* mat, const PiecewiseLinearMap& piecewiseMap) : 
        ScalaOperateBase<unsigned char>(mat), plMap(piecewiseMap){}  
};

/* 计算矩阵的最大最小值 */
template<typename T>
bool GetMatMinMax(Mat<T>* mat, T& min, T& max)
{
    VALRET_ASSERT(mat, false);
    memset(&min, 0xff, sizeof(T));
    memset(&max, 0, sizeof(T));
    T* p = mat->p;
    min = *p;
    max = *p;
    for(size_t i = 1; i < mat->length; i++, p++)
    {
        if(min > *p)
        {
            min = *p;
        }
        if(max < *p)
        {
            max = *p;
        }
    }
    return true;
}
/**
 * 线性拉伸
 * [a, b] -> [a', b']
 * 
 * X' = (X - a) * ((b' - a') / (b - a)) + a' 
*/
template<typename T>
bool LinearScala(const T& min, const T& max, const T& scalaedMin, const T& scalaedMax, unsigned char* lsMapping/* 容量要大于 max */)
{
    VALRET_ASSERT(min < max && scalaedMin <= scalaedMax, false);
    T oldSpan   = max - min;
    T newSpan   = scalaedMax - scalaedMin;
    T spanRate  = newSpan / oldSpan;
    for(T i = min; i < max; i++)
    {
        lsMapping[i] = (i - min) * spanRate + scalaedMin;
    }
    return true;
}
/* 计算分段函数的映射, 可实现分段线性 & 非线性变换的融合 */
bool PiecewiseScala(PiecewiseScalaOperate::PiecewiseLinearMap piecewiseMap, unsigned char* plsMapping)
{
    VALRET_ASSERT(plsMapping, false);
    PiecewiseScalaOperate::PiecewiseLinearMap::iterator it = piecewiseMap.begin();
    for(int i = 0; i < 256 && it != piecewiseMap.end(); it++)/* 0 - 255 依据分段函数重新计算出新的映射值 */
    {
        while(i <= it->first)
        {
            plsMapping[i] = i;
            (*(it->second))( plsMapping[i] );
            i++;
        }
    }
    return true;
}

#endif