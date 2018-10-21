#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <big.h>
#include <mirdef.h>
#include <miracl.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define times 10

using namespace std;
big Mont(big a,big N,big R)
{
    big A=mirvar(0);
    big O=mirvar(1);
    multiply(a,R,A);//A=aR
    powmod(A,O,N,A);//A=aR(mod N)
    return A;
}

big MontInv(big A,big N,big R)
{
    big AI=mirvar(0);
    big O=mirvar(1);
    xgcd(R, N, AI, AI, AI);//AI=R^-1(mod N)
    multiply(A,AI,AI);//AI=A*AI
    powmod(AI,O,N,AI);//AI=A*R^-1(mod N)
    return AI;
}


big MontMulT(big A,big B,big R,big N,long k)
{ miracl *mip = mirsys(65535, 2);
  //int len;


  char t[k+1];
  char t0[33];
  long i;
  big RI=mirvar(0);
  big T=mirvar(0);
  big Tp=mirvar(0);
  big T0=mirvar(0);
  big IN=mirvar(0);
  big d=mirvar(0);
  big O=mirvar(1);
  long n=32;
  //big temp=mirvar(0);
  expb2(32,d);//d=2^32
  xgcd(R,N,RI,RI,RI);//RI=R^-1(mod N)
  xgcd(N,d,IN,IN,IN);//IN=N^-1(mod d)
  negify(IN,IN);//IN=-IN
  multiply(A,B,T);//T=AB
  mip->IOBASE=2;
  cotstr(T,t);//将大数T转换为字符串t,长度为k
  for(i=0;i<n;i++)
 {
  strncpy(t0,t+k-32*i,32);
  t0[32]='\0';
  mip->IOBASE=2;
  cinstr(T0,t0);//将最后32位转换为大数T0
  multiply(T0,IN,Tp);//Tp=T0*IN
  powmod(Tp,O,d,Tp);//Tp=T0*IN(mod d)
  multiply(Tp,N,Tp);//Tp=N*(T0*IN(mod d))
  add(Tp,T,Tp);//Tp=T+N*(T0*IN(mod d))
  sftbit(Tp,-32,Tp);//Tp右移32位
  copy(Tp,T);//T=Tp
 }
 powmod(Tp,O,N,T);
 return T;
}

big R_generate(long k)
{   big R=mirvar(0);
    expb2(k,R);//R=2^k
    return R;
}




big Euclid(big e,big eu)
{//miracl *mip = mirsys(65536, 10);
    big a0=mirvar(0);
    big b0=mirvar(0);
    big t0=mirvar(0);
    big t=mirvar(1);
    big s0=mirvar(1);
    big s=mirvar(0);
    big q=mirvar(0);
    big r=mirvar(0);
    big zero=mirvar(0);
    big temp=mirvar(0);
    big a0t=mirvar(0);
    big b0t=mirvar(0);
    copy(eu,a0);//a0=eu
    copy(e,b0);//b0=e
    divide(a0,b0,q);//q=a0/b0
    copy(eu,a0);//a0=eu
    copy(e,b0);//b0=e
    multiply(q,b0,r);//r=qb0
    negify(r,r);//r=-r
    add(a0,r,r);//r=a0+(-r)
    while(mr_compare(r,zero)>0)
    {
    multiply(q,t,temp);//temp=qt
    negify(temp,temp);//temp=-temp
    add(t0,temp,temp);//temp=t0+(-temp)
    copy(t,t0);//t0=t
    copy(temp,t);//t=temp
    multiply(q,s,temp);//temp=qs
    negify(temp,temp);//temp=-temp
    add(s0,temp,temp);//temp=s0+(-temp)
    copy(s,s0);//s0=s
    copy(temp,s);//s=temp
    copy(b0,a0);//a0=b0
    copy(r,b0);//b0=r
    copy(a0,a0t);
    copy(b0,b0t);
    divide(a0t,b0t,q);//q=a0/b0
    multiply(q,b0,r);//r=qb0
    negify(r,r);//r=-r
    add(a0,r,r);//r=a0+(-r)
    }
    copy(b0,r);//r=b0
    return t;
}


big MontMult(big A,big B,big R,big N,long k)
{ big RI=mirvar(0);
  big O=mirvar(1);
  big T=mirvar(0);
  xgcd(R,N,RI,RI,RI);
  multiply(A,B,T);
  multiply(T,RI,T);
  powmod(T,O,N,T);
  return T;
}

