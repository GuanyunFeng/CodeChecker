#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
参照教材实例QQQ
16位输入
32位密钥key=0011 1010 1001 0100 1101 0110 0011 1111
代换规则:
0 1 2 3 4 5 6 7 8 9 a b c d e f
e 4 d 1 2 f b 8 3 a 6 c 5 9 0 7
置换规则：
0 1 2 3 4 5 6 7 8 9 a b c d e f
0 4 8 c 1 5 9 d 2 6 a e 3 7 b f
*/

//二进制对应书上演示

//获取轮密钥
unsigned short getrkey(unsigned long key, int r)
{
    //循环左移r位
    r%=32;
    unsigned long temp=key>>(32-r);
    key<<=r;
    key|=temp;
    //获取轮密钥
    return key>>16;
}

//S盒代换,const防止被调函数修改传入数组
unsigned short SBoxreplacement(unsigned short cin,const unsigned short *SBox)
{
    int i;
    unsigned short temp[4];
    for (i=0;i<4;i++)
    {
        temp[i]=cin<<4*i;
        temp[i]>>=12;
        temp[i]=SBox[temp[i]];
        temp[i]<<=4*(3-i);
        if (i>0) temp[i]|=temp[i-1];
    }
    return temp[3];
}

//P盒置换
unsigned short PBoxrepalcement(unsigned short cin,const unsigned short *PBox)
{
    unsigned short tempcin[16],tempcout[16];
    int i;
    for (i=0;i<16;i++) {tempcin[i]=cin<<i;tempcin[i]>>=15;}
    for (i=0;i<16;i++)
    {
        tempcout[i]=tempcin[PBox[i]];
        tempcout[i]<<=15-i;
        if (i>0) tempcout[i]|=tempcout[i-1];
    }
    return tempcout[15];
}

//加密单元加密
unsigned short encode(unsigned short cin,unsigned long key,const unsigned short *SBox,const unsigned short *PBox)
{
    int Nr=4;//轮数
    int j;
    for (j=1;j<=Nr-1;j++)
    {
        cin^=getrkey(key,4*(j-1));
        cin=SBoxreplacement(cin,SBox);
        cin=PBoxrepalcement(cin,PBox);
    }
    cin^=getrkey(key,4*(Nr-1));
    cin=SBoxreplacement(cin,SBox);
    cin^=getrkey(key,4*Nr);
    return cin;
}

int main()
{
    //变量定义
    unsigned long key=0x3a94d63f;//密钥
    unsigned short SBox[16]={0xe,0x4,0xd,0x1,0x2,0xf,0xb,0x8,0x3,0xa,0x6,0xc,0x5,0x9,0x0,0x7};//S盒
    unsigned short PBox[16]={0x0,0x4,0x8,0xc,0x1,0x5,0x9,0xd,0x2,0x6,0xa,0xe,0x3,0x7,0xb,0xf};//P盒
    int i,j;//循环变量
    int Nr=4;//轮数
    int len;//16位明密文长度
    unsigned short oppositeSBox[16],oppositePBox[16];//S盒逆代换,P盒逆置换
    unsigned short unit[10000];
    unsigned short inth;

    srand((unsigned)time(NULL));
    //S,P盒求逆
    for (i=0;i<16;i++)
    {
        oppositeSBox[SBox[i]]=i;
        oppositePBox[PBox[i]]=i;
    }

    //二进制书上演示
    printf("Input: 0x");
    scanf("%x",&unit[0]);
    printf("\n");
    unit[0]=encode(unit[0],key,SBox,PBox);
    printf("Output: 0x");
    printf("%x\n\n",unit[0]);

    //自由加解密
    scanf("%d",&len);
    printf("\n");
    for (i=0;i<len;i++)
    {
        unit[i]=0x0;
        for (j=0;j<16;j++)
        {
            inth=rand()%2;
            inth<<=j;
            unit[i]|=inth;
        }
    }
    for (i=0;i<len;i++) printf("%c%c",unit[i]>>4,unit[i]);
    printf("\n\n");
    for (i=0;i<len;i++) unit[i]=encode(unit[i],key,SBox,PBox);
    for (i=0;i<len;i++) printf("%c%c",unit[i]>>4,unit[i]);
    printf("\n\n");

    //解密循环
    for (i=0;i<len;i++)
    {
        unit[i]^=getrkey(key,4*Nr);
        unit[i]=SBoxreplacement(unit[i],oppositeSBox);
        unit[i]^=getrkey(key,4*(Nr-1));
        for (j=Nr-1;j>=1;j--)
        {
            unit[i]=PBoxrepalcement(unit[i],oppositePBox);
            unit[i]=SBoxreplacement(unit[i],oppositeSBox);
            unit[i]^=getrkey(key,4*(j-1));
        }
    }
    for (i=0;i<len;i++) printf("%c%c",unit[i]>>4,unit[i]);
    printf("\n\n");

    return 0;
}
