#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include <stdio.h>   
#include <time.h>


/* run this program using the console pauser or add your own getch, system("pause") or input loop */
#define KEY_LENGTH 2048      //公钥长度 
#define BASE 16				//16进制输入输出 
//#define ll mpz_t 
//#define CLOCKS_PER_SEC (clock_t)1000000   
#define MONTBITS (8*sizeof(mp_limb_t))
//假设蒙哥马利中的T=XY的最大位数不超过MONT_MAX个整数
#define MONT_MAX 128
//随机数状态
      
using namespace std;

struct key_pair

{

	char * n; 
	char * d;  
	int e;
	char * p;
	char * q; 

};


bool Miller(mpz_t p)
{
	gmp_randstate_t grt;			//生成随机数	
	gmp_randinit_default(grt);	
	gmp_randseed_ui(grt, time(NULL));	
	
	mpz_t s,t;
	int k=0;
	mpz_init(s);
	mpz_init(t);
	mpz_sub_ui(s,p,1);
	mpz_mod_ui(t,s,2);
	while(!mpz_cmp_ui(t,0)){
		mpz_divexact_ui(s,s,2);
		mpz_mod_ui(t,s,2);
		k++;
	}
	mpz_t a,b;
	mpz_init(a);
	mpz_init(b);
	mpz_urandomm(a, grt, p);
	mpz_powm(b, a, s, p);

    mpz_t r;
	mpz_init(r);
	mpz_mod(r, b, p); 
	if(!mpz_cmp_ui(r,1))
		return true;
	for(int i=0;i<k;i++){
		if(!mpz_cmp_si(r,-1)){
			return true;
		}
		else 
			mpz_powm_ui(r,b,2,p);
	}
	return false;
}

// 扩展的欧几里得算法求逆 
void invert(mpz_t rop, mpz_t e, mpz_t f){
	mpz_t a, b, t1, t2, t, q;
	mpz_init_set(a, f);
	mpz_init_set(b, e);
	mpz_init(t1);             //t1=0 
	mpz_init_set_ui(t2, 1);   //t2=1
	mpz_init(t);
	mpz_init(q);
	
	while(mpz_cmp_ui(b, 0)!=0)
	{
		mpz_set(t, a);
		mpz_set(a, b);
		mpz_fdiv_qr(q,b,t,b);		// q=t/b  b=t%b
		
		mpz_mul(t,q,t2);              //t=q*t2
		mpz_sub(t,t1,t);        // t=t1-q*t2;
		mpz_set(t1, t2);
		mpz_set(t2, t); 
	}
	if(mpz_cmp_ui(t1,0)<0)
		mpz_add(t1, t1, f);
	mpz_set(rop, t1);
} 