long decompose(big x)//将大数n-1分解为2^k乘以一个素数的形式，返回值为k
{
	big temp;
	long k = 0;
	temp = mirvar(0);
	decr(x, 1, temp);//大数减去一个整数，temp=x-1
	while (subdivisible(temp, 2)) {//判断能否整除2
		sftbit(temp, -1, temp);//左移-1位即右移1位，即除以二
		k++;
	}
	mirkill(temp);
	return k;
}

int prime_test(big x)//素性检测，用Miller-Rabin算法
{
	big a, b, m, g, num, temp;
	long i = 0,k = 0;

	//初始化
	a = mirvar(0);
	b = mirvar(0);
	m = mirvar(0);
	g = mirvar(0);
	num = mirvar(1);
	temp = mirvar(2);
	decr(x, 1, g);//g=n-1
	//判断n是否为大于2的奇数，如果是偶数，返回0
	if (subdivisible(x, 2) && (mr_compare(x, temp) == 0)) {
		return 0;
	}
	else {
		//分解n-1
		k = decompose(x);
		decr(x, 1, b);//b=n-1
		sftbit(b, (-1)*k, m);//m=b/2^k，即为约尽2后的素数

		//随机生成一个小于b的大数，判断是否为x的简化剩余系中的元素，当生成一个简化剩余系中的元素时结束循环
		do {
			bigrand(b, a);//a为随机生成的小于b的大数
			egcd(x, a, b);//b=gcd(x,a)
		} while (mr_compare(b, num) != 0);//b=1时返回值为0

		//计算a^m = b (mod x)
		powmod(a, m, x, b);
		if (mr_compare(b, num) == 0)//b=1,判断为素数
		{
			return 1;
		}
		else {
			for (i = 0; i < k; i++)
			{
				if (!mr_compare(g, b))//b=x-1,为素数
					return 1;
				powmod(b, temp, x, b);//计算b^2(mod x)
			}
		}
		mirkill(a);
		mirkill(b);
		mirkill(m);
		mirkill(g);
		mirkill(num);
		mirkill(temp);
		return 0;
	}
}

int prime_judge(big x)//进行times素性检测
{
		long i = 0, sum = 0;
		for (i = 0; i<times; i++) {
			if (prime_test(x)) {
				sum++;
				continue;
			}
			else
				break;
		}
		if (sum == times)//经过times次素性检测每次都为素数
			return 1;
		else
			return 0;
}



big prime_generate(long len)//大素数生成，len为大素数长度
{
	big temp = mirvar(0);//初始化临时大数
	bigdig(len, 2, temp);//随机生成len位10进制数
	while (!prime_judge(temp))//当不是大素数时，再重新生成
		bigdig(len, 2, temp);
	return temp;
}

big e_generate(big eu,long len)
{
	big e = mirvar(0);
	big num = mirvar(1);
	big g = mirvar(0);
	do {
		bigdig(len, 2, e);
		egcd(e, eu, g);//g=gcd(e,eu)
	} while (mr_compare(g, num) != 0);//g=1时返回值为0
	return e;
}
big d_generate(big eu,big e)
{
	big d = mirvar(0);
	//d=Euclid(e,eu);
	xgcd(e, eu, d, d, d);
	return d;
}

big Montgomery(big a, big e, big N,long k)//k为N的位数
{
	big Prod=mirvar(0);
	big A=mirvar(0);
	big O=mirvar(1);
	big Z=mirvar(0);
	big R=mirvar(0);
	R=R_generate(k);
	Prod=Mont(O,N,R);
	A=Mont(a,N,R);
	int flag;
	while (mr_compare(e,Z)!=0)//mr_compare不相等时为1
	{
		flag = subdivisible(e, 2);//判断E是否能除尽2，能返回True
		//printf("%d\n",flag);
		if(flag==FALSE)//为奇数
		{
            Prod=MontMult(Prod,A,R,N,k);
        }
			A=MontMult(A,A,R,N,k);
			sftbit(e,-1,e);

	}
	return MontInv(Prod,N,R);
}


