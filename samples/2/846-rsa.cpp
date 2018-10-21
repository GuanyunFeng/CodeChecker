#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gmp.h"
//模的位数
#define TESTBITS 1024
//GMP中，每个单精度整数占用多少比特
#define MONTBITS (8*sizeof(mp_limb_t))//8*4
//假设蒙哥马利中的T=XY的最大位数不超过MONT_MAX个整数
#define MONT_MAX 64
//随机数状态
//模重复平方法计算x^indexnumber(mod n)
void ModelRepetitionSquare(mpz_t r,mpz_t x,mpz_t indexnumber,mpz_t n)
{
    int i;
    char index[2049];
    mpz_get_str(index,2,indexnumber); //把指数indexnumber转化为二进制并储存到字符数组index中
    mpz_t z;
    mpz_init(z);
    mpz_init_set_ui(z,1); //z=1
    for(i=0;i<strlen(index);i++)
    {
        mpz_mul(z,z,z);
        mpz_mod(z,z,n); //z=z^2(mod n)
        if(index[i]=='1')
        {
            mpz_mul(z,z,x);
            mpz_mod(z,z,n);//z=z*x(mod n)
        }
    }
    mpz_set(r,z);
 //   gmp_printf("z=\n%ZX\n\n",z);
}

//模重复平方法处理明文加密
void model_Encrypt(mpz_t Y,char *plain_text,mpz_t n,mpz_t e)
{
	mpz_t P;
	mpz_init(P);
	mpz_init_set_str(P,plain_text,16);
	ModelRepetitionSquare(Y,P,e,n);    //使用模重复平方法快速计算
    mpz_clear(P);
	gmp_printf("Y=\n%ZX\n\n",Y);
}

// 模重复平方解密函数
void model_Decrypt(mpz_t Y,mpz_t n,mpz_t d)
{
    mpz_t X;
	mpz_init(X);
	ModelRepetitionSquare(X,Y,d,n);   //使用模重复平方法快速计算
	gmp_printf("X=\n%ZX\n\n",X);
    mpz_clear(X);
}

//求乘法逆元素函数，t=b^-1(mod a)
int MultiplicativeInverse(mpz_t t,mpz_t b,mpz_t a)
{
    mpz_t a0,b0,t0,q,r,temp;
    mpz_init(a0);
    mpz_init(b0);
    mpz_init(t0);
    mpz_init(t);
    mpz_init(q);
    mpz_init(r);
    mpz_init(temp);
    mpz_set(a0,a);
    mpz_set(b0,b);
    mpz_set_ui(t0,0);
    mpz_set_ui(t,1);
    mpz_fdiv_qr(q,r,a0,b0);//fdiv中q向下趋近于-∞舍入,r有与d相同的符号,f表示“floor”
    //gmp_printf("q=\n%ZX\n\n",q);
    //gmp_printf("r=\n%ZX\n\n",r);
    while(mpz_cmp_ui(r,0))//r!=0
    {
        mpz_submul(t0,q,t);
        mpz_set(temp,t0);
        mpz_mod(temp,temp,a);//temp=(t0-q*t)mod a
        mpz_set(t0,t);
        mpz_set(t,temp);
        mpz_set(a0,b0);
        mpz_set(b0,r);
        mpz_fdiv_qr(q,r,a0,b0);
    }
    if(mpz_cmp_ui(b0,1))//b0!=1
    {
        gmp_printf("%ZX has no inverse modulo %ZX\n",b,a);
        return 0;
    }
    else return 1;
    mpz_clear(a0);
    mpz_clear(b0);
    mpz_clear(t0);
    mpz_clear(q);
    mpz_clear(r);
    mpz_clear(temp);
}

