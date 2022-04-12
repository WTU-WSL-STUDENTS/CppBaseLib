/*
 * @Description: 傅里叶变换测试用例
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-12-03 07:49:00
 * @LastEditors: like
 * @LastEditTime: 2022-01-17 10:50:06
 */
// #include <Mat2dFFT.hpp>
#include <fftw3.h>
#define REAL 0
#define IMAG 1

int main()
{
    /**
     * fftw_complex 是FFTW自定义的复数类
	 * 引入<complex>则会使用STL的复数类
     */
	fftw_complex x[5];
	fftw_complex y[5];
	/**
	 * 定义plan，包含序列长度、输入序列、输出序列、变换方向、变换模式
	 * 如果in和out指针相同为原位运算，否则为非原位运算
	*/
	fftw_plan plan = fftw_plan_dft_1d(5, x, y, FFTW_FORWARD, FFTW_ESTIMATE);

	/*对于每个plan，应当"一次定义 多次使用"，同一plan的运算速度极快*/
	for (int i = 0; i < 5; i++) {
		x[i][REAL] = i;
		x[i][IMAG] = 0;
	}
	fftw_execute(plan);
	for (int i = 0; i < 5; i++) {
		printf("%lf  %lf\n", y[i][REAL], y[i][IMAG]);
	}

	//销毁plan
	fftw_destroy_plan(plan);


	return 0;
}