mpz_t * Generate_rand_num()           //生成两个大素数 
{										
	gmp_randstate_t grt;			//生成随机数	
	gmp_randinit_default(grt);	
	gmp_randseed_ui(grt, time(NULL));	
	
	mpz_t key_p, key_q;
	mpz_init(key_p);
	mpz_init(key_q);
	mpz_urandomb(key_p, grt, KEY_LENGTH / 2);		
	mpz_urandomb(key_q, grt, KEY_LENGTH / 2);	//随机生成两个大整数

	mpz_t * result = new mpz_t[2];
	mpz_init(result[0]);
	mpz_init(result[1]);
	
	if(mpz_even_p(key_p))
		mpz_add_ui(key_p, key_p, 1);
	if(mpz_even_p(key_q))
		mpz_add_ui(key_q, key_q, 1);	//如果生成的随机数为偶数，则加一
 	while(!Miller(key_p))  //逐个检查比p大的奇数是否为素数
		mpz_add_ui(key_p, key_p, 2);
	while(!Miller(key_q))
 		mpz_add_ui(key_q, key_q, 2);
		
//	if(mpz_probab_prime_p(key_p, 25)){
//		printf("sush");
//	}
//	if(mpz_probab_prime_p(key_q, 25)){
//		printf("sush");
//	}
	mpz_set (result[0],key_p);
	mpz_set (result[1],key_q);      
	mpz_clear(key_p);
	mpz_clear(key_q);
	return result;	
}
key_pair * gen_key_pair(){     //生成密钥对 
	mpz_t * primes = Generate_rand_num();
	mpz_t key_n, key_e, key_f;
	mpz_init(key_n);
	mpz_init(key_f); 
	mpz_init_set_ui(key_e, 65537);	//设置e为65537
	key_pair * result = new key_pair;

	char *buf_p = new char[KEY_LENGTH + 10];
	char *buf_q = new char[KEY_LENGTH + 10];
	mpz_get_str(buf_p, BASE, primes[0]);
 	result->p = buf_p;
 	mpz_get_str(buf_q, BASE, primes[1]);
 	result->q = buf_q;
	mpz_mul(key_n, primes[0], primes[1]);		//计算n=p*q
	mpz_sub_ui(primes[0], primes[0], 1);		//p=p-1
	mpz_sub_ui(primes[1], primes[1], 1);		//q=q-1
 	mpz_mul(key_f, primes[0], primes[1]);		//计算欧拉函数φ(n)=(p-1)*(q-1)
  
	mpz_t key_d;	
 	mpz_init(key_d);
// 	mpz_invert(key_d, key_e, key_f);   //计算数论倒数   求逆 
	invert(key_d, key_e, key_f); 
	
	char * buf_n = new char[KEY_LENGTH + 10];  	
 	char * buf_d = new char[KEY_LENGTH + 10];
	
	mpz_get_str(buf_n, BASE, key_n);                 //将key_n转换成16进制下表示的字符串 
	result->n       = buf_n;
 	mpz_get_str(buf_d, BASE, key_d);
 	result->d = buf_d;
	result->e = 65537;
	
	mpz_clear(primes[0]);   //释放内存
	mpz_clear(primes[1]);
	mpz_clear(key_n);
	mpz_clear(key_d);
	mpz_clear(key_e);
	mpz_clear(key_f);

	return result;
}
// 模重复平方法 
void mod_exp(mpz_t result, const mpz_t exponent, const mpz_t base, const mpz_t n){     //const修饰函数参数    传递过来的参数在函数内不可以改变，与上面修饰变量时的性质一样。
	char exp[2048+10];																					
	mpz_get_str(exp, 2, exponent); //把指数e转化为二进制并储存到字符数组exp中
	mpz_t x, power;                                                             
	mpz_init(power);
	mpz_init_set_ui(x,1);  
	mpz_mod(power, base, n);  //power = base mod n
	
	for(int i=strlen(exp)-1;i>-1;i--){
		if (exp[i]=='1'){
			mpz_mul(x,x,power);             //x = x*power; 
			mpz_mod(x,x,n);                 // x = x mod n;
		}
		mpz_mul(power,power,power);        // power = power^2;
		mpz_mod(power,power, n);           //  power = power mod n;
	}
	mpz_set(result,x);
}
//模重复平方 加密
char *mod_Encryption(const char * plain_text, const char * key_n, int key_e)
{
	mpz_t M, res, n, e;
	mpz_init_set_str(M, plain_text, BASE);
	mpz_init_set_str(n, key_n, BASE);
	mpz_init_set_ui(res, 0);
	mpz_init_set_ui(e, key_e);
	
	mod_exp(res, e, M, n);
	
	char * result = new char[KEY_LENGTH+10];
	mpz_get_str(result, BASE, res);
	
	return result;
 } 
//模重复平方解密
char *mod_Decryption(const char* cipher_text, const char*key_n, const char * key_d)
{
	mpz_t M,C,n,d;
	mpz_init_set_str(C, cipher_text, BASE);
	mpz_init_set_str(n, key_n, BASE);
	mpz_init_set_str(d, key_d, BASE);
	mpz_init(M);
	
	mod_exp(M,d,C,n);
	
	char * result = new char[KEY_LENGTH+10];
	mpz_get_str(result, BASE, M);
	
	return result;
 } 
// 中国 剩余定理实现
 void China(mpz_t result, const char * P, const char * Q, const char*X, const char*E, const char*N)
 {
 	mpz_t p,q,x,e,n;
 	mpz_t Xp,Xq,ep,eq,P1,Q1,Yp,Yq,total;
 	mpz_inits(Xp,Xq,ep,eq,P1,Q1,Yp,Yq,total,NULL);
 	
 	mpz_init_set_str(p,P,BASE);
 	mpz_init_set_str(q,Q,BASE);
//	mpz_init_set(p,P);
//	mpz_init_set(q,Q);
 	mpz_init_set_str(n,N,BASE);
 	mpz_init_set_str(x,X,BASE);
 	mpz_init_set_str(e,E,BASE);
 	
 	mpz_mod(Xp,x,p);
 	mpz_mod(Xq,x,q);
 	mpz_sub_ui(P1,p,1);
 	mpz_sub_ui(Q1,q,1);
 	mpz_mod(ep,e,P1);
 	mpz_mod(eq,e,Q1);
 	mod_exp(Yp,ep,Xp,p);
 	mod_exp(Yq,eq,Xq,q);
// 	mpz_invert(Q1,q,p);                 //求逆 
// 	mpz_invert(P1,p,q);
 	invert(Q1, q, p);
 	invert(P1, p, q);
 	
 	mpz_mul(q,Q1,q);
 	mpz_mul(q,Yp,q);
 	mpz_mul(p,P1,p);
 	mpz_mul(p,Yq,p);
 	mpz_add(total,p,q);
 	mpz_mod(result,total,n);
 	
  } 
 //中国剩余定理加密
