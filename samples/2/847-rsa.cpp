// Con1.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <time.h>

#define TESTCOUNT 100

void b16c(char* a, int* b,int L)
{
	int L16=L/4;
	char c;
	int i, j,k;
	if (a[0] == '0') L16++;
	if (L % 4 == 0) L16--;
	
		
		for (i = 0; i < L16+1; i++)
		{
				k = L16 - i;
			if (a[k] >= '0'&&a[k] <= '9')
				c = a[k] - 48;
			else if (a[k] >= 'A'&&a[k] <= 'Z')
				c = a[k] - 55;
			else if (a[k] >= 'a'&&a[k] <= 'z')
				c = a[k] - 87;

			for (j = 0; j < 4; j++)
			{
				k = 4 * i + j;
				if (k >L ) break;
				b[k] = (int)c % 2;
				c = c / 2;
			}
		}
}

void mchong(BIGNUM* r, BIGNUM* a, BIGNUM* b, BIGNUM* n, BN_CTX *ctx)
{
	BIGNUM *d = BN_new();
	int i,len;
	int bit[3000];
	char* c = NULL;

	BN_copy(d, a);
	len = BN_num_bits(b);
	c = BN_bn2hex(b);
	b16c(c, bit, len);
	BN_one(r);
	for (i = 0; i<len; i++)
	{
		if (bit[i])
		{
			BN_mod_mul(r, r, d, n,ctx);
		}
		BN_mod_mul(d, d, d, n,ctx);
	}

}

void china(BIGNUM*r, BIGNUM* a, BIGNUM* b, BIGNUM* n,BIGNUM* p,BIGNUM* q, BN_CTX *ctx)
{
	BIGNUM *p1 = BN_new();
	BIGNUM *q1 = BN_new();
	BIGNUM *b1 = BN_new();
	BIGNUM *b2 = BN_new();
	BIGNUM *a1 = BN_new();
	BIGNUM *a2 = BN_new();
	BN_one(b1);
	BN_sub(p1, p, b1);
	BN_sub(q1, q, b1);
	BN_mod(b1, b, p1, ctx);
	BN_mod(b2, b, q1, ctx);
	BN_mod(a1, a, p, ctx);
	BN_mod(a2, a, q, ctx);

	mchong(p1, a1, b1, p, ctx);
	mchong(q1, a2, b2, q, ctx);

	BN_mod_inverse(b2, p, q, ctx);

	BN_sub(b1, q1, p1);
	BN_mod_mul(b2, b1, b2, q, ctx);
	BN_mul(a1, b2, p, ctx);
	BN_add(r, p1, a1);
}

void montmul(BIGNUM* r, BIGNUM* a, BIGNUM* b, BIGNUM* n, BIGNUM* w, BN_CTX *ctx)
{
	BIGNUM *temp = BN_new();
	BIGNUM *temp0 = BN_new();
	BIGNUM *num = BN_new();
	BIGNUM *D = BN_new();
	BN_one(D);
	BN_lshift(D, D, 32);//=2^32
	int i = 0;
	int k = 0;
	k = 2048 / 32;
	BN_mul(temp, a, b, ctx);
	for (i = 0; i < k; i++)
	{
		BN_copy(temp0, temp);
		BN_mask_bits(temp0, 32);
		BN_mod_mul(num, w, temp0, D,ctx);
		BN_mul(num, num, n, ctx);
		BN_add(temp, temp, num);
		BN_rshift(temp, temp, 32);
	}
	if (BN_cmp(temp, n) >= 0)
		BN_sub(temp, temp, n);
	BN_copy(r, temp);
}

