/*
 * @Description: ����Ԫ�ص�����任
 * ScalaOperateBase     : ����任����, ͨ������ ScalaOperate ʵ���Զ���任��
 * PiecewiseScalaOperate: �ֶ��������, �̳� ScalaOperateBase��
 * LinearScala          : ���������㷨ʵ��
 * PiecewiseScala       : �ֶ������㷨ʵ�֡�������Mapֻ��һ������ֵΪ 255 , ��Ϊ �û��������������任
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
class ScalaOperateBase /* ͨ���̳и��࣬����д�麯�� ScalaOperate ����ʵ�������������͵�ֵ�任 */
{
private:
    Mat<T>* mat;
protected:
    ScalaOperateBase(Mat<T>* _mat) : mat(_mat){}
    virtual void ScalaOperate(int row, int col, T& val){throw " Can't Use ScalaOperate Without Override First ";}
public:
    /* �ֶ����Ժ������õ������ݽṹ */
    typedef void (*DoScalaOperate)(T& val);/* ���ͺ���ָ�������� */
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
/* �ֶ����Ժ��� */
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

/* �������������Сֵ */
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
 * ��������
 * [a, b] -> [a', b']
 * 
 * X' = (X - a) * ((b' - a') / (b - a)) + a' 
*/
template<typename T>
bool LinearScala(const T& min, const T& max, const T& scalaedMin, const T& scalaedMax, unsigned char* lsMapping/* ����Ҫ���� max */)
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
/* ����ֶκ�����ӳ��, ��ʵ�ֶַ����� & �����Ա任���ں� */
bool PiecewiseScala(PiecewiseScalaOperate::PiecewiseLinearMap piecewiseMap, unsigned char* plsMapping)
{
    VALRET_ASSERT(plsMapping, false);
    PiecewiseScalaOperate::PiecewiseLinearMap::iterator it = piecewiseMap.begin();
    for(int i = 0; i < 256 && it != piecewiseMap.end(); it++)/* 0 - 255 ���ݷֶκ������¼�����µ�ӳ��ֵ */
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