//中国剩余定理计算x^e(mod n)
void CRT(mpz_t Y,mpz_t x,mpz_t p,mpz_t q,mpz_t e,mpz_t n)
{
    mpz_t Xp,Xq,ep,eq,P_sub,Q_sub,Yp,Yq,Q_,P_,total;
    mpz_inits(Xp,Xq,ep,eq,P_sub,Q_sub,Yp,Yq,Q_,P_,total,NULL);
    mpz_init(Y);
    mpz_mod(Xp,x,p); //先对底数处理，Xp=x(mod p)
    mpz_mod(Xq,x,q); //Xq=x(mod q)
    mpz_sub_ui(P_sub,p,1); //P_sub=p-1
    mpz_sub_ui(Q_sub,q,1); //Q_sub=q-1
    mpz_mod(ep,e,P_sub); //应用费马小定理对指数处理，ep=e mod(p-1)
    mpz_mod(eq,e,Q_sub); //同理，eq=e mod(q-1)
//    double start,finish;
//    start=clock();//取开始时间
    ModelRepetitionSquare(Yp,Xp,ep,p);
    ModelRepetitionSquare(Yq,Xq,eq,q);
//    finish=clock();//取结束时间
 //   printf("中国剩余定理加解密用时：%f seconds\n\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    MultiplicativeInverse(Q_,q,p); //计算逆元
    MultiplicativeInverse(P_,p,q);
    mpz_mul(P_sub,Q_,q);
    mpz_mul(P_sub,Yp,P_sub); //P_sub=Yp*q*Q_
    mpz_mul(Q_sub,P_,p);
    mpz_mul(Q_sub,Yq,Q_sub); //Q_sub=Yq*p*P_
    mpz_add(total,P_sub,Q_sub);
    mpz_mod(Y,total,n); //Y=(Yp*q*Q_+Yq*p*P_)mod n
    mpz_clears(Xp,Xq,ep,eq,P_sub,Q_sub,Yp,Yq,Q_,P_,NULL);
}

//中国剩余定理处理明文加密
void CRT_Encrypt(mpz_t Y,char *plain_text,mpz_t p,mpz_t q,mpz_t n,mpz_t e)
{
    mpz_t P;
	mpz_init(P);
	mpz_init_set_str(P,plain_text,16);
	CRT(Y,P,p,q,e,n);
    mpz_clear(P);
	gmp_printf("Y=\n%ZX\n\n",Y);
}

//中国剩余定理解密函数
void CRT_Decrypt(mpz_t Y,mpz_t p,mpz_t q,mpz_t d,mpz_t n)
{
    mpz_t X;
	mpz_init(X);
	CRT(X,Y,p,q,d,n);   //使用中国剩余定理快速计算
	gmp_printf("X=\n%ZX\n\n",X);
    mpz_clear(X);
}

void Mont(mpz_t A,mpz_t a,mpz_t N)
{
    mpz_t R;
    mpz_init(R);
    mpz_init(A);
    mpz_setbit(R,2048); //R=2^2048
    mpz_mul(A,a,R);
    mpz_mod(A,A,N); //A=aR(mod N)
    mpz_clear(R);
}

void MontInv(mpz_t A,mpz_t N)
{
    mpz_t R;
    mpz_init(R);
    mpz_setbit(R,2048); //R=2^2048
    mpz_mod(R,R,N);
    mpz_invert(R,R,N);
    mpz_mul(A,A,R);
    mpz_mod(A,A,N); //A=AR^-1(mod N)
    mpz_clear(R);

}


