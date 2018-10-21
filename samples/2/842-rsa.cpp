#include "stdafx.h"
#include "openssl/bn.h"
#include <time.h>
#define bits 256

//链接路径不在工作目录下，需要将库文件加入指定位置

BIGNUM *modresqr(BIGNUM *d, BIGNUM *cipher, BIGNUM*n)
{
	BIGNUM *plain = BN_new();
	BIGNUM *res = BN_new();
	BIGNUM *two = BN_new();
	BIGNUM *rem = BN_new();
	BIGNUM *cipher0 = BN_new();
	BN_CTX *ctx = BN_CTX_new();

	BN_one(plain);
	BN_copy(cipher0, cipher);
	BN_set_word(two, 2);
	do
	{
		BN_mod(res, d, two, ctx);
		if (BN_is_one(res))
			BN_mod_mul(plain, plain, cipher0, n, ctx);
		BN_mod_mul(cipher0, cipher0, cipher0, n, ctx);
		BN_div(d, rem, d, two, ctx);
	} while (!(BN_is_zero(d)));
	return plain;
}

BIGNUM* mont1(BIGNUM* a, BIGNUM* n)
{
	BIGNUM* r = BN_new();
	BIGNUM* m = BN_new();;
	BN_CTX *ctx = BN_CTX_new();
	BN_clear(r);
	BN_clear(m);
	BN_set_bit(r, 512);
	BN_mod_mul(m, a, r, n, ctx);
	BN_free(r);
	BN_CTX_free(ctx);
	return m;
}

int primetest(BIGNUM *a);

int Get_mod_inverse(BIGNUM * R, BIGNUM * x, BIGNUM * n, BN_CTX *ctx);

BIGNUM* zhongguoshengyu(BIGNUM* cipher, BIGNUM* d, BIGNUM* n, BIGNUM* p, BIGNUM* q)
{
	BIGNUM *psub1 = BN_new();
	BIGNUM *qsub1 = BN_new();
	BIGNUM *dp = BN_new();
	BIGNUM *dq = BN_new();
	BIGNUM *p0 = BN_new();
	BIGNUM *q0 = BN_new();
	BIGNUM *p0q = BN_new();
	BIGNUM *q0p = BN_new();
	BIGNUM *xp = BN_new();
	BIGNUM *xq = BN_new();
	BIGNUM *s1 = BN_new();
	BIGNUM *s2 = BN_new();
	BIGNUM *decipher3 = BN_new();
	BN_CTX *ctx = BN_CTX_new();

	BN_copy(psub1, p);
	BN_copy(qsub1, q);
	BN_sub_word(psub1, 1);
	BN_sub_word(qsub1, 1);
	BN_mod(dp, d, psub1, ctx);
	BN_mod(dq, d, qsub1, ctx);
	BN_mod_inverse(p0, q, p, ctx);
	BN_mod_inverse(q0, p, q, ctx);
	BN_mul(p0q, p0, q, ctx);
	BN_mul(q0p, q0, p, ctx);
	BN_mod_exp(xp, cipher, dp, p, ctx);
	BN_mod_exp(xq, cipher, dq, q, ctx);
	BN_mul(s1, p0q, xp, ctx);
	BN_mul(s2, q0p, xq, ctx);
	BN_mod_add(decipher3, s1, s2, n, ctx);
	return decipher3;
}

int print(BIGNUM *n,const char * q)
{
	char *p;
	p = BN_bn2hex(n);
	printf(q);
	printf(" 0x%s\n", p);
	OPENSSL_free(p);
	return 1;
}

BIGNUM* montmult(BIGNUM* a, BIGNUM* b, BIGNUM* n)
{
	BIGNUM* a1 = BN_new();
	BIGNUM* b1 = BN_new();
	BIGNUM* t = BN_new();
	BIGNUM* t0 = BN_new();
	BIGNUM* in = BN_new();
	BIGNUM* r = BN_new();
	BIGNUM * temp = BN_new();
	BN_CTX *ctx = BN_CTX_new();
	BN_clear(r);
	BN_set_bit(r, 32);
	BN_copy(a1, a);
	BN_copy(b1, b);
	Get_mod_inverse(in, n, r, ctx);
	BN_sub(in, r, in);
	BN_mod_mul(t0, n, in, r, ctx);
	BN_mul(t, a1, b1, ctx);
	for (int i = 0; i < 16; i++)
	{
		BN_mod(t0, t, r, ctx);
		BN_mul(t0, in, t0, ctx);
		BN_mod(t0, t0, r, ctx);
		BN_mul(t0, n, t0, ctx);
		BN_add(t, t0, t);
		BN_div(t, temp, t, r, ctx);
	}
	BN_mod(t, t, n, ctx);
		BN_CTX_free(ctx);
		BN_free(r);
		BN_free(in);
		BN_free(t0);
		return t;
}

