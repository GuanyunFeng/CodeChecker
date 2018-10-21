
//  main.cpp
//  mmx
//
//  Created by 李环 on 2018/7/23.
//  Copyright © 2018年 李环. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <gmp.h>

#define FALSE 0
#define OK 1
#define MONT_MAX 64

void GetInverse(mpz_t out, mpz_t in,mpz_t mod);
void ModExp(mpz_t result, const mpz_t exponent, const mpz_t base, const mpz_t n);
int IsPrime(mpz_t n);
mpz_t * GetRSAparm();
void ChineseMod(mpz_t plain, mpz_t cipher, mpz_t n,mpz_t d, mpz_t p, mpz_t q);
void Montgomery(mpz_t result, const mpz_t exponent, const mpz_t base, const mpz_t n) ;
void MontMul(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t r1, const mpz_t n);
void MontMulti(mpz_t T, const mpz_t x, const mpz_t y, const  mpz_t N, const mpz_t N_2);


int main(){
    printf("RSA密钥生成中...\n");
    clock_t start, finish;
    double duration;

    mpz_t * rsa = GetRSAparm();
    mpz_t fn,n,e,d;
    mpz_init(n);
    mpz_init(fn);
    mpz_init(d);
    mpz_init_set_ui(e, 65537);    //设置e为65537
    mpz_mul(n, rsa[0], rsa[1]);        //计算n=p*q
    mpz_sub_ui(rsa[0], rsa[0], 1);        //p=p-1
    mpz_sub_ui(rsa[1], rsa[1], 1);        //q=q-1
    mpz_mul(fn, rsa[0], rsa[1]);        //计算欧拉函数φ(n)=(p-1)*(q-1)
    mpz_add_ui(rsa[0],rsa[0],1);
    mpz_add_ui(rsa[1],rsa[1],1);        //【【【前面减完忘了加回来，debug三个小时，血的教训：）】】】
//    mpz_invert(d,e,fn);
//    gmp_printf("%ZX\n",d);
    GetInverse(d, e, fn);   //求e的逆d
//    gmp_printf("%ZX\n",d);
//    gmp_printf("%s (%ZX, %ZX)\n", "private key is:", n, d);//输出私钥(n, d)
    gmp_printf("P:\n%ZX\nQ:\n%ZX\n",rsa[0],rsa[1]);
    printf("生成完毕，请输入明文：\n");
    mpz_t plain, cipher,m;
    mpz_init(plain);
    mpz_init(cipher);
    mpz_init(m);
    gmp_scanf("%ZX",plain);
    start = clock();
    ModExp(cipher, e, plain, n);
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("seconds:%f\n",duration);
    gmp_printf("密文：%ZX\n",cipher);
//    mpz_t m,cipher,e,n,d,rsa[2];
//    mpz_init_set_ui(cipher,32748327948);
//    mpz_init_set_ui(n,1717);
//    mpz_init_set_ui(d,5);
//    mpz_init_set_ui(e,13);
//    mpz_init_set_ui(rsa[1],101);
//    mpz_init_set_ui(rsa[0],17);
////    ModExp(cipher, e, m, n);
////    gmp_printf("%ZX\n",cipher);
    start = clock();
    ChineseMod(m, cipher, n, d, rsa[0], rsa[1]);
    finish = clock();
    gmp_printf("中国剩余定理解密：%ZX\n",m);
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("seconds:%f\n",duration);
    start = clock();
    ModExp(m, d, cipher, n);
    finish = clock();
    gmp_printf("模重复平方解密：%ZX\n",m);
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("seconds:%f\n",duration);
    start = clock();
    Montgomery(m, d, cipher, n);
    finish = clock();
    gmp_printf("蒙哥马利解密：%ZX\n",m);
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("seconds:%f\n",duration/4);
    return 0;
    
}

/* 求模的逆
 使用辗转相除法 */
void GetInverse(mpz_t out, mpz_t in,mpz_t mod){
    mpz_t buffer,y,z;
    int i, j;      //y存放mod，z存放in，buffer中转，i用作辗转相处的计数
    mpz_t x[2000], s[2000];        //x存放每次的商,s放最后的相加数
    mpz_init(buffer);
    mpz_init(y);
    mpz_init(z);
    mpz_set(z,in);
    mpz_set(y,mod);     //初始化+赋值
//    gmp_printf("%ZX\n%ZX\n",z,y);
    mpz_mod(z, z, y);        //in = in % mod;
    for (i = 0; mpz_cmp_ui(y, 0); i++) {
        mpz_init(x[i]);
//        gmp_printf("%d\n%ZX\n%ZX\n%ZX\n",i,buffer,z,y);
        mpz_fdiv_qr(x[i], buffer, z, y);
        mpz_set(z, y);
        mpz_set(y, buffer);
//        x[i] = mod / in;
//        y = mod;
//        mod = in;
//        in = y - x[i]*in;
    }
    
    mpz_init(s[0]);
    mpz_init_set_ui(s[1],1);
    i -= 2;     //退回倒数第二个式子，取商
    for (j = 2; i >= 0; i--,j++) {
        mpz_init(s[j]);
        mpz_mul(buffer, s[j-1], x[i]);
        mpz_sub(s[j], s[j-2], buffer);      //s[j] = s[j-2] - (s[j-1] * x[i]);
//        mpz_clear(x[i]);
//        gmp_printf("%d    %ZX\n",j,s[j]);
    }
    j -= 2;
    mpz_mod(s[j], s[j], mod);
    mpz_set(out, s[j]); //return s[j];
    /*  释放内存  */
    for (j+=1; j>=2;j--) {
        mpz_clear(s[j]);
    }
    
}

