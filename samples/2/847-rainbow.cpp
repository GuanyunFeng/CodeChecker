// rainbow.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"
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

#pragma warning(disable:4996)
#define T 10//生成彩虹链的数目
typedef struct rainbowl
{
	struct rainbowl *pre;
	unsigned char head[7];//链首
	unsigned char last[7];//链尾
	struct rainbowl *next;
}L;


unsigned char compute(unsigned char sum)//计算Simp函数值
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
	return final;
}



void Simp(unsigned char* rainbow, unsigned char* head)//约简函数
{
	unsigned char temp[7];
	int i;
	temp[0] = head[0] + head[19] + head[6] + head[13];
	temp[1] = head[1] + head[18] + head[7] + head[12];
	temp[2] = head[2] + head[17] + head[8];
	temp[3] = head[3] + head[16] + head[9];
	temp[4] = head[4] + head[15] + head[10];
	temp[5] = head[5] + head[14] + head[11];
	for (i = 0; i < 6; i++)
	{
		rainbow[i] = compute(temp[i]);
	}
	rainbow[6] = '\0';
	return;
}



unsigned char randchar(void)//随机生成单字符
{
	unsigned char t;
	unsigned int a, b;
	a = rand()%2;
	if (a == 0)
	{
		t = 48 + rand() % 10;;
	}
	if (a == 1)
	{
		t = 97 + rand() % 26;
	}
	return t;
}


void lian(unsigned char* head, unsigned char * rainbow)//链生成
{
	unsigned char hash[21];
	size_t n;//传输信息的长度
	for (size_t i = 0; i < 6; i++)//生成随机明文
	{
		head[i] = randchar();
	}
	head[6] = '\0';
	n = strlen((const char*)head);//输入信息的长度
	for (int i = 0; i < 10; i++)
	{
		SHA1(head, n, hash);
		hash[20] = '\0';
	//printf("%s\n", hash);
		Simp(rainbow, hash);
	}
	
	return;
}
void rainbowl_generate(struct rainbowl * first)//参数为首链
{
	int i;
	first->pre = NULL;
	first->next = NULL;
	struct  rainbowl* pos = first;
	lian(first->head, first->last);
	for (i = 0; i < T; i++)
	{
		struct  rainbowl* newprior = (struct rainbowl*)malloc(sizeof(rainbowl));
		newprior->pre = pos;
		newprior->next = NULL;
		pos->next = newprior;
		lian(newprior->head, newprior->last);
		pos = newprior;
	}
	return;
}

struct rainbowl *hit(unsigned char* rainbow, struct rainbowl * first)
{
	struct rainbowl *pos = first;
	do
	{
		if (!strcmp((const char*)rainbow, (const char*)pos->last))
		{
			printf("符合\n");
			return pos;
		}
		else
		{
			pos = pos->next;
		}
	} while (pos->next != NULL);
	return NULL;
}

void search(unsigned char* head, struct rainbowl * first)
{
	unsigned char temp[7];
	unsigned char head[21];
	struct rainbowl *pos;
	Simp(temp, head);
	for (int i = 0; i < 10; i++)
	{
		pos = hit(temp, first);
		if (pos != NULL)
		{
			printf("位于彩虹链第%d段\n",10-i);
			SHA1(pos->head, 6, head);
			for (int j = 0; j < 10-i; j++)
			{
				head[20] = '\0';
				Simp(temp, head);
				SHA1(temp, 6, head);
			}
			printf("%s\n", temp);
			return;
		}
		SHA1(temp, 6, head);
		head[20] = '\0';
		Simp(temp, head);
	}
	printf("Not Find!\n");
	return;
}

int main()
{
	srand((unsigned)(time(NULL)));
	unsigned char a;
	unsigned char store[13];
	
	struct rainbowl* first = (struct rainbowl*)malloc(sizeof(rainbowl));//初始化
	first->pre = NULL;
	first->next = NULL;
	
	rainbowl_generate(first);
	
	FILE* fp;
	if ((fp = fopen("D:\\123.txt", "r")) == NULL) {
		printf("can not open file!\n");
		getchar();
		exit(0);
	}
	struct rainbowl* pos = first;

	while (!feof(fp))
	{
		fscanf(fp, "%s", store);
		store[12] = '\0';
		strncpy((char*)pos->head, (const char*)store, 6);
		strncpy((char*)pos->last, (const char*)store + 6, 6);
		//printf("%s  %s\n", pos->head,pos->last);
		pos = pos->next;
	}

	unsigned char test[7] = "5a3x48";
	unsigned char rainbow[7];
	unsigned char hash[21];

	SHA1(test, 6, hash);
	Simp(rainbow, hash);
	search(hash, first);
	printf("%s", test);
	getchar();
	return 0;
}


