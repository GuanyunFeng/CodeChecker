#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gmp.h"
#include <string.h>

#define keylength 2048
#define pqlength 1024
#define elength 1024
#define plainelength 1024
#define montlength 32

void initpq(mpz_t p, mpz_t q, int bits);
void inite(mpz_t e, int bits);
int gcdone(const mpz_t m, const mpz_t n);
void initx(mpz_t x, int bits);
void modexp(mpz_t ciphertext, const mpz_t key_e, const mpz_t plaintext, const mpz_t n);
void montgomery(mpz_t ciphertext, const mpz_t key_e, const mpz_t plaintext, const mpz_t n);
void remainder(mpz_t ciphertext, const mpz_t p, const mpz_t q, const mpz_t d, const mpz_t plaintext, const mpz_t n);
void montmul(mpz_t T, mpz_t A, mpz_t B, mpz_t N);
//void initpara(mpz_t p, mpz_t q, mpz_t n, mpz_t f, mpz_t e, mpz_t d);

int main()
{
	mpz_t q, p, n, f, e, d, x, y, dp, dq, qInv;
	//mpz_init_set_ui(p, 9);
    //mpz_init_set_ui(q, 2);
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(f);
	mpz_init(d);
	mpz_init(x);
	//mpz_init_set_ui(x, 12345678);
	mpz_init(y);
	mpz_init(dp);
	mpz_init(dq);
	mpz_init(qInv);
	mpz_init(e);
	//mpz_div_ui(e, e, 2);
	//gmp_printf("e==%Zd\n\n", e);
	//mpz_fdiv_r(x, p, q);
	//gmp_printf("r=%Zd",x);

	clock_t start, finish;
	double during = 0.0;
	srand(time(0));

	int i;
	int op = 1;
	printf("1.生成参数\n");
	printf("2.加密\n");
	printf("3.解密\n");
	printf("0.退出\n");
	while (op)
	{
		printf("请输入操作数：\n");
		scanf("%d", &op);
		getchar();
		switch (op)
		{
		case 1:
			initpq(p,q,pqlength);
			//mpz_set(p, 28081);
			//mpz_set(q, 48787);
			//gmp_printf("p==%Zd\n\n", p);
			//gmp_printf("q==%Zd\n\n", q);
			mpz_mul(n, p, q);
			mpz_sub_ui(p, p, 1);
			mpz_sub_ui(q, q, 1);
			//mpz_invert(dp, e, p);
			//mpz_invert(dq, e, q);
			//gmp_printf("e==%Zd\n\n", e);
			//gmp_printf("p==%Zd\n\n", p);
			//gmp_printf("q==%Zd\n\n", q);
			mpz_mul(f, p, q);

			int flag = 1;
			while(flag)
            {
                inite(e, elength);
                if(gcdone(e, f))
                {
                    flag = 0;
                }
            }

			mpz_invert(d, e, f);
			mpz_add_ui(p, p, 1);
			mpz_add_ui(q, q, 1);
			//mpz_invert(qInv, q, p);
			gmp_printf("d==%Zd\n\n", d);
			gmp_printf("p==%Zd\n\n", p);
			gmp_printf("q==%Zd\n\n", q);
			gmp_printf("n==%Zd\n\n", n);
			gmp_printf("f==%Zd\n\n", f);
			gmp_printf("e==%Zd\n\n", e);


			system("pause");
			break;

		case 2:
			initx(x, plainelength);
			//mpz_set(x, 12345678);
			gmp_printf("明文为%Zd\n\n", x);

			printf("1.模重复平方\n");
			start = clock();


				modexp(y, e, x, n);

			finish = clock();
			during = (double)(finish - start);
			gmp_printf("\n密文为%Zd\n\n", y);
			printf("\n模重复平方加密用时为%f s\n\n", during / 1000);
			system("pause");



			/*printf("2.蒙哥马利算法\n");
			start = clock();

				//montgomery(y, e, x, n);

			finish = clock();
			during = (double)(finish - start);
			gmp_printf("\n密文为%Zd\n\n", y);
			printf("\n蒙哥马利算法加密用时为%f s\n\n", during / 1000);
			system("pause");*/
			break;

		case 3:
			printf("1.模重复平方\n");
			/*gmp_printf("d==%Zd\n\n", d);
			gmp_printf("p==%Zd\n\n", p);
			gmp_printf("q==%Zd\n\n", q);
			gmp_printf("n==%Zd\n\n", n);
			gmp_printf("f==%Zd\n\n", f);
			gmp_printf("e==%Zd\n\n", e);*/
			start = clock();

				modexp(x, d, y, n);

			finish = clock();
			during = (double)(finish - start);
			gmp_printf("\n明文为%Zd\n\n", x);
			printf("\n模重复平方解密用时为%f s\n\n", during /1000);
			system("pause");

			/*printf("2.蒙哥马利算法\n");
			start = clock();

				//montgomery(x, d, y, n);

			finish = clock();
			during = (double)(finish - start);
			gmp_printf("\n明文为%Zd\n\n", x);
			printf("\n蒙哥马利算法解密用时为%f s\n\n", during / 1000);
			system("pause");*/

			printf("2.中国剩余定理\n");
			/*mpz_invert(qInv, q, p);
			mpz_sub_ui(p, p, 1);
			mpz_sub_ui(q, q, 1);
			mpz_invert(dp, e, p);
			mpz_invert(dq, e, q);
			gmp_printf("dq=%Zd\n",dq);
			gmp_printf("dp=%Zd\n",dp);
			gmp_printf("qinv=%Zd\n",qInv);*/

			start = clock();

				remainder(x, p, q, d, y, n);

			finish = clock();
			during = (double)(finish - start);
			gmp_printf("\n明文为%Zd\n\n", x);
			printf("\n中国剩余定理解密用时为%f s\n\n", during / 1000);
			system("pause");

			break;

		case 0:



		default:
			break;
		}
	}

    return 0;
}