/*big Montgomery(big x, big e, big m)
{
	big X = mirvar(0);
	big E = mirvar(0);
	big Y = mirvar(1);//Y的初始值为1
	big O = mirvar(1);//1
	big Z = mirvar(0);//0
	big M = mirvar(0);//乘积值
	bool flag;//奇偶判断返回值
	copy(x, X);//X的初始值为x
	//cotnum(X, stdout);
	copy(e, E);//E的初始值为e
	//cotnum(E, stdout);
	//i=mr_compare(E,Z)+5;
	//printf("%d\n",i);
	while (mr_compare(E,Z)!=0)//mr_compare不相等时为1
	{
		flag = subdivisible(E, 2);//判断E是否能除尽2，能返回True
		//printf("%d\n",flag);
		if(flag==FALSE)//为奇数
		{
            multiply(X, Y, M);//M等于X*Y
			powmod(M, O, m, Y);//Y等于M mod m
			decr(E, 1, E);//E=E-1
		}
		else//偶数
		{
			multiply(X, X, M);//M等于X的平方
			powmod(M, O, m, X);//X等于M mod m
			subdiv(E, 2, E);//E=E/2
		}
	}

	return Y;
}*/

big china(big x, big e, big p,big q)
{
	big pt = mirvar(0);
	big qt = mirvar(0);
	big ptv = mirvar(0);
	big qtv = mirvar(0);
	big r = mirvar(0);
	big f = mirvar(0);
	big pq = mirvar(0);
	big num = mirvar(1);
	powmod(x, e, p, pt);//x^e=pt(mod p)
	powmod(x, e, q, qt);//x^e=qt(mod q)
	//ptv=Euclid(q,p);
	xgcd(q, p, ptv, ptv, ptv);//ptv=q^-1(mod p)
	//qtv=Euclid(p,q);
	xgcd(p, q, qtv,qtv, qtv);//v=p^-1(mod q)
	multiply(pt, q, pt);//pt=q*pt
	multiply(pt, ptv, pt);//pt=q*pt*ptv
	multiply(qt, p, qt);//=p*qt;
	multiply(qt, qtv, qt);//=p*qt*qtv;
	multiply(p, q, pq);//pq=p*q
	add(pt, qt, r);//r=pt+qt
	powmod(r, num, pq, f);//f=r(mod pq)
	return f;
}

