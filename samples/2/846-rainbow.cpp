// rainbow1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <openssl/md5.h>
//选用多少个R函数
#define RCount 6

//假设口令长度6，由数字和小写字母组成
#define SIZE 6

//彩虹表容量
#define TABLESIZE 1000000

unsigned char PASSCHAR[] = "0123456789abcdefghijklmnopqrstuvwxyz";
unsigned char HASH_PASSCHAR[256];
unsigned char pass0[TABLESIZE][SIZE];
unsigned char pass6[TABLESIZE][SIZE];
int LEN = 36;

int random(int m)//取随机数
{
	return rand() % m;
}
void R6(unsigned char * pwd, unsigned char *hash, int n_redux)
{

	/* Convert the hash to four unsigned 32-bit integers. */

	uint32_t idx[4];
	idx[0] = (*(uint32_t*)(hash) ^ n_redux);
	idx[1] = (*(uint32_t*)(hash + 4));
	idx[2] = (*(uint32_t*)(hash + 8));
	idx[3] = (*(uint32_t*)(hash + 12));

	/* Compute the password */

	idx[0] %= LEN * LEN;
	idx[1] %= LEN * LEN;
	idx[2] %= LEN * LEN;
	idx[3] %= LEN * LEN;

	pwd[0] = PASSCHAR[idx[0] / LEN];
	pwd[1] = PASSCHAR[idx[1] / LEN];
	pwd[2] = PASSCHAR[idx[1] % LEN];
	pwd[3] = PASSCHAR[idx[2] / LEN];
	pwd[4] = PASSCHAR[idx[2] % LEN];
	pwd[5] = PASSCHAR[idx[3] / LEN];
	/* Finish the password. */

}
void Creat_rainbow_table()
{
	int i, n;
	unsigned char in[SIZE];
	unsigned char pwd[SIZE];
	unsigned char digest[16];//md5摘要值,16字节,128比特
	for (n = 0;n < TABLESIZE;n++)
	{
		/*pass0*/
		for (i = 0;i < SIZE;i++)
		{
			in[i] = PASSCHAR[random(36)];
			pass0[n][i] = in[i];
			//	printf("%c ", in[i]);
		}
		MD5(in, SIZE, digest);
		//printf("\n");
		for (i = 0;i < RCount - 1;i++)
		{
			R6(pwd, digest, i);
			MD5(pwd, SIZE, digest);
		}
		R6(pwd, digest, RCount - 1);
		/*pass6*/
		for (i = 0;i < SIZE;i++)
		{
			pass6[n][i] = pwd[i];
			//	printf("%c ", pwd[i]);
		}
		//printf("\n\n");	
	}
}
void test()
{
	int i, j, n;
	unsigned char in1[SIZE + 1];
	unsigned char pwd0[SIZE];
	unsigned char digest[16];//md5摘要值,16字节,128比特
	printf("pass0:\n");
	scanf("%s", in1);
	getchar();
	MD5(in1, SIZE, digest);
	for (i = 0;i < RCount - 1;i++)
	{
		R6(pwd0, digest, i);
		printf("pass%d:\n", i + 1);
		for (j = 0;j < SIZE;j++)
		{
			printf("%c ", pwd0[j]);
		}
		printf("\n");
		MD5(pwd0, SIZE, digest);
	}
	R6(pwd0, digest, RCount - 1);
	/*pass6*/
	printf("pass6:\n");
	for (i = 0;i < SIZE;i++)
	{
		printf("%c ", pwd0[i]);
	}
	printf("\n");
}
int Find_pass6(unsigned char *pwd1, int hi)//密文在hi处
{
	int i, j, flag;
	unsigned char digest1[16];//md5摘要值,16字节,128比特
	unsigned char pwd2[SIZE];
	for (i = 0;i < TABLESIZE;i++)
	{
		flag = 1;
		for (j = 0;j < SIZE - 1 && flag == 1;j++)
		{
			if (pwd1[j] == pass6[i][j])
				flag = 1;
			else
				flag = 0;
		}
		if (pwd1[SIZE - 1] == pass6[i][SIZE - 1] && flag == 1)
		{
			if (hi == 1)
			{
				for (j = 0;j < SIZE;j++)
				{
					printf("%c ", pass0[i][j]);
				}
			}
			else
			{
				MD5(pass0[i], SIZE, digest1);
				for (i = 0;i < hi - 2;i++)
				{
					R6(pwd2, digest1, i);
					MD5(pwd2, SIZE, digest1);
				}
				R6(pwd1, digest1, hi - 2);
				for (j = 0;j < SIZE;j++)
				{
					printf("%c ", pwd1[j]);
				}
			}

			return 1;
		}
	}
	return 0;
}
int main()
{
	int i, j, op = 1;
	while (op) {
		system("cls");	printf("\n\n");
		printf("                    Menu for Rainbow              \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. 生成彩虹表 \n");
		printf("    	  2. 获得测试数据\n");
		printf("    	  3. 读取彩虹表并尝试破解\n");
		printf("    	  0. 退出系统         \n");
		printf("-------------------------------------------------\n");
		printf("    请选择你的操作[0~3]:");
		scanf("%d", &op);
		switch (op) {
		case 1:
			printf("正在生成彩虹表中...\n");
			Creat_rainbow_table();
			printf("生成彩虹表完毕!\n");
			FILE *fp;
			if ((fp = fopen("rainbow.txt", "w")) == NULL)
			{
				fprintf(stderr, "Can not open rainbow.txt file.\n");
				return 0;
			}
			for (i = 0;i<TABLESIZE;i++)
			{
				fwrite(pass0[i], 1, SIZE, fp);
				fprintf(fp, " ");
				fwrite(pass6[i], 1, SIZE, fp);
				fprintf(fp, "\n");
			}
			fclose(fp);
			/*test
			int i, n;
			for (n = 0;n < TABLESIZE;n++)
			{
			for (i = 0;i < SIZE;i++)
			{
			printf("%c ",pass0[n][i] );
			}
			printf("\n");
			for (i = 0;i < SIZE;i++)
			{

			printf("%c ", pass6[n][i]);
			}
			printf("\n\n");
			}
			*/
			printf("输出2组彩虹表中的明文以备测试\n");
			for (i = 0;i < SIZE;i++)
			{
				printf("%c ", pass0[0][i]);
			}
			printf("\n");
			for (i = 0;i < SIZE;i++)
			{
				printf("%c ", pass0[TABLESIZE - 1][i]);
			}
			printf("\n");
			printf("按任意键继续...");
			getchar();getchar();
			break;
		case 2:
			test();
			printf("按任意键继续...");
			getchar();
			break;
		case 3:
			int success = 0;
			printf("请输入口令(口令长度6，由数字和小写字母组成);\n");
			unsigned char input[SIZE + 1];
			unsigned char cipher[16];//密文
			unsigned char digest[16];
			unsigned char pwd1[SIZE];
			scanf("%s", input);
			getchar();
			MD5(input, SIZE, cipher);
			printf("得到密文;\n");
			for (i = 0;i < 16;i++)
			{
				//	cipher1[i] = cipher[i];
				printf("%02X", cipher[i]);
			}
			printf("\n");
			printf("正在载入彩虹表...\n");
			FILE *fp1;
			if ((fp1 = fopen("rainbow.txt", "r")) == NULL)
			{
				fprintf(stderr, "Can not open rainbow.txt file.\n");
				return 0;
			}
			for (i = 0;i<TABLESIZE;i++)
			{
				fread(pass0[i], 1, SIZE, fp1);
				fscanf(fp1, " ");
				fread(pass6[i], 1, SIZE, fp1);
				fscanf(fp1, "\n");
			}
			fclose(fp1);
			printf("进行彩虹表破解...\n");
			//若密文在h6,h5,h4,h3,h2,h1处
			for (i = 0;i < RCount;i++)
			{
				R6(pwd1, cipher, RCount - 1 - i);
				for (j = 0;j < i;j++)
				{
					MD5(pwd1, SIZE, digest);
					R6(pwd1, digest, RCount - 5 + j);
				}
				if (Find_pass6(pwd1, RCount - i) == 1)
				{
					success = 1;
					printf("破译成功!\n");
				}
			}
			if (success == 0)
				printf("破译失败!\n");
			printf("按任意键继续...");
			getchar();
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
	getchar();getchar();
	return 0;
}

