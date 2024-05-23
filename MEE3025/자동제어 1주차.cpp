#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double* time, * input, * output, icond, ocond, mcond;
int size = 0;

typedef struct Rec {
	int in;
	int out;
	int max;
}rec;

int main(int argc, char** argv)
{
	
	if (argc <= 1)
	{
		printf("Selected No File\n");
		return -1;
	}
	
	char name[512] = { 0 };
	strcpy(name, argv[1]);
	strcpy(name + strlen(argv[1]) - strlen(".txt"), "_out.txt");

	FILE* fin, * fout;
	fopen_s(&fin, argv[1], "r");
	fopen_s(&fout, name, "w");
	
	/* for test
	FILE* fin, * fout;
	fopen_s(&fin, "0.1.txt", "r");
	fopen_s(&fout, "0.1_out.txt", "w");
	*/

	// 맨 위에 '전체갯수 input판정선 output판정선 min판정선 형태로 추가입력 + time은 오름차순 (앞부분 데이터 컷팅 필요)
	fscanf(fin, "%d", &size);
	/*
	fscanf(fin, "%lf", &icond);
	fscanf(fin, "%lf", &ocond);
	fscanf(fin, "%lf", &mcond);
	*/

	printf("%s loaded.\n", argv[1]);
	time = (double*)malloc(sizeof(double) * size);
	input = (double*)malloc(sizeof(double) * size);
	output = (double*)malloc(sizeof(double) * size);

	for (int i = 0; i < size; i++)
	{
		fscanf(fin, "%lf", &(time[i]));
		fscanf(fin, "%lf", &(input[i]));
		fscanf(fin, "%lf", &(output[i]));
		icond += input[i];
		ocond += output[i];
	}
	icond /= size;
	ocond /= size;
	mcond = ocond;
	printf("total size = %d, input >= %lf, output >= %lf, min <= %lf \n", size, icond, ocond, mcond);

	rec icheck[100] = { 0 };  // input local max index rec
	int ilen = 0, itoggle = 0;
	for (int i = 0; i < size; i++)
	{
		if (itoggle == 0) // find max in
		{
			if (input[i] > icond + 0.05)
			{
				itoggle = 1;
				icheck[ilen].in = i;
				icheck[ilen].out = -1;
				ilen++;
			}
		}
		else // find max out
		{
			if (input[i] < icond - 0.05)
			{
				itoggle = 0;
				icheck[ilen-1].out = i;
			}
		}
	}
	if (icheck[ilen - 1].out == -1)
		icheck[ilen - 1].out = size - 1;

	rec ocheck[100] = { 0 };  // output local max index rec
	int olen = 0, otoggle = 0;
	for (int i = 0; i < size; i++)
	{
		if (otoggle == 0) // find max in
		{
			if (output[i] > ocond + 0.05)
			{
				otoggle = 1;
				ocheck[olen].in = i;
				ocheck[olen].out = -1;
				olen++;
			}
		}
		else // find max out
		{
			if (output[i] < ocond - 0.05)
			{
				otoggle = 0;
				ocheck[olen-1].out = i;
			}
		}
	}
	if (ocheck[olen - 1].out == -1)
		ocheck[olen - 1].out = size - 1;

	rec mcheck[100] = { 0 };  // output local min index rec
	int mlen = 0, mtoggle = 0;
	for (int i = 0; i < size; i++)
	{
		if (mtoggle == 0) // find min in
		{
			if (output[i] < mcond - 0.05)
			{
				mtoggle = 1;
				mcheck[mlen].in = i;
				mcheck[mlen].out = -1;
				mlen++;
			}
		}
		else // find min out
		{
			if (output[i] > mcond + 0.05)
			{
				mtoggle = 0;
				mcheck[mlen - 1].out = i;
			}
		}
	}
	if (mcheck[mlen - 1].out == -1)
		mcheck[mlen - 1].out = size - 1;

	fprintf(fout,"--------------------- input local max list\n");
	for (int i = 0; i < ilen; i++)
	{
		int index = icheck[i].in;
		double max = input[index];
		for (int j = icheck[i].in; j <= icheck[i].out; j++)
		{
			if (max < input[j])
			{
				max = input[j];
				index = j;
			}
		}
		icheck[i].max = index;

		fprintf(fout, "%lf\t%lf\n", time[index], input[index]);
	}
	
	fprintf(fout, "\n--------------------- output local max list\n");
	for (int i = 0; i < olen; i++)
	{
		int index = ocheck[i].in;
		double max = output[index];
		for (int j = ocheck[i].in; j <= ocheck[i].out; j++)
		{
			if (max < output[j])
			{
				max = output[j];
				index = j;
			}
		}
		ocheck[i].max = index;

		fprintf(fout, "%lf\t%lf\n", time[index], output[index]);
	}

	fprintf(fout, "\n--------------------- output local min list\n");
	for (int i = 0; i < mlen; i++)
	{
		int index = mcheck[i].in;
		double min = output[index];
		for (int j = mcheck[i].in; j <= mcheck[i].out; j++)
		{
			if (min > output[j])
			{
				min = output[j];
				index = j;
			}
		}
		mcheck[i].max = index;

		fprintf(fout, "%lf\t%lf\n", time[index], output[index]);
	}

	printf("saved as %s\n", name);
	
	fclose(fin);
	system("pause");
	return 0;
}
