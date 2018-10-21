// ConsoleApplication5.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <stdio.h>
#include <string>
#include "openssl/bn.h"
#include <openssl/sha.h>
#include <time.h>
#define T 100000//生成彩虹链的数目
int i1 = 0;
typedef struct rainbowlist
{
	struct rainbowlist *pre;
	unsigned char plain[7];//彩虹链的首端
	unsigned char rainbow[7];//彩虹链的末尾
	struct rainbowlist *next;
}L;
unsigned char compute(unsigned char sum)//计算R函数值
{
	unsigned int y;
	unsigned char final;
	y = sum % 36;
	if (y >= 0 && y < 10)
	{
		final = y % 10 + 48;
	}
	if (y >= 10 && y < 36)
	{
		final = y % 26 + 97;
	}
	/*if (y >= 36 && y < 62)
	{
	final = y % 26 + 65;
	}*/
	return final;
}
void R(unsigned char* rainbow, unsigned char* hash)//约简函数
{
	unsigned char temp[7];
	int i;
	temp[0] = hash[0] + hash[19] + hash[6] + hash[13];
	temp[1] = hash[1] + hash[18] + hash[7] + hash[12];
	temp[2] = hash[2] + hash[17] + hash[8];
	temp[3] = hash[3] + hash[16] + hash[9];
	temp[4] = hash[4] + hash[15] + hash[10];
	temp[5] = hash[5] + hash[14] + hash[11];
	for (i = 0; i < 6; i++)
	{
		rainbow[i] = compute(temp[i]);
	}
	rainbow[6] = '\0';
	return;
}
unsigned char rand_generate(unsigned int l, unsigned int h)//随机数生成,范围为l-h
{
	unsigned char t;
	t = rand() % (h - l + 1) + l;
	return t;
}
unsigned char rain_generate(void)//明文单字符随机生成
{
	unsigned char t;
	unsigned int a, b;
	a = rand_generate(1, 2);
	if (a == 1)
	{
		t = rand_generate(48, 57);
	}
	if (a == 2)
	{
		t = rand_generate(97, 122);
	}
	/*if (a==3)
	{
	t = rand_generate(65, 90);
	}*/
	return t;
}
void node_generate(unsigned char* plain, unsigned char * rainbow)//结点（单条链）生成
{
	unsigned char digest[21];
	size_t n;//传输信息的长度
	for (size_t i = 0; i < 6; i++)//生成随机明文
	{
		plain[i] = rain_generate();
	}
	plain[6] = '\0';
	//printf("%s\n", plain);
	for (int i = 0; i < 7; i++)
	{
		rainbow[i] = plain[i];
	}
	n = strlen((const char*)plain);//输入信息的长度，以字节为单位
    //生成的彩虹链长度为100
	int target;
	for (target = 0; target < 100; target++)
	{
		SHA1(rainbow, 6, digest);
		digest[20] = '\0';
		//printf("%s\n", digest);
		R(rainbow, digest);
		//printf("%s\n", rainbow);
	}
	return;
}
void rainbowlist_generate(struct rainbowlist * first)//参数为彩虹链的首链
{
	int i;
	first->pre = NULL;
	first->next = NULL;
	struct  rainbowlist* pos = first;
	node_generate(first->plain, first->rainbow);
	for (i = 0; i < T; i++)
	{
		struct  rainbowlist* newprior = (struct rainbowlist*)malloc(sizeof(rainbowlist));
		newprior->pre = pos;
		newprior->next = NULL;
		pos->next = newprior;
		node_generate(newprior->plain, newprior->rainbow);
		pos = newprior;
	}
	return;
}
struct rainbowlist *hit(unsigned char* rainbow, struct rainbowlist * first)//判断彩虹链中储存的末尾值是否符合
{
	i1 = 1;
	struct rainbowlist *pos = first;
	do
	{
		if (!strcmp((const char*)rainbow, (const char*)pos->rainbow))
		{
			/*unsigned char temp1[7];
			unsigned char digest[21];
			SHA1(pos->plain, 6, digest);
			for (int i = 0; i < 100; i++)
			{
				R(temp1, digest);
				SHA1(temp1, 6, digest);
				digest[20] = '\0';
				printf("%s\n", temp1);
			}*/
			printf("符合\n");
			return pos;
		}
		else
		{
			pos = pos->next;
			i1++;
		}
	} while (pos->next != NULL);
	return NULL;
}
void search(unsigned char* hash, struct rainbowlist * first)
{
	unsigned char temp1[7];
	unsigned char digest[21];
	struct rainbowlist *pos;
	R(temp1, hash);
	//对所要查询的哈希值进行约简
	int target = 100; 
	while (1) {
		pos = hit(temp1, first);
		if (pos != NULL)
		{
			SHA1(pos->plain, 6, digest);
			if(target == 1)
			printf("%s\n", pos->plain);
			else {
				for (int i = 0; i < target - 1; i++)
				{
					R(temp1, digest);
					SHA1(temp1, 6, digest);
					digest[20] = '\0';
				}
				printf("%s\n", temp1);
			}
			return;
		}
		else {
			target = target - 1;
			if (target <= 0)
				break;
			SHA1(temp1, 6, digest);
			digest[20] = '\0';
			R(temp1, digest);
		}
	}
	printf("Not Find!\n");
	return;
}
int main()
{
	srand(time(0));//设定随机数种子
	unsigned char a;
	//unsigned char store[13];
	//a = rain_generate();
	//printf("%c\n", a);
	struct rainbowlist* first = (struct rainbowlist*)malloc(sizeof(rainbowlist));//初始化首结点
	first->pre = NULL;
	first->next = NULL;
	//node_generate(first->plain, first->rainbow);
	//printf("%s\n%s\n", first->plain, first->rainbow);
	rainbowlist_generate(first);
	//printf("%s\n%s\n", first->plain, first->rainbow);
	//printf("%s\n%s\n", first->next->next->plain, first->next->next->rainbow);
	FILE* fp = fopen("data.txt", "w");;
	/*if ((fp = fopen("C:/Users/YoYoLuMing/Desktop/rainbowtable.txt", "r")) == NULL) { //打开要写入的文件
		printf("can not open file!\n");
		exit(0);
	}
	*/
	struct rainbowlist* pos = first;
	 do
	{
	fprintf(fp, "%s ", pos->plain);
	fprintf(fp, "%s", pos->rainbow);
	fprintf(fp, "\n");
	pos = pos->next;
	} while (pos->next!=NULL);
	fclose(fp);
	/*while (!feof(fp))
	{
		fscanf(fp, "%s", store);
		store[12] = '\0';
		strncpy((char*)pos->plain, (const char*)store, 6);
		strncpy((char*)pos->rainbow, (const char*)store + 6, 6);
		//printf("%s  %s", pos->plain,pos->rainbow);
		pos = pos->next;
	}*/
	unsigned char test[6] = { 0 };
	unsigned char rainbow[7];
	unsigned char digest[21];
	digest[20] = '\0';
	scanf("%s",test);
	SHA1(test, 6, digest);
	R(test, digest);
	//printf("%s\n", test);
	/*R(rainbow, digest);
	printf("%s\n", rainbow);
	SHA1(rainbow, 6, digest);*/
	search(digest, first);
	

 	//return 0;
}