big Square_Multiply(big x, big e, big m,long b)
{
   long len;//转换成二进制后的数组长度
   long i;
   char t[b+1];
   double start,finish;
   miracl *mip = mirsys(65535, 2);
   big z=mirvar(1);
   big O=mirvar(1);
   big T=mirvar(2);
   mip->IOBASE=2;
   len=cotstr(e,t);
   start=clock();//取开始时间
   for(i=0;i<len;i++)
   {
       powmod(z,T,m,z);
       if(t[i]=='1')
       {
           multiply(z,x,z);
           powmod(z,O,m,z);
       }
   }
   finish=clock();//取结束时间
   printf("CCCCC:%f seconds\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
   mip->IOBASE=16;
   return z;
}

int main()
{
	miracl *mip = mirsys(100000, 2);//初始化miracl函数
	big m = mirvar(0);//明文m
	big c = mirvar(0);//密文c
	big p = mirvar(0);//大素数p
	big q = mirvar(0);//大素数q
	big n = mirvar(0);//n=pq
	big eu = mirvar(0);//欧拉函数值eu = (p - 1)(q - 1)
	big d = mirvar(0);//私钥d  
	big e = mirvar(0);//公钥e
	big pt = mirvar(0);
	big qt = mirvar(0);
	big t=mirvar(0);
	big O=mirvar(1);
    int i;
    long len;
    char temp[10000];
	double start,finish;

	m=prime_generate(256);

    p=prime_generate(1024);
    i=isprime(p);
    if(i==1)
        printf("P is prime!\n");
    q=prime_generate(1024);
    i=isprime(q);
    if(i==1)
        printf("Q is prime!\n");
    multiply(p,q,n);
    decr(p, 1, pt);//pt=p-1
    decr(q, 1, qt);//qt=q-1
    multiply(pt,qt,eu);//eu=(p-1)(q-1)
    e=e_generate(eu,512);
    d=d_generate(eu,e);
    multiply(e,d,t);
    powmod(t,O,n,t);
    if(mr_compare(t,O)==1)
        printf("D is the inverse of E\n");
    mip->IOBASE=16;
    printf("m=   ");
    cotnum(m, stdout);
    printf("P=   ");
    cotnum(p, stdout);
    printf("Q=   ");
    cotnum(q, stdout);
    printf("E=   ");
    cotnum(e, stdout);
    printf("D=   ");
    cotnum(d, stdout);

    big sresult=mirvar(0);
    start=clock();//取开始时间
    sresult=Square_Multiply(m,e,n,512);
    finish=clock();//取结束时间
    printf("Square_Multiply Time:%f seconds\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    printf("Square_Multiply result=   ");
    cotnum(sresult, stdout);

    big cresult=mirvar(0);
    start=clock();//取开始时间
    cresult=china(m,e,p,q);
    finish=clock();//取结束时间
    printf("China Time:%f seconds\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    printf("China result=   ");
    cotnum(cresult, stdout);

    big mresult=mirvar(0);
    //powmod(m,e,n,mresult);
    start=clock();//取开始时间
    mresult=Montgomery(m,e,n,2048);
    printf("Montgomery Time:%f seconds\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    printf("Montgomery result=   ");
    cotnum(mresult, stdout);


    mip->IOBASE=2;
    len=cotstr(d,temp);

    printf("\n\n\n\n\n");
    len=len*4;
    big sd=mirvar(0);
    start=clock();//取开始时间
    sd=Square_Multiply(sresult,d,n,len);
    finish=clock();//取结束时间
    //printf("Square_Multiply Time:%f seconds\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    printf("Square_Multiply result=   ");
    cotnum(sd, stdout);

    big cd=mirvar(0);
    start=clock();//取开始时间
    cd=china(cresult,d,p,q);
    finish=clock();//取结束时间
    ///printf("China Time:%f seconds\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    printf("China result=   ");
    cotnum(cd, stdout);

    big md=mirvar(0);
    //powmod(m,e,n,mresult);
    start=clock();//取开始时间
    md=Montgomery(mresult,d,n,1024);
    //printf("Montgomery Time:%f seconds\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之
    printf("Montgomery result=   ");
    cotnum(md, stdout);

    /*big a, b, c,d,e;
    long i;
    int flag;
    char t[1025];
    char t0[33];

       //mip->IOBASE=10;
    a=mirvar(256);
    //a=prime_generate(1024);
    //mip->IOBASE=2;
    //len=cotstr(a,t);
    //printf("%c\n",t[0]);
    //bigdig(1024,2,a);
    //a=prime_generate(10);

    //bigdig(2,10,a);
    //i=prime_judge(a);
    //printf("%ld\n",i);
    //i=subdivisible(a, 2);
    b=mirvar(256);
    //mip->IOBASE=2;
    //cotstr(a,t);
    //strncpy(t0,t+1024-32*2,32);
    //t0[32]='\0';
    //mip->IOBASE=2;
    //cinstr(b,t0);
    //negify(b,b);
    c=mirvar(30);
    //add(a,b,b);
    //c=Euclid(a,b);
    d=mirvar(0);
    //d=Montgomery(a,b,c);

    //flag = subdivisible(a, 2);
    //e=mirvar(0);
    //printf("%ld\n",i);
    //subdiv(a, 2, a);
    //mip->IOBASE=2;
    //e=china(a,b,c,d);
    //b=e_generate(a,2048);
    //c=d_generate(a,b);
    //d=Square_Multiply(a,b,c,1024);
    //mip->IOBASE=16;
    d=Mont(a,c,b);
    cotnum(a,stdout);
    //d=Montgomery(a,b,c);
    cotnum(b, stdout);
    cotnum(c, stdout);
    cotnum(d, stdout);
    //cotnum(e, stdout);*/
	return 0;
}
    /*int op = 1;
	while (op) {
		system("cls");	printf("\n\n");
		printf("                          RSA                    \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. 生成RSA参数       2. 模重复平方\n");
		printf("    	  3. 中国剩余定理      4. 蒙哥马利算法\n");
		printf("    	  0. 退出\n");
		printf("-------------------------------------------------\n");
		printf("    请选择你的操作[0~4]:");
		scanf("%d", &op);
		switch (op) {
		case 1:
            p=prime_generate(2048);
            q=prime_generate(2048);
            multiply(p,q,n);
            decr(p, 1, pt);//pt=p-1
	        decr(q, 1, qt);//qt=q-1
	        multiply(pt,qt,eu);//eu=(p-1)(q-1)
	        e=e_generate(eu,2048);
            d=d_generate(eu,e);
            cotnum(p, stdout);
            cotnum(q, stdout);
            cotnum(e, stdout);
            cotnum(d, stdout);
			getchar(); getchar();
			break;
		case 2:



			getchar(); getchar();
			break;
		case 3:


Mont
			getchar(); getchar();
			break;
		case 4:



			getchar(); getchar();
			break;
		case 0:
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
	return 0;*/