void montexp(BIGNUM* r, BIGNUM* a, BIGNUM* b, BIGNUM* n, BN_CTX *ctx)
{
	BIGNUM *d = BN_new();
	BIGNUM *e = BN_new();
	int i,j, len;
	int bit[3000];
	char* c = NULL;
	len = BN_num_bits(n);
	j = len/ 32 + 1;
	BIGNUM *R = BN_new();
	BIGNUM *K = BN_new();
	BIGNUM *N1 = BN_new();
	BIGNUM *N32 = BN_new();
	BIGNUM *R2 = BN_new();
	BN_set_word(R, 1);
	BN_set_word(N32, 1);
	BN_lshift(N32, N32, 32);//N32=2^32
	BN_lshift(R, R, 32 * j);

	BN_mod_inverse(N1, n, N32, ctx);
	BN_sub(N1, N32, N1);//= -N^-1

	BN_mul(R2, R, R, ctx);
	BN_mod(R2, R2, n, ctx);//r^2

	BN_one(K);

	montmul(d, R2, a, n, N1,ctx);
	montmul(e, R2, K, n, N1, ctx);
	len = BN_num_bits(b);
	c = BN_bn2hex(b);
	b16c(c, bit, len);
	for (i = 0; i<len; i++)
	{
		if (bit[i])
		{
			montmul(e, e, d, n, N1,ctx);
		}
		montmul(d, d, d, n, N1,ctx);
	}
	montmul(r, e, K, n, N1, ctx);
}
int main()
{
	BIGNUM *p = BN_new();
	BIGNUM *q = BN_new();
	BIGNUM *pq1 = BN_new();
	BIGNUM *m = BN_new();
	BIGNUM *n = BN_new();
	BIGNUM *r = BN_new();
	BIGNUM *a = BN_new();
	BIGNUM *b = BN_new();
	BIGNUM *x = BN_new();
	BIGNUM *y = BN_new();
	BIGNUM *xxx = BN_new();
	BN_CTX *ctx = BN_CTX_new();
	char *c = "abc";
	unsigned char *s=NULL;
	int len = 0;
	int i;
	time_t t1;
	time_t t2;
	BN_one(xxx);
	
	BN_generate_prime(p, 1024,NULL,NULL,NULL,NULL,NULL);
	BN_generate_prime(q, 1024, NULL, NULL, NULL, NULL, NULL);
	BN_mul(n, p, q, ctx);
	
	BN_sub(a, p, xxx);
	BN_sub(b, q, xxx);
	BN_mul(pq1, a, b, ctx);

	BN_dec2bn(&m, "65537"); //65537，公钥
	c = BN_bn2hex(m);
	printf("公钥 %s\n", c);
	BN_mod_inverse(r, m,pq1,ctx);
	BN_dec2bn(&x, "00544");//明文
	c = BN_bn2hex(x);
	printf("明文 %s\n", c);


	b = x;
	BN_one(a);	
	BN_mod_exp(a, b, m, n,ctx);//利用标准库函数进行加密解密，用于对比
	c = BN_bn2hex(a);
	printf("encrpyt:  %s\n", c);

	BN_mod_exp(a, a, r, n, ctx);
	c = BN_bn2hex(a);
	printf("encode:  %s\n", c);


	mchong(a, x, m, n,ctx);//模重复加密
	y = a;

	
	t1 = clock();
	for (i = 0; i < TESTCOUNT; i++)
	{
	china(b, a, r, n, p, q, ctx);
	}
	t2 = clock();
	printf("T:%lf\n", (double)(t2 - t1) / TESTCOUNT);
	c = BN_bn2hex(b);
	printf("china:   %s\n\n", c);
	BN_one(b);


	t1 = clock();
	for (i = 0; i < TESTCOUNT; i++)
	{
	montexp(b,y,r,n,ctx);
	}
	t2 = clock();
	printf("T:%lf\n", (double)(t2 - t1) / TESTCOUNT);
	c = BN_bn2hex(b);
	printf("mont:   %s\n\n", c);



	t1 = clock();
	for (i = 0; i < TESTCOUNT; i++)
	{
		mchong(b, y, r, n, ctx);
	}
	t2 = clock();
	printf("T:%lf\n", (double)(t2 - t1) / TESTCOUNT);

	c=BN_bn2hex(b);
	printf("模重复 %s\n\n", c);

	
	
	OPENSSL_free(c);
	BN_CTX_free(ctx);
	getchar();

    return 0;
}

