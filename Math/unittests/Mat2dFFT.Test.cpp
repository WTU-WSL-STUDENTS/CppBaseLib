/*
 * @Description: ����Ҷ�任��������
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
     * fftw_complex ��FFTW�Զ���ĸ�����
	 * ����<complex>���ʹ��STL�ĸ�����
     */
	fftw_complex x[5];
	fftw_complex y[5];
	/**
	 * ����plan���������г��ȡ��������С�������С��任���򡢱任ģʽ
	 * ���in��outָ����ͬΪԭλ���㣬����Ϊ��ԭλ����
	*/
	fftw_plan plan = fftw_plan_dft_1d(5, x, y, FFTW_FORWARD, FFTW_ESTIMATE);

	/*����ÿ��plan��Ӧ��"һ�ζ��� ���ʹ��"��ͬһplan�������ٶȼ���*/
	for (int i = 0; i < 5; i++) {
		x[i][REAL] = i;
		x[i][IMAG] = 0;
	}
	fftw_execute(plan);
	for (int i = 0; i < 5; i++) {
		printf("%lf  %lf\n", y[i][REAL], y[i][IMAG]);
	}

	//����plan
	fftw_destroy_plan(plan);


	return 0;
}