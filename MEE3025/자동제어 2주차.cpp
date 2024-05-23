#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double ERR = -12345;
int size = 0;

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		printf("Selected No File\n");
		return -1;
	}

	char name[512] = { 0 };
	char name2[512] = { 0 };
	strcpy(name, argv[1]);
	strcpy(name + strlen(argv[1]) - strlen(".txt"), "_out.txt");
	strcpy(name2, argv[1]);
	strcpy(name2 + strlen(argv[1]) - strlen(".txt"), "_err.txt");

	FILE* fin, * fout;
	fopen_s(&fin, argv[1], "r");
	fopen_s(&fout, name, "w");

	/* for test
	FILE* fin, * fout;
	fopen_s(&fin, "0.1.txt", "r");
	fopen_s(&fout, "0.1_out.txt", "w");
	*/

	fscanf(fin, "%d", &size);
	printf("%s loaded.\n", argv[1]);

	int mode;
	printf("1. 데이터 가공 (시간 정렬, 중복 제거, 누락 정렬)\n2. 데이터 통합 (누락된 부분 제거)\n");
	scanf("%d", &mode);

	if (mode == 1)
	{
		int index = 0, check = 0;
		double input[10], pinput, ptime, poutput;

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < 10; j++)
				fscanf(fin, "%lf", &(input[j]));
			
			int fill = 0;
			if (i > 0)
			{
				// 중복 제거
				if ((input[0] - ptime < 0.001))
					continue;
				// 누락 Fill
				if (input[0] - ptime >= 0.003)
					fill = (int)(((int)(input[0]*1000) - (int)(ptime*1000)) / 2) - 1;
			}

			if (fill > 0)
			{
				for (int j = 0; j < fill; j++)
				{
					fprintf(fout, "%lf\t%lf\t%lf\t%lf\n", input[0], ((double)index) * 0.002, ERR, ERR);
					index++;
				}
			}

			fprintf(fout, "%lf\t%lf\t%lf\t%lf\n",input[0], ((double)index) * 0.002, input[5], input[4]);
			index++;

			ptime = input[0];
			pinput = input[5];
			poutput = input[4];

			if (check == 1 && pinput > 0)
			{
				printf("index=%d time=%lf input=%lf output=%lf\n", index, ptime, pinput, poutput);
				check = 0;
			}
			if (check == 0 && pinput < 0)
				check = 1;

		}
		printf("saved as %s\n", name);
	}
	else if (mode == 2)
	{
		FILE* ferr;
		fopen_s(&ferr, name2, "w");
		double input[7];
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < 7; j++)
				fscanf(fin, "%lf", &(input[j]));

			if (input[1] < -10000 || input[2] < -10000 || input[3] < -10000 || input[4] < -10000)
				continue;

			fprintf(fout, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",input[0], input[1], input[2], input[3], input[4], input[5], input[6]);
			fprintf(ferr, "%lf\t%lf\t%lf\t%lf\n", input[0], input[1] - input[2], input[5] - input[3], input[6] - input[4]);
		}
		fclose(ferr);
		printf("saved as %s\n", name);
		printf("saved as %s\n", name2);
	}

	fclose(fin);
	fclose(fout);
	system("pause");
	return 0;
}
