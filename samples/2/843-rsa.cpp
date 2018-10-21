#include<stdio.h>
#include<openssl/bn.h>
#include<time.h>
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
void SquareMod(BIGNUM *result, BIGNUM *x, BIGNUM *a, BIGNUM *n);
BIGNUM *exgcd(BIGNUM *result, BIGNUM *a, BIGNUM *b);
void MultiInverse(BIGNUM *result, BIGNUM *b1, BIGNUM *a1);
void Chinese_Remainder(BIGNUM *result, BIGNUM *x, BIGNUM *a, BIGNUM *p, BIGNUM *q, BIGNUM *n);
void Montgomerie(BIGNUM *result, const BIGNUM *a, const BIGNUM *b, const BIGNUM *n);
int Miller_Rabin(BIGNUM* n);
int main()
{
	
	//Rsa参数生成
	BIGNUM *One = BN_new(), *a = BN_new(), *b = BN_new(), *n = BN_new(), *p = BN_new(), *q = BN_new();
	BIGNUM *temp = BN_new(),  *p1 = BN_new(), *q1 = BN_new(), *n1 = BN_new(), *x= BN_new(),*y = BN_new();
	BN_CTX *ctx = BN_CTX_new();
	double start, finish;
B:  BN_rand(p, 1024, 0, 0);
	while(!Miller_Rabin(p))  BN_rand(p, 1024, 0, 0);//产生大素数p
	BN_rand(q, 1024, 0, 0);
	while (!Miller_Rabin(q))  BN_rand(q, 1024, 0, 0);//产生大素数q
	if (BN_cmp(p, q) == 0)  goto B;//p=q重新生成
	BN_mul(n, p, q, ctx);//n=pq
	BN_one(One);
	BN_sub(p1, p, One);//p1=p-1
	BN_sub(q1, q, One);//q1=q-1
	BN_mul(n1, p1, q1, ctx);//n1=(p-1)(q-1)
C: BN_rand_range(b, n1);
	if(BN_is_one(b)) goto C;
	exgcd(temp,b, n1);
	if (!BN_is_one(temp))   goto C;//是的b与n1互素
	MultiInverse(a,b, n1);//a=b^-1mod n1
	printf("p:%s\n\n", BN_bn2dec(p));
	printf("q:%s\n\n", BN_bn2dec(q));
	printf("n:%s\n\n", BN_bn2dec(n));
	printf("a:%s\n\n", BN_bn2dec(a));
	printf("b:%s\n\n", BN_bn2dec(b));
	//RSA实现，公钥（n,b),私钥（p,q,a)
	char str1[] = "1234567898765432123456789123456789";
	BN_dec2bn(&x, str1);
	BN_mod_exp(y, x, b, n,ctx);//y=x^a mod n
	printf("x:%s\n\n", BN_bn2dec(x));
	printf("y:%s\n\n", BN_bn2dec(y));
	//库函数实现
	start = clock();//取开始时间
	BN_mod_exp(temp,y,a,n,ctx);//x=y^a mod n
	finish = clock();//取结束时间
	printf("库函数解密用时：%f seconds\n", (finish - start) / CLOCKS_PER_SEC);
	printf("x:%s\n\n", BN_bn2dec(temp));
	//模重复平方实现
	start = clock();//取开始时间
	 SquareMod(temp,y, a, n);//x=y^a mod n
	finish = clock();//取结束时间
	printf("模重复平方解密用时：%f seconds\n", (finish - start) / CLOCKS_PER_SEC);
	printf("x:%s\n\n", BN_bn2dec(temp));
	//中国剩余定理实现
	start = clock();//取开始时间
	Chinese_Remainder(temp,y,a,p,q,n);//x=y^a mod n
	finish = clock();//取结束时间
	printf("中国剩余定理解密用时：%f seconds\n", (finish - start) / CLOCKS_PER_SEC);
	printf("x:%s\n\n", BN_bn2dec(temp));
	//蒙哥马利实现
	start = clock();//取开始时间
	Montgomerie(temp, y,a, n);//x=y^a mod n
	finish = clock();//取结束时间
	printf("蒙哥马利算法解密用时：%f seconds\n", (finish - start) / CLOCKS_PER_SEC);
	printf("x:%s\n\n", BN_bn2dec(temp));
	getchar();
	
	/*
	//米勒拉宾测试
	BIGNUM *x = BN_new(), *a = BN_new(), *n = BN_new(), *p = BN_new(), *q = BN_new();
	unsigned long A = 17, B;
	char *s = NULL;
	BN_set_word(x, A);
	B= Miller_Rabin(x);
	printf("%d", B);
	getchar();*/

	/*
	//中国剩余定理测试
	BIGNUM *x = BN_new(), *a = BN_new(), *n = BN_new(), *p= BN_new(), *q= BN_new(),*r;
	unsigned long A = 3, B =6, C = 35,D=5,E=7;
	char *s = NULL;
	BN_set_word(x, A);
	BN_set_word(a, B);
	BN_set_word(n, C);
	BN_set_word(p, D);
	BN_set_word(q, E);
	r = Chinese_Remainder(x,a,p,q,n);
	s = BN_bn2dec(r);
	printf("%s", s);
	getchar();
	BN_free(r);
*/
	/*
	BIGNUM *x = BN_new(), *a = BN_new(), *n = BN_new(), *r;
	unsigned long A =4 , B = 7, C = 3;
	char *p = NULL;
	BN_set_word(x, A);
	BN_set_word(a, B);
	BN_set_word(n, C);
	r = MultiInverse(x,a);
	p = BN_bn2dec(r);
	printf("%s", p);
	getchar();
	BN_free(r); */

	/*
	//测试模重复平方
	BIGNUM *x = BN_new(), *a = BN_new(), *n = BN_new(),*s= BN_new();
	unsigned long A=4, B=4, C=7;
	char *p=NULL;
	BN_set_word(x, A);
	BN_set_word(a, B);
	BN_set_word(n, C);
	Montgomerie(s,x, a, n);
	p = BN_bn2dec(s);
	printf("%s", p);
	getchar();
	BN_free(s);
	*/

}