void initpq(mpz_t p, mpz_t q, int bits)
{
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	gmp_randseed_ui(randstate, time(NULL));

	mpz_urandomb(p, randstate, bits);
	mpz_urandomb(q, randstate, bits);

	if (mpz_even_p(p))
	{
		mpz_add_ui(p, p, 1);
	}

	if (mpz_even_p(q))
	{
		mpz_add_ui(q, q, 1);
	}

	while (!mpz_probab_prime_p(p, 25) > 0)
	{
		mpz_add_ui(p, p, 2);
	}

	while (!mpz_probab_prime_p(q, 25) > 0)
	{
		mpz_add_ui(q, q, 2);
	}
}

void inite(mpz_t e, int bits)
{
    gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	gmp_randseed_ui(randstate, time(NULL));

	mpz_urandomb(e, randstate, bits);
}

void initx(mpz_t x, int bits)
{
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	gmp_randseed_ui(randstate, time(NULL));

	mpz_urandomb(x, randstate, bits);
}

int gcdone(const mpz_t m, const mpz_t n)
{
    mpz_t a, b, c;
    mpz_init_set_ui(a, 1);
    mpz_init_set_ui(b, 1);
    mpz_init(c);
    mpz_mul(a, a, m);
    mpz_mul(b, b, n);
    while(mpz_cmp_ui(b, 0))
    {
        mpz_fdiv_r(c, a, b);
        mpz_set(a, b);
        mpz_set(b, c);
    }
    mpz_sub_ui(a, a, 1);
    if(mpz_cmp_ui(a, 0))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void modexp(mpz_t ciphertext, const mpz_t key_e, const mpz_t plaintext, const mpz_t n)
{
	//gmp_printf("mingwen:%Zd\n",plaintext);
	//gmp_printf("d:%Zd\n",key_e);
	//gmp_printf("n:%Zd\n",n);
	char exp[2048 + 10];
	mpz_get_str(exp, 2, key_e);
	mpz_t x, power;
	mpz_init(power);
	mpz_init_set_ui(x, 1);  // x = 1
	mpz_mod(power, plaintext, n); //power = plaintext mod n
	//gmp_printf("ciphermod=%Zd",power);

	for (int i = strlen(exp) - 1; i >= 0; i--)
	{
		if (exp[i] == '1')
		{
			mpz_mul(x, x, power);
			mpz_mod(x, x, n);   //x = x * power mod n
		}
		mpz_mul(power, power, power);
		mpz_mod(power, power, n);  //power = power^2 mod n
	}
	mpz_set(ciphertext, x); //返回结果
}

void montgomery(mpz_t ciphertext, const mpz_t key_e, const mpz_t plaintext, const mpz_t n)
{
	mpz_t temp;
	mpz_t i;
	mpz_t e;
	mpz_t res;
	mpz_t r2;
	mpz_init(r2);
	mpz_ui_pow_ui(r2, 2, plainelength);
	mpz_mul(r2, r2, r2);
	mpz_init_set_ui(i,1);
	mpz_init(e);
	mpz_set(e,key_e);
	mpz_init_set_ui(res,1);
	montmul(temp,i,r2,n);
	montmul(res,r2,plaintext,n);
	while (mpz_cmp_ui(e,0))//bb > 0
	{
		mpz_mod_ui(i, e, 2);// i = bb % 2;
		if (mpz_cmp_ui(i, 0))//i == 1时返回1
		{
                            //	res = res * temp % n;
			//mpz_mul(res, res, temp);
			//mpz_mod(res, res, n);
			montmul(res,res,temp,n);
		}
		                    //bb = bb >> 1;
		mpz_fdiv_q_ui(e, e, 2);
		                    //temp = temp * temp % n;
		//mpz_mul(temp, temp, temp);
		//mpz_mod(temp,temp, n);
		montmul(temp,temp,temp,n);
	}
	montmul(ciphertext,res,i,n);

}

/*void montmul(mpz_t T, mpz_t A, mpz_t B, mpz_t N)
{
    mpz_mul(T, A, B);
    mpz_mod(T, T, N);
}*/

void montmul(mpz_t T, mpz_t A, mpz_t B, mpz_t N)
{
	mpz_mul(T, A, B);
	//gmp_printf("AB=%Zd\n",T);
	mpz_t R, IN, bit32, ti, temp, T0;
	mpz_init(R);
	mpz_init(IN);
	mpz_init(bit32);
	mpz_init(ti);
	mpz_init(temp);
	mpz_init_set(T0, T);
	mpz_ui_pow_ui(R, 2, montlength);
	mpz_sub_ui(bit32, R, 1);
	mpz_invert(IN, N, R);
	//mpz_and(t0, bit32, T);
	//mpz_mul(temp, t0, IN);
	//mpz_mod(temp, temp, R);
	//mpz_mul(temp, temp, N);
	//gmp_printf("R=%Zd\n",R);
	//gmp_printf("bit32=%Zd\n",bit32);
	//gmp_printf("%IN=%Zd\n",IN);

	int i;
	for (i = 0; i < 64; i++)
	{
		mpz_and(ti, bit32, T);
		mpz_mul(temp, ti, IN);
		mpz_mod(temp, temp, R);
		mpz_mul(temp, temp, N);
		//mpz_tdiv_q_2exp(T0, T0, montlength);

		mpz_sub(T, T, temp);
		mpz_mod(T, T, N);
		mpz_tdiv_q_2exp(T, T, montlength);
		//gmp_printf("T=%Zd\n",T);
	}

	if (mpz_cmp(T, N) >= 0)
	{
		mpz_sub(T, T, N);
	}
	/*mpz_t R, bit32, IN, t, x0, yi, x, y, t0, u, s0, s1, s2;
	mpz_init(R);
	mpz_init(IN);
	mpz_init(bit32);
	mpz_init_set_ui(t, 0);
	mpz_init_set_ui(x, 1);
	mpz_init_set_ui(y, 1);
	mpz_init(t0);
	mpz_init(u);
	mpz_init(s0);
	mpz_init(s1);
	mpz_init(s2);
	mpz_init(yi);
	mpz_init(x0);
	mpz_mul(x, x, A);
	mpz_mul(y, y, B);
	mpz_ui_pow_ui(R, 2, montlength);
	mpz_sub_ui(bit32, R, 1);
	mpz_invert(IN, N, R);
	mpz_add(x0, x, bit32);

	int i;
	for (i = 0; i < 64; i++)
    {
        mpz_add(t0, t, bit32);
        mpz_add(yi, y, bit32);
        mpz_tdiv_q_2exp(y, y, montlength);
        mpz_mul(u, yi, x0);
        mpz_add(u, u, t0);
        mpz_mul(u, u, IN);
        mpz_mod(u, u, R);
        mpz_mul(s0, yi, x);
        mpz_mul(s1, u, N);
        mpz_add(s2, s0, s1);
        mpz_add(t, s2, t);
        mpz_tdiv_q_2exp(t, t, montlength);
    }

    mpz_set(T, t);*/
}

void remainder(mpz_t ciphertext, const mpz_t p, const mpz_t q, const mpz_t d, const mpz_t plaintext, const mpz_t n)
{
   mpz_t dp, dq, a1, a2, p1, q1, pn, qn, y;
   mpz_init(dp);
   mpz_init(dq);
   mpz_init(a1);
   mpz_init(a2);
   mpz_init(p1);
   mpz_init(q1);
   mpz_init(pn);
   mpz_init(qn);
   mpz_init(y);

   mpz_sub_ui(p1, p, 1);
   mpz_sub_ui(q1, q, 1);

   mpz_mod(dp, d, p1);
   mpz_mod(dq, d, q1);
   //gmp_printf("dp=%Zd\n",dp);
   //gmp_printf("dq=%Zd\n",dq);

   modexp(a1, dp, plaintext, p);
   modexp(a2, dq, plaintext, q);
   //gmp_printf("a1=%Zd\n",a1);
   //gmp_printf("a2=%Zd\n",a2);

   mpz_invert(pn, p, q);
   mpz_invert(qn, q, p);
   //gmp_printf("pn=%Zd\n",pn);
   //gmp_printf("qn=%Zd\n",qn);

   mpz_mul(a1, a1, q);
   mpz_mul(a1, a1, qn);
   mpz_mul(a2, a2, p);
   mpz_mul(a2, a2, pn);
   //gmp_printf("a1=%Zd\n",a1);
   //gmp_printf("a2=%Zd\n",a2);

   mpz_add(y, a1, a2);
   mpz_mod(y, y, n);
   //gmp_printf("y=%Zd\n",y);
   mpz_set(ciphertext, y);

}

/*void initpara(mpz_t p, mpz_t q, mpz_t n, mpz_t f, mpz_t e, mpz_t d)
{
	initpq(p, q, pqlength);
	mpz_mul(n, p, q);
	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);
	mpz_mul(f, p, q);
	mpz_invert(d, e, f);
}*/