/* 快速模幂运算
 使用模重复平方算法 */
void ModExp(mpz_t result, const mpz_t exponent, const mpz_t base, const mpz_t n)
{
    char exp[2048 + 10] ;
    mpz_get_str(exp, 2, exponent); //把指数e转化为二进制并储存到字符数组exp中
    
    mpz_t x, power;
    mpz_init(power);
    mpz_init_set_ui(x, 1);  // x = 1
    mpz_mod(power, base, n); //power = base (mod n)
//    gmp_printf("%ZX\n%ZX\n%ZX",power,base,n);
    for(int i = strlen(exp) - 1; i >= 0; i--)
    {
        if(exp[i] == '1')
        {
            mpz_mul(x, x, power);
            mpz_mod(x, x, n);   //x = x * power mod n
        }
        mpz_mul(power, power, power);
        mpz_mod(power, power, n);  //power = power^2 mod n
    }
    mpz_set(result, x); //返回结果
//    gmp_printf("%ZX%ZX",result,x);
}


/* 素性检测
 使用Miller-Rabin算法 */
int IsPrime(mpz_t n){     //返回1则为素数，0为合数
    int k;
//    static int jishuqi = 0;
//    printf("%d\n",jishuqi++);
    mpz_t m, num, a, b;       //将n-1写成n-1=2km的形式，其中m是一个奇数，随机选择整数grt，满足1≤grt≤n-1
    mpz_init(a);
    mpz_init(b);
    mpz_init(num);
    mpz_init(m);        //初始化
    mpz_sub_ui(m,n,1);
//    gmp_printf("%ZX",m);
    for (k = 0; ; k++) {
        mpz_mod_ui(a,m,2);      //a暂时判断奇偶数
//        gmp_printf("%ZX",a);
        if(mpz_cmp_ui(a, 0) == 0)
            mpz_div_ui(m,m,2);
        else break;
    }
    for (int j = 0; j<10; j++) {
        //        do a = rand() % n;
        gmp_randstate_t grt;
        gmp_randinit_default(grt);
        gmp_randseed_ui(grt, time(NULL));   //【【【停一下，先去写modexp了】】】
        do {
            mpz_urandomb(a, grt, 1024);
            mpz_mod(a,a,n);
        }
        while (mpz_cmp_ui(a, 1) == 0);        //满足1≤a≤n-1
        ModExp(b, m, a, n);
        mpz_mod(b,b,n);
        if ( mpz_cmp_ui(b, 1)  == 0) continue;      //  b%n == 1
        for (int i = 0; i<k; i++) {
            mpz_mod(b,b,n);
            mpz_sub_ui(num,n,1);
            if ( mpz_cmp(b, num)  == 0) continue;       //if ((b%n) == (n-1)) continue;
            else{
                mpz_mul(b,b,b);
                mpz_mod(b,b,n);     //b = (b*b) % n;
            }
        }
        return 0;
    }
    return 1;
    //    return 0;
}

mpz_t * GetRSAparm(){       //获得两个大素数
    //    RSAparm rsa;
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time(NULL));

    mpz_t * rsa = new mpz_t[2];
    mpz_init(rsa[0]);
    mpz_init(rsa[1]);
    
    mpz_urandomb(rsa[0], grt, 1024);
    mpz_urandomb(rsa[1], grt, 1024);    //随机生成两个大整数

    if(mpz_even_p(rsa[0]))
        mpz_add_ui(rsa[0], rsa[0], 1);
    if(mpz_even_p(rsa[1]))
        mpz_add_ui(rsa[1], rsa[1], 1);    //如果生成的随机数为偶数，则加一
    
//    mpz_t test;
//    mpz_init_set_ui(test,101);
//    int suxing = IsPrime(test);
    while(!IsPrime(rsa[0]))  //逐个检查比p大的奇数是否为素数
        mpz_add_ui(rsa[0], rsa[0], 2);
    while(!IsPrime(rsa[1]))
        mpz_add_ui(rsa[1], rsa[1], 2);
