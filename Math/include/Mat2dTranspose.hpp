/*
 * @Description: �����ת�� 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-11-09 06:56:59
 * @LastEditors: like
 * @LastEditTime: 2021-11-09 09:00:16
 */
#ifndef MAT2D_TRANSPOSE_HPP
#define MAT2D_TRANSPOSE_HPP
#include <Mat.hpp>

 /*��̣�ԭʼ�����->�¾����*/   
int GetNext(int i, int m, int n){return (i%n)*m + i / n;}/*�¾����row * �¾���� + �¾����col*/
/*ǰ�����¾����->ԭʼ�����*/
int GetPre(int i, int m, int n){return (i%m)*n + i / m;}

/*��ԭ�ء�ת�þ��� https://www.cnblogs.com/jcchan/p/10402403.html*/
template<typename T>
bool SelfTranspose(Mat<T>* mat)
{  
    int newWidth    = mat->h;
    int newHeight   = mat->w;
    int elementCount= (int)mat->length;
    T* span = mat->p;
    int m = mat->h;
    int n = mat->w;
    for(int i = 0; i < elementCount; ++i)
    {
        int next = GetNext(i, m, n);
        while (next > i)
        {
            next = GetNext(next, m, n);
        }
        if(next == i)
        {
            T temp = span[i];   // �ݴ� 
            int cur = i;        // ��ǰ�±� 
            int pre = GetPre(cur, m, n); 
            while(pre != i) 
            { 
                span[cur] = span[pre]; 
                cur = pre; 
                pre = GetPre(cur, m, n); 
            } 
            span[cur] = temp; 
        }
    }
    mat->w = newWidth;
    mat->h = newHeight;
    return true;
}

#endif

