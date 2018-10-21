// rainbow.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/ssl.h>
#include<openssl/ec.h>
#include<openssl/ecdsa.h>
#include<openssl/objects.h>
#include<openssl/err.h>
#include<openssl/sha.h>
#include<openssl/comp.h>
#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <time.h>

#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#define T 100000//生成彩虹链的数目
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
	temp[2] = hash[2] + hash[17] + hash[8] ;
	temp[3] = hash[3] + hash[16] + hash[9] ;
	temp[4] = hash[4] + hash[15] + hash[10] ;
	temp[5] = hash[5] + hash[14] + hash[11] ;
	for ( i = 0; i < 6; i++)
	{
		rainbow[i] = compute(temp[i]);
	}
	rainbow[6] = '\0';
	return;
}

unsigned char rand_generate(unsigned int l,unsigned int h)//随机数生成,范围为l-h
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
	if (a==2)
	{
		t = rand_generate(97, 122);
	}
	/*if (a==3)
	{
		t = rand_generate(65, 90);
	}*/
	return t;
}


void node_generate(unsigned char* plain,unsigned char * rainbow)//结点（单条链）生成
{
	unsigned char digest[21];
	size_t n;//传输信息的长度
	for (size_t i = 0; i < 6; i++)//生成随机明文
	{
		plain[i] = rain_generate();
	}
	plain[6] = '\0';
	//printf("%s\n", plain);
	n = strlen((const char*)plain);//输入信息的长度，以字节为单位
	//生成的彩虹链长度为3
	SHA1(plain, n, digest);
	digest[20] = '\0';
	//printf("%s\n", digest);
	R(rainbow, digest);
	//printf("%s\n", rainbow);
	SHA1(rainbow, n, digest);
	digest[20] = '\0';
	//printf("%s\n", digest);
	R(rainbow, digest);
	//printf("%s\n", rainbow);
	SHA1(rainbow, n, digest);
	digest[20] = '\0';
	//printf("%s\n", digest);
	R(rainbow, digest);
	//printf("%s\n", rainbow);
	return;
}
void rainbowlist_generate(struct rainbowlist * first)//参数为彩虹链的首链
{  
	int i;
	first->pre = NULL;
	first->next = NULL;
	struct  rainbowlist* pos = first;
	node_generate(first->plain, first->rainbow);
	for ( i = 0; i < T; i++)
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
	struct rainbowlist *pos = first;
	do
	{
		if (!strcmp((const char*)rainbow, (const char*)pos->rainbow))
		{   
			printf("符合\n");
			return pos;
		}
		else
		{
			pos = pos->next;
		}
	}while (pos->next!=NULL);
	printf("不符合\n");
		return NULL;
}

void search(unsigned char* hash, struct rainbowlist * first)
{
	unsigned char temp[7];
	unsigned char digest[21];
	struct rainbowlist *pos;
	R(temp, hash);//对所要查询的哈希值进行约简
	pos = hit(temp, first);
	if (pos != NULL)//彩虹链第三段
	{
		printf("位于彩虹链第三段\n");
		SHA1(pos->plain, 6, digest);
		digest[20] = '\0';
		R(temp, digest);
		SHA1(temp, 6, digest);
		digest[20] = '\0';
		R(temp, digest);
		printf("%s\n", temp);
		return;
	}
	SHA1(temp, 6, digest);
	digest[20] = '\0';
	R(temp, digest);
	pos = hit(temp, first);
	if (pos != NULL)//彩虹链第二段
	{
		printf("位于彩虹链第二段\n");
		SHA1(pos->plain, 6, digest);
		digest[20] = '\0';
		R(temp, digest);
		printf("%s\n", temp);
		return;
	}
	SHA1(temp, 6, digest);
	digest[20] = '\0';
	R(temp, digest);
	pos = hit(temp, first);
	if (pos != NULL)//彩虹链第一段
	{
		printf("位于彩虹链第一段\n");
		printf("%s\n", pos->plain);
		return;
	}
	printf("Not Find!\n");
	return;
}

int main()
{  
	srand(time(0));//设定随机数种子
	unsigned char a;
	unsigned char store[13];
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
	FILE* fp;
	if ((fp = fopen("C:/Users/YoYoLuMing/Desktop/rainbowtable.txt", "r")) == NULL) { //打开要写入的文件
		printf("can not open file!\n");
		exit(0);
	}
	struct rainbowlist* pos = first;
   /* do
    { 
		fprintf(fp, "%s", pos->plain);
		fprintf(fp, "%s", pos->rainbow);
		fprintf(fp, "\n");
		pos = pos->next;
	} while (pos->next!=NULL);*/
	while (!feof(fp))
	{
		fscanf(fp, "%s",store);
		store[12] = '\0';
		strncpy((char*)pos->plain, (const char*)store, 6);
		strncpy((char*)pos->rainbow, (const char*)store+6, 6);
		//printf("%s  %s", pos->plain,pos->rainbow);
		pos = pos->next;
	}

	unsigned char test[7] = "830t9a";
	unsigned char rainbow[7];
	unsigned char digest[21];
	SHA1(test, 6, digest);
    //R(rainbow, digest);
	//printf("%s\n", rainbow);
	//SHA1(rainbow, 6, digest);
	search(digest, first);
	//pos=hit(test, first);
	//printf("%s", pos->plain);
	/*if (!strcmp((const char*)test,(const char*)first->rainbow))
	{
		printf("1");
	}*/
	return 0;
}