#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "openssl/bn.h"
#include <openssl/sha.h>
#include <time.h>

void R(unsigned char* rainbow, unsigned char* hash);

unsigned char compute(unsigned char sum)
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


unsigned char rand_generate(unsigned int l, unsigned int h)
{
	unsigned char t;
	t = rand() % (h - l + 1) + l;
	return t;
}
unsigned char rain_generate(void);

typedef struct rainbowlist
{
	struct rainbowlist *pre;
	unsigned char plain[7];
	unsigned char rainbow[7];
	struct rainbowlist *next;
}L;

#define T 100000
int i1 = 0;

void node_generate(unsigned char* plain, unsigned char * rainbow)
{
	unsigned char digest[21];
	size_t n;
	for (size_t i = 0; i < 6; i++)
	{
		plain[i] = rain_generate();
	}
	plain[6] = '\0';
	for (int i = 0; i < 7; i++)
	{
		rainbow[i] = plain[i];
	}
	n = strlen((const char*)plain);
	int target;
	for (target = 0; target < 100; target++)
	{
		SHA1(rainbow, 6, digest);
		digest[20] = '\0';
		R(rainbow, digest);
	}
	return;
}
void rainbowlist_generate(struct rainbowlist * first)
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

struct rainbowlist *hit(unsigned char* rainbow, struct rainbowlist * first);

void search(unsigned char* hash, struct rainbowlist * first);

int main()
{


	srand(time(0));
	unsigned char a;
	struct rainbowlist* first = (struct rainbowlist*)malloc(sizeof(rainbowlist));
	first->pre = NULL;
	first->next = NULL;
	rainbowlist_generate(first);


	FILE* fp = fopen("data.txt", "w");;
	struct rainbowlist* pos = first;
	 do
	{
	    fprintf(fp, "%s ", pos->plain);
	    fprintf(fp, "%s", pos->rainbow);
	    fprintf(fp, "\n");
	    pos = pos->next;
	} while (pos->next!=NULL);
	fclose(fp);


	unsigned char test[6] = { 0 };
	unsigned char rainbow[7];
	unsigned char digest[21];
	digest[20] = '\0';
	scanf("%s",test);
	SHA1(test, 6, digest);
	R(test, digest);
	search(digest, first);

}

unsigned char rain_generate(void)
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
	return t;
}

void R(unsigned char* rainbow, unsigned char* hash)
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

struct rainbowlist *hit(unsigned char* rainbow, struct rainbowlist * first)
{
	i1 = 1;
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
	int target = 100;
	while (1) {
		pos = hit(temp1, first);
		if (pos != NULL)
		{
			SHA1(pos->plain, 6, digest);
			if (target == 1)
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