void MontMult(mpz_t r,mpz_t A,mpz_t B,mpz_t N)
{
    mpz_t T,d,t0,IN,Tp;
    mpz_inits(T,d,t0,IN,Tp,NULL);
    mpz_setbit(d,32); //d=2^32
    mpz_mul(T,A,B); //T=AB
/*
    mpz_t a;
    mpz_init(a);
    mpz_setbit(a,2048); //a=2^48
    mpz_invert(a,a,N); //a=a^-1(mod N)
    mpz_mul(a,a,T);//a=ABR^-1(mod N)
    mpz_mod(r,a,N);
*/

    int i;
  //  int j,len;
    mpz_invert(IN,N,d); //IN=N^-1(mod d)
    mpz_neg(IN,IN);
//    char t[4100]={0};  //空间
    for(i=0;i<64;i++)
    {
/*
    char str[33]={0};
    mpz_get_str(t,2,T); //把T转化为二进制并储存到字符数组t中
    len=strlen(t);
    for(j=0;j<32;j++)
    {
        str[j]=t[len-32+j];
    }
    str[32]='\0';
    mpz_init_set_str(t0,str,2);// t0为T低32位
    */
    mpz_mod(t0,T,d);
    mpz_mul(Tp,IN,t0);
    mpz_mod(Tp,Tp,d); //Tp=-t0*IN(mod d)
    mpz_mul(Tp,Tp,N); //Tp=N*(-t0*IN(mod d))
    mpz_add(T,T,Tp); //T=T+N*(-t0*IN(mod d))
   // mpz_divexact(T,T,d);
    mpz_fdiv_q_2exp(T,T,32);
    }
    if(mpz_cmp(T,N)>=0)//判断是否需要-N
		mpz_sub(T,T,N);
    mpz_set(r,T); //r=T=ABR^-1(modN)
    mpz_clears(T,d,t0,IN,Tp,NULL);

}
//蒙哥马利算法计算a^e(mod N)
void Montgomery(mpz_t Prod,mpz_t a,mpz_t e,mpz_t N)
{
    mpz_t temp,A;
    mpz_init(temp);
    mpz_init(Prod);
    mpz_init(A);
    mpz_init_set_ui(temp,1); //temp=1
    Mont(Prod,temp,N);
    mpz_clear(temp);
    Mont(A,a,N);
    int i=0;
    while(mpz_cmp_ui(e,0)) //e!=0
    {
        if(mpz_odd_p(e)) //判断e为奇数若是，返回非0，否则返回0。
        {
            MontMult(Prod,Prod,A,N);
            i++;
            mpz_sub_ui(e,e,1); //e--
        }
        MontMult(A,A,A,N);
        mpz_divexact_ui(e,e,2);
        i++;
    }
    MontInv(Prod,N);
}
//蒙哥马利算法处理明文加密
void Montgomery_Encrypt(mpz_t Y,char *plain_text,mpz_t n,mpz_t e)
{
	mpz_t P;
	mpz_init(P);
	mpz_init_set_str(P,plain_text,16);
    double start,finish;
    start=clock();//取开始时间
	Montgomery(Y,P,e,n);    //使用蒙哥马利算法快速计算
    finish=clock();//取结束时间
    gmp_printf("Y=\n%ZX\n\n",Y);
    printf("蒙哥马利算法加密用时：%f seconds\n\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    mpz_clear(P);
}

// 蒙哥马利算法解密函数
void Montgomery_Decrypt(mpz_t Y,mpz_t n,mpz_t d)
{
    mpz_t X;
	mpz_init(X);
	double start,finish;
    start=clock();//取开始时间
	Montgomery(X,Y,d,n);   //使用蒙哥马利算法快速计算
    finish=clock();//取结束时间
    gmp_printf("X=\n%ZX\n\n",X);
    printf("蒙哥马利算法解密用时：%f seconds\n\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    mpz_clear(X);
}

//Miller-Rabin检测算法
int TestPrime(mpz_t n)
{
	mpz_t a,k,m,j,l,n_sub,b;
	mpz_init(a);
	mpz_init(k);
	mpz_init(m);
	mpz_init(j);
	mpz_init(l);
	mpz_init(n_sub);
	mpz_init(b);
	gmp_randstate_t state;
	gmp_randinit_default(state);
	mpz_sub_ui(n_sub,n,1);//n_sub=n-1;
	mpz_set(m,n_sub);//m=n-1;
	mpz_mod_ui(l,m,2);//l=m%2;
	// n-1=2^k*m
	while(!mpz_cmp_ui(l,0))//l=0
	{
		mpz_fdiv_q_ui(m,m,2);//m=m/2;
		mpz_add_ui(k,k,1);// k++;
		mpz_mod_ui(l,m,2);//l=m%2;
	}
//	gmp_printf("k=%Zd\n",k);
//	gmp_printf("m=%Zd\n",m);
	mpz_urandomm(a,state,n);//产生随机数1<=a<=n-1;
	mpz_powm(b,a,m,n);    //使用模重复平方法计算函数
	if (!mpz_cmp_ui(b,1)||!mpz_cmp(b,n_sub))//b==1||b==n-1
		return 1;
	else
	{
		for(;mpz_cmp(j,k);mpz_add_ui(j,j,1))//j=0..k-1
		{
			mpz_mul(b,b,b);
			mpz_mod(b,b,n);//b=b^2(mod n);
			if(!mpz_cmp(b,n_sub))//b==n-1
				return 1;
		}
	}
	mpz_clear(a);
	mpz_clear(k);
	mpz_clear(m);
	mpz_clear(j);
	mpz_clear(l);
	mpz_clear(n_sub);
	mpz_clear(b);
	return 0;
}

int MRTest(mpz_t n)
{
    int i;
	for (i=0;i<5;i++)//当输入的待测数n是很大(1024bits)的随机数的时候，算法的误判概率远远小于上面给出的理论上的误判概率上界，进行五轮测试，出错概率近乎为0
	{
		if(TestPrime(n))
			return 1;
	}
	return 0;
}

//加密函数（16进制）
void Encrypt(mpz_t Y,char *plain_text,mpz_t n,mpz_t e)
{
	mpz_t P;
	mpz_init(P);
	mpz_init_set_str(P,plain_text,16);
	mpz_powm(Y,P,e,n);    //使用GMP中模幂计算函数
    mpz_clear(P);
	gmp_printf("Y=\n%ZX\n\n",Y);
}

//加密函数（10进制）
void Encrypt2(mpz_t Y,int plain_text,mpz_t n,mpz_t e)
{
	mpz_t P;
    mpz_init(P);
	mpz_init_set_ui(P,plain_text);
	mpz_powm(Y,P,e,n);    //使用GMP中模幂计算函数
	gmp_printf("Y=\n%Zd\n\n",Y);
}

//解密函数（16进制）
void Decrypt(mpz_t Y,mpz_t n,mpz_t d)
{
    mpz_t X;
	mpz_init(X);
	mpz_powm(X,Y,d,n);   //使用GMP中的模幂计算函数
	gmp_printf("X=\n%ZX\n\n",X);
}

//解密函数（10进制）
void Decrypt2(mpz_t Y,mpz_t n,mpz_t d)
{
	mpz_t X;
	mpz_init(X);
	mpz_powm(X,Y,d,n);   //使用GMP中的模幂计算函数
	gmp_printf("X=\n%Zd\n\n",X);
}



int main()
{
    int flag,i=0;
//    int j=0;
    mpz_t bignum1, bignum2,p,q; //定义声明gmp的整数,mpz_t 多精度整型
    mpz_init(bignum1);  //　初始化，默认值为０
    mpz_init(bignum2);
    mpz_init(p);
    mpz_init(q);
    unsigned short seed;
    printf("正在生成参数...\n");
    seed=time(NULL); //把系统时间作为种子
    gmp_randstate_t state;
    gmp_randinit_default(state);//设置state的随机数生成算法，默认为梅森旋转算法
    gmp_randseed_ui(state,seed);//对state置初始种子
    mpz_urandomb(bignum1,state,1024);
    mpz_urandomb(bignum2,state,1024); //随机生成两个大整数
    flag=0;//终止标志
    while(flag==0)
    {
    //首先判断是否为偶数，是偶数则加1
        mpz_t temp;
	    mpz_init(temp);
	    mpz_mod_ui(temp,bignum1,2);
	    if(!mpz_cmp_ui(temp,0))//temp == 0
	    {
		    mpz_add_ui(bignum1,bignum1,1);
	    }
	    mpz_clear(temp);
	    //对奇数bignum1进行Miller-Rabin算法检测
        if(MRTest(bignum1))
        {
//            printf("It's a prime.\n");
            mpz_set(p,bignum1);
            flag=1;
        }
        else
        {
//            printf("It's not a prime.\n");
//            gmp_printf("\n%ZX\n\n",bignum1);
            mpz_add_ui(bignum1,bignum1,2);  //每次加2直到得到素数
            i++;
//            j++;
//            printf("\n%d\n\n",j);    //统计次数
        }
    }
    flag=0;//终止标志
    while(flag==0)
    {
    //首先判断是否为偶数，是偶数则加1
        mpz_t temp;
	    mpz_init(temp);
	    mpz_mod_ui(temp,bignum2,2);
	    if(!mpz_cmp_ui(temp,0))//temp == 0
	    {
		    mpz_add_ui(bignum2,bignum2,1);
	    }
	    mpz_clear(temp);
	    //对奇数bignum2进行Miller-Rabin算法检测
        if(MRTest(bignum2))
        {
//            printf("It's a prime.\n");
            mpz_set(q,bignum2);
            flag=1;
        }
        else
        {
//            printf("It's not a prime.\n");
//            gmp_printf("\n%ZX\n\n",bignum2);
            mpz_add_ui(bignum2,bignum2,2);  //每次加2直到得到素数
            i++;
//            j++;
//            printf("\n%d\n\n",j);    //统计次数
        }
    }
    gmp_printf("p=\n%ZX\n\n",p);//按大整数的格式来输出结果，因为是mpz_t类型，不能用一般的printf，只能用gmp_printf
	gmp_printf("q=\n%ZX\n\n",q);
    mpz_clear(bignum1); //释放integer所占据的空间，使用完应该对所有的mpz_t调用这个函数。
    mpz_clear(bignum2);
//生成密钥对
    mpz_t n,e,d,f,p_sub,q_sub;
    mpz_init(n);  //　初始化，默认值为０
    mpz_init(e);
    mpz_init(d);
    mpz_init(f);
    mpz_init(p_sub);
    mpz_init(q_sub);
    mpz_init_set_ui(e,65537); //赋值函数，置e为65537
    mpz_mul(n,p,q); //大数乘法,计算n=p*q
    gmp_printf("(n,e)=\n(%ZX,%Zd)\n\n",n,e);
    mpz_sub_ui(p_sub,p,1); //大数减法,计算p_sub=p-1
    mpz_sub_ui(q_sub,q,1); //大数减法,计算p_sub=q-1
    mpz_mul(f,p_sub,q_sub); //计算欧拉函数φ(n)=(p-1)*(q-1)
//求乘法逆元素函数，t=b^-1(mod a)
    MultiplicativeInverse(d,e,f);
//   mpz_invert(d,e,f); //求逆元函数，即数论倒数，计算d=e^-1(mod f)
    gmp_printf("d=\n%ZX\n\n",d);
//以上为生成 RSA参数p、q、n、e、d
    char plain_text[2049]={0};
//    char cipher_text[2049]={0};
	printf("请输入要加密的数字，二进制长度不超过2048:");
    scanf("%s",plain_text);
	mpz_t Y;
//    mpz_init(Y);
//    Encrypt(Y,plain_text,n,e);
    double start,finish;
    start=clock();//取开始时间
    model_Encrypt(Y,plain_text,n,e);
    finish=clock();//取结束时间
    printf("模重复平方加密用时：%f seconds\n\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    start=clock();//取开始时间
 	model_Decrypt(Y,n,d);
    finish=clock();//取结束时间
    printf("模重复平方解密用时：%f seconds\n\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    mpz_init(Y);
    start=clock();//取开始时间
    CRT_Encrypt(Y,plain_text,p,q,n,e);
    finish=clock();//取结束时间
    printf("中国剩余定理加密用时：%f seconds\n\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    start=clock();//取开始时间
    CRT_Decrypt(Y,p,q,d,n);
    finish=clock();//取结束时间
    printf("中国剩余定理解密用时：%f seconds\n\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    mpz_init(Y);
    Montgomery_Encrypt(Y,plain_text,n,e);
    Montgomery_Decrypt(Y,n,d);
/*
//给定RSA参数测试
    mpz_t n,e,d;
    mpz_init(n);
    mpz_init(e);
    mpz_init(d);
    int n_num,e_num,d_num;
    scanf("%d%d%d",&n_num,&e_num,&d_num);
    mpz_init_set_ui(n,n_num); //n=n_num
    mpz_init_set_ui(e,e_num); //e=n_num
    mpz_init_set_ui(d,d_num); //d=n_num
    int plain_text;
	printf("请输入要加密的数字，二进制长度不超过2048:");
    scanf("%d",&plain_text);
	mpz_t Y;
    mpz_init(Y);
    Encrypt2(Y,plain_text,n,e);
    Decrypt2(Y,n,d);
//RSA测试
    mpz_t t,a,b;
    mpz_init(t);  //　初始化，默认值为０
    mpz_init(a);
    mpz_init(b);
    mpz_set_ui(a,14);//模数
    mpz_set_ui(b,2);
    if(MultiplicativeInverse(t,b,a)==1)
    {
        gmp_printf("t=\n%Zd\n\n",t);
    }
//测试求逆函数
*/
//测试部分
    mpz_clear(Y);
    return 0;
}