char *China_Encryption(const char * Plaintext, const char * key_p, const char * key_q, const char *key_n, int key_e)
{
	  mpz_t res;
	  mpz_init_set_ui(res, 0);
	  char e_c[2048+10];
	  _itoa(key_e,e_c,16);			//将整型转化成字符串 
	  
	  China(res, key_p, key_q, Plaintext, e_c, key_n);
	  
	  char * result = new char[KEY_LENGTH+10];
	  mpz_get_str(result, BASE, res);
	  return result; 
 } 
 //中国剩余定理解密
char *China_Decryption(const char * cipher_text, const char * key_p, const char * key_q, const char* key_n, const char* key_d)
{
		mpz_t res;
		mpz_init_set_ui(res,0);
		char e_c[2048+10];
//		_itoa(key_d, e_c, 16);    //将整型转化成字符串
		
		China(res, key_p, key_q, cipher_text,key_d, key_n);
		
		char * result = new char[KEY_LENGTH+10];
		mpz_get_str(result, BASE, res);
		return result; 
 } 

 //蒙哥马利加密算法
 void montgomery1(mpz_t res, const char *base, const char *exponent, const char *n)
 {
 	mpz_t A,B,N,D;
 	mpz_t temp, R;
 	mpz_init_set_ui(temp,0);
 	mpz_init_set_str(A, base, BASE);
 	mpz_init_set_str(B, exponent, BASE);              //16进制大数 
 	mpz_init_set_str(N, n, BASE);
 	mpz_init_set_ui(D, 1);
 	
 	while(mpz_cmp_ui(B,0)){
 		if(mpz_odd_p(B)){              //确定B是否为奇数； 
 			mpz_mul(temp, D, A);
 				mpz_mod(D, temp, N);
			 mpz_sub_ui(B, B, 1);
		 }
		 else{
		 	mpz_pow_ui(temp, A, 2);             //指数函数  temp=A^2; 
			mpz_mod(A, temp, N);
			mpz_divexact_ui(B, B, 2);           //     B=B/2;
		 }
	 }
	mpz_set(res,D);
}