void SquareMod(BIGNUM *result,BIGNUM *x, BIGNUM *a, BIGNUM *n)//模重复平方
{
	BIGNUM *z = BN_new(), *a1 = BN_new(),*base= BN_new();
	BN_CTX *ctx = BN_CTX_new();
	BN_one(z);
	int length, i;
	length = BN_num_bits(a); //返回a的2进制位数
	BN_copy(a1, a);
	BN_copy(base, x);
	for (i = 0; i < length; i++)
	{
		if (BN_is_odd(a1)==1)//第i位是否为1
		{
			BN_mod_mul(z, z, base, n, ctx);
		}
		BN_mod_sqr(base, base, n, ctx);
		BN_rshift1(a1, a1);
	}
	BN_copy(result, z);
	BN_CTX_free(ctx);
	BN_free(a1);
	BN_free(z);
	BN_free(base);
}

BIGNUM *exgcd(BIGNUM *result,BIGNUM *a, BIGNUM *b)//扩展欧几里得求最大公因数
{
	BIGNUM *r=BN_new();//不会自动回收
	BN_CTX *ctx = BN_CTX_new();
	if (BN_is_zero(b))
	{
		BN_copy(result, a);
		return a;
	}
	BN_nnmod(r, a, b, ctx);
	exgcd(r,b, r);
	BN_copy(result, r);
	return r;
	BN_free(r);
}
void MultiInverse(BIGNUM *result, BIGNUM *b1, BIGNUM *a1)//扩展欧几里得求逆，b模a的逆
{
	BIGNUM *r = BN_new(), *t0 = BN_new(), *t = BN_new(),*temp= BN_new(),*q = BN_new(),*A= BN_new();
	BIGNUM *a = BN_new(), *b = BN_new();
	BN_copy(a, a1);
	BN_copy(b, b1);
	BN_CTX *ctx = BN_CTX_new();
	BN_one(t);//t=1
	BN_zero(t0);//t0=0
	BN_div(q, r, a, b, ctx);//q=a/b,r=a-qb
	BN_copy(A, a);
	while (!BN_is_zero(r))
	{
		BN_mul(temp, q, t, ctx);//temp=qt
		BN_mod_sub(temp, t0, temp, A, ctx);//temp=(t0-qt)mod a
		BN_copy(t0, t);//t0=t
		BN_copy(t, temp);//t=temp
		BN_copy(a, b);//a=b
		BN_copy(b, r);//b=r
		BN_div(q, r, a, b, ctx);//q=a/b,r=a-qb
	}
	if (BN_is_one(b)) BN_copy(result,t);
	else printf("error!");
	BN_free(t0);
	BN_free(r);
	BN_free(temp);
	BN_free(a);
	BN_free(b);
	BN_free(A);
	BN_free(q);
	BN_free(t);
}

