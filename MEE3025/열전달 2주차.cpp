#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double calcK(double T, double x, double d, double h);
double func(double h, double k, double x, double d);
double calcerr(double f, double T);
double Ts = 0, Tout = 21.0;
int main(int argc, char** argv)
{
	char buffer[100] = { 0 };
	char fname[256] = { 0 };
	int i, j, N;
	double T, f, x, step, h, k, err, esum = 0, d;
	FILE* fin, *fout, *fadd;
	
	if (argc <= 1)
	{
		printf("입력된 파일이 없습니다.\n");
		return -1;
	}

	strcpy(fname, argv[1]);
	strcpy(fname + strlen(argv[1]) - 4, "_out.txt");

	fopen_s(&fin, argv[1], "r");
	fopen_s(&fout, fname, "w");
	fopen_s(&fadd, "test.txt", "w");
//	fopen_s(&fin, "Glass.txt", "r");
//	fopen_s(&fout, "Glass_out.txt", "w");

	fscanf(fin, "%d %lf %lf %lf", &N, &step, &d, &Ts);
	d /= 1000;
	
	double* TArr = (double*)malloc(sizeof(double) * N);
	for (i = 0; i< N; i++)
		fscanf(fin, "%lf", &(TArr[i]));

	double maxh, maxk, maxesum=10000000, maxT, maxX, max_A, max_B, max_C;
	h = 10;
	for (h = 15; h <= 25; h+=5)
	{
		maxk = 0;
		for (i = 0; i < N-1; i++) // for center points
		{
			x = (double)((N - 1) * step - step * i) / 1000;

			k = calcK(TArr[i], x, d, h);
			esum = 0;
			for (j = 0; j < N-1; j++) // for center points
			{
				double y = (double)((N - 1) * step - step * j) / 1000;
				f = func(h, k, y, d);
				err = calcerr(f, TArr[j]);
				esum += err;
			}
			esum /= (N - 1);
			max_A = Ts - Tout;
			max_B = -sqrt(4 / d * h / k);
			max_C = Tout;
			fprintf(fout, "h = %lf, k = %lf, err_avg = %lf, x = %lf, T = %lf\nA = %lf, B = %lf, C = %lf\n\n", h, k, esum,x*1000,TArr[i],max_A,max_B,max_C);
			printf("h = %lf, k = %lf, err_avg = %lf, x = %lf, T = %lf\nA = %lf, B = %lf, C = %lf\n\n", h, k, esum, x * 1000, TArr[i], max_A, max_B, max_C);

			if (maxk < k && esum <= 12.5)
			{
				maxesum = esum;
				maxh = h;
				maxk = k;
				maxX = x;
				maxT = TArr[i];
				max_A = Ts - Tout;
				max_B = -sqrt(4 / d * h / k);
				max_C = Tout;
			}
		}
		fprintf(fadd, "%lf\n", maxk);
		fprintf(fout,"------------------------------------\n");
		//fprintf(fout, "\nh = %lf, k = %lf, esum = %lf, x = %lf, T = %lf\n\n", maxh, maxk, maxesum, maxX * 1000, maxT);
		//printf("\nh = %lf, k = %lf, esum = %lf, x = %lf, T = %lf\nA = %lf, B = %lf, C = %lf\n\n", maxh, maxk, maxesum, maxX * 1000, maxT, max_A, max_B, max_C);
	}
	
	free(TArr);
	fclose(fin);
	fclose(fout);
	fclose(fadd);
	system("pause");
	return 0;
}

double ABS(double x)
{
	if (x >= 0)
		return x;
	else
		return -x;
}

double calcK(double T, double x, double d, double h)
{
	double ret, S = (T - Tout) / (Ts - Tout);
	double a = pow(log(S) / x, 2);
	return ret = 4 * h / d / a;

}

double func(double h, double k, double x, double d) 
{
	double ret, C = 4 / d * h / k;
	ret = (Ts - Tout) * exp(-sqrt(C) * x) + Tout;
	return ret;
}

double calcerr(double f, double T)
{
	return ABS((f - T) / f) * 100;
}
