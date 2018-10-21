#include <stdio.h>
#include <stdlib.h>
typedef unsigned short B16;
B16 sre(B16 a,int* s);
B16 pre(B16 a,int* p);
B16 encrypt(B16 a,int* s,int* p,B16* t);
B16 encode(B16 a,int* s,int* p,B16* t);
void p16(B16 a);
int main()
{
    B16 x = 0x26b7;// dbc8
    B16 k1 = 0x3a94,k2 = 0xd63f;
    B16 t[5];
    B16 a,b;
    int s[16] = {0xe,0x4,0xd,0x1, 0x2,0xf,0xb,0x8, 0x3,0xa,0x6,0xc, 0x5,0x9,0x0,0x7};
    int p[16] = {1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
    int i,j;

        for (i=0;i<5;i++)
    {
        j=4*i;
        t[i]=(k1<<j)|(k2>>16-j);
        printf("轮秘钥t%d :",i+1);p16 (t[i]);
    }

    printf("K: ");
    p16(k1);p16(k2);
    printf("P: ");
    p16 (x);
    printf("加密：\n");
    b=encrypt(x,s,p,t);

    printf("\n");
    printf("解密：\n");
    b=encode(b,s,p,t);








    return 0;
}

B16 sre(B16 a,int* s)
{
    B16 e = 0x0000;
    B16 b = a;
    int i;
    for (i=0;i<4;i++)
    {
        b=a;
        b>>=4*i;
        b=b&0x000f;
        b=s[b];
        b<<=4*i;
        e=e|b;
    }
    return e;
}

B16 pre(B16 a,int* p)
{
    int e[16];
    B16 b=0x0001;
    B16 c;
    int i,j;
    for (i=0;i<16;i++)
    {
        c=a&b;
        j=p[15-i];
        e[j-1]=(int)c;
        a>>=1;
    }
    b=0x0000;
    for (i=0;i<4;i++)
    {
        b<<=4;
        j=4*i;
        b|=e[j]*8+e[j+1]*4+e[j+2]*2+e[j+3];
    }
    return b;
}

void p16(B16 a)
{
    int i;
    int b;
    for (i=0;i<16;i++)
    {
        b=(a>>(15-i))&0x0001;
        printf("%d",b);
        if ((i+1)%4==0) printf(" ");
    }
    printf("\n");
}

B16 encrypt(B16 a,int* s,int* p,B16* t)
{
    B16 b;
    int i;
    b=a;
    for (i=0;i<3;i++)
    {
        b=b^t[i];  p16 (b);
        b=sre(b,s);  p16 (b);
        b=pre(b,p);  p16 (b);printf("\n");
    }
    b=b^t[3];  p16 (b);
    b=sre(b,s);  p16 (b);
    b=b^t[4];  p16 (b);
    return b;
}

B16 encode(B16 a,int* s,int* p,B16* t)
{
    B16 b;
    int i,j;
    int z[16];int x[16];

    b=a;
    for (i=0;i<16;i++)
    {
        j=s[i];
        x[j]=i;
        j=p[i]-1;
        z[j]=i+1;
    }
    b=b^t[4];  p16 (b);
    b=sre(b,x);  p16 (b);
    b=b^t[3];  p16 (b);printf("\n");
    for (i=0;i<3;i++)
    {
        b=pre(b,z);  p16 (b);
        b=sre(b,x);  p16 (b);
        b=b^t[2-i];  p16 (b);printf("\n");
    }
    return b;
}