void Chinese_Remainder(BIGNUM *result,BIGNUM *x, BIGNUM *a, BIGNUM *p, BIGNUM *q, BIGNUM *n)
{
	BIGNUM *M1 = BN_new(), *M2 = BN_new(), *M_1 = BN_new(), *M_2 = BN_new(), *b1=BN_new(), *b2 = BN_new();
	BN_CTX *ctx = BN_CTX_new();
	BN_mod_exp(b1, x, a, p, ctx);//b1=x^a(modp)
	BN_copy(M1, q);//M1=M/M2=q
	MultiInverse(M_1,M1, p);
	BN_mod_exp(b2, x, a, q, ctx);//b2=x^a(modq)
	BN_copy(M2, p);//M2=p
	 MultiInverse(M_2 ,M2, q);
	BN_mul(M_1, M_1, M1, ctx);//M-1=M1*M_1
	BN_mul(b1, b1, M_1, ctx);//b1=b1*M1*M_1
	BN_mul(M_2, M_2, M2, ctx);//M-2=M2*M_2
	BN_mul(b2, b2, M_2, ctx);//b2=b2*M2*M_2
	BN_mod_add(b1, b1, b2, n, ctx);
	BN_copy(result, b1);
	BN_free(M1);
	BN_free(M2);
	BN_free(M_1);
	BN_free(M_2);
	BN_free(b1);
	BN_free(b2);
}

int Miller_Rabin(BIGNUM* n)
{
	BIGNUM *n1 = BN_new(),*m = BN_new(),*temp=BN_new(),*a = BN_new(), *b = BN_new(), *One = BN_new();
	BN_CTX *ctx = BN_CTX_new();
	unsigned long i=2, k;
	BN_set_word(temp, i);//temp=2
	BN_one(One);
	BN_sub(n1, n, One);//n1=n-1
	BN_div(m, NULL, n1, temp, ctx);//m=(n-1)/2
	i = 1;
	if (BN_is_odd(n))//偶数不满足大素数的条件
	{
		while (!BN_is_odd(m))//当m为奇数的时候停止
		{
			BN_div(m, NULL, m, temp, ctx);//m=m/2
			i++;
		}
		k = i;//n-1=2^k*m
	}
	else return 0;
   BN_rand_range(a, n);//随机选取1<=a<=n-1
	BN_mod_exp(b, a, m, n, ctx);//b=a^m(mod n)
	BN_mod_sub(temp, b, One, n, ctx);//temp=b-1mod n
	if (BN_is_zero(temp)) return 1;//返回素数
	for (i = 0; i < k; i++)
	{
		BN_mod_add(temp, b, One, n, ctx);//temp=b+1mod n
		if (BN_is_zero(temp)) return 1;//返回素数
		else BN_mod_sqr(b, b, n, ctx);//b=b^2mod n
	}
	BN_free(n1);
	BN_free(m);
	BN_free(temp);
	BN_free(a);
	BN_free(b);
	BN_free(One);
	return 0;//返回和数
}

void Montgomerie(BIGNUM *result, const BIGNUM *a, const BIGNUM *b, const BIGNUM *n) {
	BIGNUM *t = BN_new(),*r1 = BN_new(), *r = BN_new();
	BIGNUM *tmp = BN_new(), *p = BN_new(), *A = BN_new();
	BN_CTX *ctx = BN_CTX_new();

	int len = BN_num_bits(n);
	BN_one(tmp);
	BN_set_bit(r, len);//r=2^len
	BN_mod_inverse(r1,r,n,ctx);
	BN_mod_mul(p, r, tmp, n,ctx);//p=mont(1)
	BN_mod_mul(A, r, a, n,ctx);//A=mont(a)
	BN_copy(t, b);
	while (!BN_is_zero(t))
	{
		if (BN_is_odd(t) == 1) {
			BN_mod_mul(tmp, p, A, n, ctx);
			BN_mod_mul(p, tmp, r1, n, ctx);//Prod=MontMult(Prod,A)
		}
		BN_mod_mul(tmp, A, A, n, ctx);
		BN_mod_mul(A, tmp, r1, n, ctx); //A = MontMult(A, A);
		BN_rshift1(t,t);//e=e>>1;
	}
	BN_mod_mul(result, p, r1, n, ctx);  //return  MontInv(Prod)
	BN_CTX_free(ctx);
	BN_free(tmp);
	BN_free(r1);
	BN_free(r);
	BN_free(t);
	BN_free(p);
	BN_free(A);
}