BIGNUM* montgeomery(BIGNUM* a, BIGNUM* e, BIGNUM* n)
{
	BIGNUM *prod = BN_new();
	BIGNUM *e1 = BN_new();
	BN_copy(e1, e);
	BN_CTX *ctx = BN_CTX_new();
	BIGNUM *a1=BN_new();
	prod = mont1((BIGNUM *)BN_value_one(),n);
	a1 = mont1(a,n);
	while (!BN_is_zero(e1))
	{
		if (BN_mod_word(e1, 2)==1) 
		{
			prod = montmult(prod, a1,n);
		}
		a1 = montmult(a1, a1,n);
		BN_rshift1(e1, e1);
	}
	BN_CTX_free(ctx);
	BN_free(a1);
	return montmult(prod, (BIGNUM *)BN_value_one(),n);
}

int main()
{
	unsigned long w = 1;
	int flag = 0;
	BIGNUM *p = BN_new();
	BIGNUM *q = BN_new();
	BIGNUM *temp = BN_new();
	BIGNUM *contrust = BN_new();
	BN_set_word(contrust, 1);
	while (flag == 0)
	{
		BN_rand(q, bits, NULL, NULL);
		if (primetest(q) == 1)
		{
			char *print;
			print = BN_bn2hex(q);
			printf("q = 0x%s\n\n", print);
			OPENSSL_free(print);
			flag = 1;
		}
	}
	flag = 0;
	while (flag == 0)
	{
		BN_rand(p, bits, NULL, NULL);
		if (primetest(p) == 1 )
		{
			char *print;
			print = BN_bn2hex(p);
			printf("p = 0x%s\n\n", print);
			OPENSSL_free(print);
			flag = 1;
		}
	}
		BIGNUM *n = BN_new();
		BN_CTX *ctx = BN_CTX_new();
		BN_mul(n, p, q, ctx);
		BIGNUM *e = BN_new();
		BIGNUM *b = BN_new();
		BIGNUM *d = BN_new();
	    w = 65537;
		BN_set_word(e, w);
		 w = 1;
		BN_set_word(b, w);
		
		BN_sub(p, p, b);
		BN_sub(q, q, b);
		BIGNUM *n1 = BN_new();
		BN_mul(n1, p, q, ctx);
		while (1)
		{
			BN_rand_range(e, n1);
			BN_gcd(temp, e, n1, ctx);
			if(BN_is_one(temp))
				break;
		}
		Get_mod_inverse(d, e, n1, ctx);
		char *print;
		print = BN_bn2hex(e);
		printf("e = 0x%s\n\n", print);
		print = BN_bn2hex(d);
		printf("d = 0x%s\n\n", print);

		print = BN_bn2hex(n);
		printf("n = 0x%s\n\n", print);
		BN_rand_range(b, n);
		print = BN_bn2hex(b);
		printf("m = 0x%s\n\n", print);

		clock_t start;
		clock_t end;

		start = clock();

		//中国剩余定理
		BN_add(p, p, (BIGNUM *)BN_value_one());
		BN_add(q, q, (BIGNUM *)BN_value_one());
		BIGNUM *c1;
		BIGNUM *c2;
		c1 = zhongguoshengyu(b, e, n, p, q);
		c2 = zhongguoshengyu(c1, d, n, p, q);
		print = BN_bn2hex(c1);
		printf("c = 0x%s\n\n", print);
		OPENSSL_free(print);
		print = BN_bn2hex(c2);
		printf("m = 0x%s\n\n", print);
		OPENSSL_free(print);

        //模重复平方
		c1 = modresqr(e,b,n);
		c2 = modresqr(d, c1, n);
		print = BN_bn2hex(c1);
		printf("c = 0x%s\n\n", print);
		print = BN_bn2hex(c2);
		printf("m = 0x%s\n\n", print);
		OPENSSL_free(print);

/*
		//蒙哥马利算法
		BIGNUM *c1;
		BIGNUM *c2;
		c1 = montgeomery(b, e,n);
		c2 = montgeomery(c1, d,n); 
		print = BN_bn2hex(c1);
		printf("c = 0x%s\n\n", print);
		OPENSSL_free(print);
		print = BN_bn2hex(c2);
		printf("m = 0x%s\n\n", print);
		OPENSSL_free(print);
*/

		end = clock();
		printf("%g ms\n\n", double(end - start));

    return 0;
}