//蒙哥马利乘法 
void MontMulti(mpz_t T, const mpz_t x, const mpz_t y, const  mpz_t N, const mp_limb_t N_1)
{
	/*
	功能：计算x和y的蒙哥马利乘积，结果保存在T中，其中 0<=x，y<N
	N：模
	N_1:满足N*N_1=-1(mod 2^32)的整数
	*/

	unsigned int i;
	mp_limb_t num, carry, res[MONT_MAX] = { 0 };
	mp_limb_t *temp,t[MONT_MAX] = { 0 };

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
//void mul_m(mpz_t z, mpz_t x, mpz_t y, mpz_t n) 		//z=x*y mod(n);
//{
//	mpz_t x1;
//	mpz_init(x1);
//	
//	mpz_mul(x1, x, y);			//x1=x*y
//	int i;
//	
//	for(i=0; i<mpz_sizeinbase(n, 2);i++){
//		if(mpz_odd_p(x1))
//			mpz_add(x1, x1, n);
//		mpz_cdiv_q_2exp(x1, x1, 1);   //x1=x1/2    右移一位 
//	}	
////    if(mpz_cmp(x1, n)>=0)
////		mpz_sub(x1, x1, n);
//	mpz_mod(x1, x1, n);
//	mpz_set(z, x1);
//	mpz_clear(x1); 
//} 
void montgomery(mpz_t res, const char *base, const char *exponent, const char *n){
	int i;

	mpz_t A,B,N,D,r;
	mpz_init(r);
 	mpz_t Prod, R, R1;
 	mpz_init(Prod);
 	mpz_init_set_str(A, base, BASE);
 	mpz_init_set_str(B, exponent, BASE);              //16进制大数 
 	mpz_init_set_str(N, n, BASE);
 	mpz_init_set_ui(D, 1);
 	
 	mp_limb_t N_1;
 	mpz_t N_inv, b;
 	mpz_init(b);
 	mpz_init(N_inv);
 	mpz_setbit(b, MONTBITS);	//b = 2^32
	mpz_invert(N_inv, N, b);//
	mpz_sub(N_inv, b, N_inv);
	N_1 = *(N_inv->_mp_d);	//N*N_1 =-1 (mod 2^32)

// 	mpz_mul_2exp(r, D, mpz_sizeinbase(N, 2));		//r=2^key_N;
	unsigned int rbit = N->_mp_size*MONTBITS;//蒙哥马利中r的选择是和模的位数相关的，r=2^rbit。此处并不要求N一定是1024比特，1023，1022等，得到的rbit值都是一样的
 	mpz_setbit(r, 2 * rbit);//K=r^2
	mpz_mod(r, r, N);//保证0<=r<N
// 	mul_m(Prod, r, r, N);                          //prod = mont(1);
 	MontMulti(Prod, r, D, N, N_1);                 //计算Mont(1)=1*r(mod N)=Prod; 
 	MontMulti(A, r, A, N, N_1);                    //计算Mont(A)=base *r (mod N); 
 	
// 	mpz_invert(r, r, N);                         //计算r^-1 
 	int B_bit=mpz_sizeinbase(B, 2);
 
 	for(i=0; i<B_bit; i++){                     
 		if(mpz_odd_p(B)){
 			mpz_sub_ui(B, B, 1);
// 			mul_m(Prod, Prod, A, N);
			MontMulti(Prod, Prod, A, N, N_1);      //B为奇数    计算 Prod *A*r-1; 
		 }
//		 mul_m(A, A, A, N);
		 MontMulti(A, A, A, N, N_1);
		 mpz_cdiv_q_2exp(B, B, 1);
	 }
//	 mul_m(Prod, Prod, D, N);
	 MontMulti(Prod, Prod, D, N, N_1);
	 mpz_set(res, Prod);

}
  //蒙哥马利算法加密函数
char * mont_Encryption(const char*plain_text, const char*key_n, int key_e){
	mpz_t e;
	mpz_t x,n,res;
	mpz_init(res);
	mpz_init_set_str(x,plain_text,BASE);
	mpz_init_set_str(n, key_n, BASE);
	mpz_init_set_ui(e,key_e);
	
	char e_c[2048+10];
	_itoa(key_e, e_c, 16);
	montgomery1(res, plain_text,e_c,key_n);
	char*result = new char[KEY_LENGTH +10];
	mpz_get_str(result,BASE,res);
	
	return result;
} 
//蒙哥马利算法解密函数
char *mont_Decryption(const char *cipher_text, const char *key_n, const char*key_d){
	mpz_t M;
	mpz_init(M);
	montgomery(M, cipher_text, key_d, key_n);
	char *result = new char[KEY_LENGTH+10];
	mpz_get_str(result, BASE, M);
	return  result;
} 
int main(){
	key_pair *Key = gen_key_pair();
	cout<<"   n="<<Key->n<<endl;
	cout<<"   d="<<Key->d<<endl;
	cout<<"   e="<<Key->e<<endl;
	cout<<"   p="<<Key->p<<endl;
	cout<<"   q="<<Key->q<<endl;
	
	char buf[KEY_LENGTH+10];
	char *Output_Plain, *Output_Cipher;
	int  op;
	clock_t start,finish;
	double duration;
	cout<<"请输入要加密的数字，二进制长度不超过"<<KEY_LENGTH<<endl;
	cin>>buf;
//	cout<<"请选择使用的算法："<<endl;
	
	cout<<endl<<"模重复平方加密 密文为："<<endl;
	Output_Cipher = mod_Encryption(buf, Key->n, Key->e); 
	cout<<endl<<Output_Cipher<<endl;
	cout<<endl<<"模重复平方解密 明文为："<<endl;
	start = clock();
	Output_Plain = mod_Decryption(Output_Cipher, Key->n, Key->d); 
	finish = clock();
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	cout<<endl<<Output_Plain<<endl; 
	cout<<"The time is:"<< duration<<endl;
	
	cout<<endl<<"蒙哥马利算法加密 密文为："<<endl;
	Output_Cipher = mont_Encryption(buf, Key->n, Key->e);
	cout<<endl<<Output_Cipher<<endl;
	cout<<endl<<"蒙哥马利算法解密 明文为："<<endl;
	start = clock();
	Output_Plain = mont_Decryption(Output_Cipher, Key->n, Key->d);
// 	Output_Plain =decrypt_montgomery(Output_Cipher, Key->n, Key->d);
	finish = clock();
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	cout<<endl<<Output_Plain<<endl;
	cout<<"The time is:"<< duration<<endl;
	
	cout<<endl<<"中国剩余定理加密 密文为："<<endl; 
	Output_Cipher = China_Encryption(buf, Key->p, Key->q, Key->n, Key->e);
	cout<<endl<<Output_Cipher<<endl;
	cout<<endl<<"中国剩余定理解密 明文为："<<endl;
	start = clock();
	Output_Plain = China_Decryption(Output_Cipher, Key->p, Key->q, Key->n, Key->d);
	finish = clock();
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	cout<<endl<<Output_Plain<<endl;
	cout<<"The time is:"<< duration<<endl;
	return 0;	 
}