//    gmp_printf("%ZX\n%ZX\n",rsa[0],rsa[1]);
//    int ju = mpz_probab_prime_p(rsa[0],10);
    //    return result;
    //    mpz_init(rsa.d);
    //    mpz_init(rsa.p);
    //    mpz_init(rsa.e);
    //    mpz_init(rsa.n);
    //    mpz_init(rsa.q);        //初始化
    
    //    return rsa;
    return rsa;
}

/* 中国剩余定理
  利用中国剩余定理加速
 */
void ChineseMod(mpz_t plain, mpz_t cipher, mpz_t n, mpz_t d, mpz_t p, mpz_t q) {
    mpz_t x1,x2;
    mpz_init(x1);
    mpz_init(x2);
    ModExp(x1, d, cipher, p);
    ModExp(x2, d, cipher, q);       //x1 = c^d mod p,x2 = c^d mod q

    mpz_t buf1,buf2;
    mpz_init(buf1);
    mpz_init(buf2);
    mpz_invert(buf1, q,p);     //buf存放(M/m)^(-1)
    mpz_invert(buf2, p ,q);
  
    mpz_mul(buf1,x1,buf1);
    mpz_mul(buf1,q,buf1);
//    mpz_mod(buf1,buf1,n);   //b*(M/m)*(M/m)^(-1) mod n
    mpz_mul(buf2,x2,buf2);
    mpz_mul(buf2,p,buf2);
//    mpz_mod(buf2,buf2,n);   //b*(M/m)*(M/m)^(-1) mod n
    
    mpz_add(plain,buf2,buf1);
    mpz_mod(plain,plain,n);
}

/* 蒙哥马利算法
  幂模转化为乘模
 */
void Montgomery(mpz_t result, const mpz_t exponent, const mpz_t base, const mpz_t n) {
    mpz_t buffer,buffers, prod, A, r, r1;
    //buffer用来比较幂是否模1，buffers用来放置exp/2，r1 : r^(-1)

    mpz_init(buffer);
    mpz_init_set(buffers,exponent);
    mpz_init_set_ui(prod,1);
    mpz_init_set(A,base);
    mpz_init(r);
    mpz_ui_pow_ui(r,2,2048);
    mpz_init(r1);
    mpz_invert(r1, r, n);
    mpz_mul(prod,prod,r);
    mpz_mul(A,A,r);
    
    while (mpz_cmp_ui(buffers, 0)) {
        mpz_mod_ui(buffer,buffers,2);
        if (!mpz_cmp_ui(buffer, 1)) {
            MontMul(prod, prod, A, r1, n);
        }
        MontMul(A, A, A, r1, n);
        mpz_div_ui(buffers,buffers,2);
    }
    mpz_mul(prod,prod,r1);
    mpz_mod(prod,prod,n);

    mpz_set(result,prod);
}

/*
 蒙特马力乘模
 */
void MontMul(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t r1, const mpz_t n) {
    mpz_mul(result, a, b);
    mpz_mul(result, result, r1);
    mpz_mod(result, result, n);
}

void MontMulti(mpz_t T, const mpz_t x, const mpz_t y, const  mpz_t N, const mpz_t N_2)
{
    unsigned int i;
    mp_limb_t num, carry, res[MONT_MAX] = { 0 };
    mp_limb_t *temp,t[MONT_MAX] = { 0 };
    mp_limb_t N_1=*(N_2->_mp_d);
    
    //计算x和y的乘积，保存在t中，这里假设x和y均为蒙哥马利数
    if (x->_mp_size > y->_mp_size)
        mpn_mul(t, x->_mp_d, x->_mp_size,y->_mp_d,y->_mp_size);
    else
        mpn_mul(t, y->_mp_d, y->_mp_size, x->_mp_d, x->_mp_size);
    
    temp = t;
    for (i = 0; i < N->_mp_size; i++){
        num = temp[0]*N_1;//num=t[0]*N_1
        res[i]=mpn_addmul_1(temp, N->_mp_d,N->_mp_size,num);//t=t+N*num,但是加法只做了N->_mp_size次，超出N->_mp_size长度的保存在res[i]中
        temp++;//相当于整除2^32
    }
    
    carry = mpn_add_n(temp, temp, res, N->_mp_size);//将上面步骤中所有没有处理的进位res[i]一次性地加到t上
    if (carry != 0 || mpn_cmp(temp, N->_mp_d, N->_mp_size) >= 0)//判断是否需要-N
        mpn_sub_n(temp, temp, N->_mp_d, N->_mp_size);
    
    mpz_import(T, N->_mp_size, -1, sizeof(mp_limb_t), 0, 0,temp);//将得到的结果保存在T中
}