int primetest(BIGNUM *a)
{
	if (BN_is_odd(a))
	{
		int i, flag;
		unsigned long w = 1;
		BN_CTX *ctx = BN_CTX_new();
		BIGNUM *r = BN_new();
		BIGNUM *r1 = BN_new();
		BIGNUM *b = BN_new();
		BIGNUM *dv = BN_new();
		BIGNUM *a1 = BN_new();
		BIGNUM *a2 = BN_new();
		BIGNUM *m = BN_new();
		BN_copy(a1, a);
		BN_set_word(b, w);
		BN_sub(a, a, b);
		BN_copy(a2, a);
		w = 2;
		BN_set_word(b, w);
		for (i = 0; 1; i++)
		{
			BN_div(dv, r, a, b, ctx);
			if (!BN_is_odd(dv))
			{
				BN_copy(a, dv);
			}
			else {
				break;
			}
		}
		flag = i;
		while (1)
		{
			BN_rand_range(m, a2);
			if (BN_cmp(m, a1) == -1)
			{
				BN_mod_exp(b, m, dv, a1, ctx);
				BN_mod(r, b, a1, ctx);
				BIGNUM *num1 = BN_new();
				BN_set_word(num1, 1);
				if (BN_cmp(r, num1) == 0)
				{
					BN_copy(a, a1);
					BN_CTX_free(ctx);
					BN_free(r);
					BN_free(r1);
					BN_free(b);
					BN_free(dv);
					BN_free(a1);
					BN_free(a2);
					BN_free(m);
					BN_free(num1);
					return 1;
				}
				else for (i = 0; i < flag; i++)
				{
					BN_mod(r, b, a1, ctx);
					if (BN_cmp(r, a2) == 0)
					{
						BN_copy(a, a1);
						BN_CTX_free(ctx);
						BN_free(r);
						BN_free(r1);
						BN_free(b);
						BN_free(dv);
						BN_free(a1);
						BN_free(a2);
						BN_free(m);
						BN_free(num1);
						return 1;
					}
					BN_mod_sqr(b, b, a1, ctx);
				}
				BN_copy(a, a1);
				BN_CTX_free(ctx);
				BN_free(r);
				BN_free(r1);
				BN_free(b);
				BN_free(dv);
				BN_free(a1);
				BN_free(a2);
				BN_free(m);
				BN_free(num1);

				return 0;
			}
		}
	}
	else return 0;
}

int Get_mod_inverse(BIGNUM * R, BIGNUM * x, BIGNUM * n, BN_CTX *ctx)
{
	BIGNUM *a0 = BN_dup(x), *b0 = BN_dup(n), *t0 = BN_new(), *t = BN_new(), *s0 = BN_new(), *s = BN_new(), *q = BN_new(), *r = BN_new();
	BIGNUM *temp = BN_new(), *ZERO = BN_new();
	BN_zero(s);
	BN_zero(t0);
	BN_zero(ZERO);
	BN_one(t);
	BN_one(s0);
	BN_div(q, r, a0, b0, ctx);
	while (BN_cmp(r, ZERO) > 0) {
		BN_mul(temp, q, t, ctx);
		BN_sub(temp, t0, temp);
		BN_copy(t, temp);
		BN_mul(temp, q, s, ctx);
		BN_sub(temp, s0, temp);
		BN_copy(s0, s);
		BN_copy(s, temp);
		BN_copy(a0, b0);
		BN_copy(b0, r);
		BN_div(q, r, a0, b0, ctx);
	}
	BN_mod(R, s, n, ctx);
	if (BN_is_negative(R))BN_add(R, n, R);
	BN_free(a0);
	BN_free(b0);
	BN_free(t0);
	BN_free(t);
	BN_free(s0);
	BN_free(s);
	BN_free(q);
	BN_free(r);
	BN_free(temp);
	BN_free(ZERO);
	return 1;
}

