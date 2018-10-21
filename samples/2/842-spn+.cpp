#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//加强版SPN
//分组长度:16->64位
//轮数4->16轮
//密钥长度32位->128位
//S盒->8位256长
//P盒->64位置换
//CBC工作模式
//原密钥每次循环左移8位,取64位
//每次使用不同的S盒和P盒

//get轮密钥
//使用P盒将密钥再次编排
unsigned long long getrkey(int *key, int Nr)
{
    unsigned long long rkey=0x0;
    int i,j;
    j=((Nr-1)*8)%128;
    for (i=0;i<64;i++)
    {
        rkey|=key[j];
        rkey<<=(63-i);
        j++;
        if (j==128) j=0;
    }
    return rkey;
}

//S盒代换
unsigned long long SBoxreplacement(unsigned long long cin, int SBox[256])
{
    unsigned long long temp[8];
    int i;
    for (i=0;i<8;i++)
    {
        temp[i]=cin<<i*8;
        temp[i]>>=56;
        temp[i]=SBox[temp[i]];
        temp[i]<<=(7-i)*8;
        if (i>0) temp[i]|=temp[i-1];
    }
    return temp[7];
}

//P盒置换
unsigned long long PBoxreplacement(unsigned long long cin, int PBox[64])
{
    unsigned long long tempcin[64],tempcout[64];
    int i;
    for (i=0;i<64;i++) {tempcin[i]=cin<<i;tempcin[i]>>=63;}
    for (i=0;i<64;i++)
    {
        tempcout[i]=tempcin[PBox[i]];
        tempcout[i]<<=63-i;
        if (i>0) tempcout[i]|=tempcout[i-1];
    }
    return tempcout[63];
}

//明文单元加密
unsigned long long encode(unsigned long long cin, unsigned long long quantity, int *key, int SBox[16][256], int PBox[15][64])
{
    int Nr=16;
    int i;
    unsigned long long rkey;
    cin^=quantity;
    for (i=1;i<Nr;i++)
    {
        rkey=getrkey(key,i);
        rkey=PBoxreplacement(rkey,PBox[i-1]);
        cin^=rkey;
        cin=SBoxreplacement(cin,SBox[i-1]);
        cin=PBoxreplacement(cin,PBox[i-1]);
    }
    cin^=getrkey(key,Nr);
    cin=SBoxreplacement(cin,SBox[Nr-1]);
    cin^=getrkey(key,Nr+1);
    return cin;
}

//密文单元解密
unsigned long long decode(unsigned long long cin, unsigned long long quantity, int *key, int SBox[16][256], int PBox[15][64], int keyPBox[15][64])
{
    int Nr=16;
    int i;
    unsigned long long rkey;
    cin^=getrkey(key,Nr+1);
    cin=SBoxreplacement(cin,SBox[Nr-1]);
    cin^=getrkey(key,Nr);
    for (i=Nr-1;i>0;i--)
    {
        rkey=getrkey(key,i);
        rkey=PBoxreplacement(rkey,keyPBox[i-1]);
        cin=PBoxreplacement(cin,PBox[i-1]);
        cin=SBoxreplacement(cin,SBox[i-1]);
        cin^=rkey;
    }
    cin^=quantity;
    return cin;
}

int main()
{
    int SBox[16][256],PBox[15][64];
    int i,j;
    int key[128];
    unsigned long long unit[10000];
    unsigned long long startquantity=0x0;
    int oppositeSBox[48][256],oppositePBox[47][64];
    unsigned long long unitbit;

    srand((unsigned)time(NULL));
    //生成随机密钥
    for (i=0;i<128;i++) key[i]=rand()%2;
    //生成S盒
    int boolean[256];
    int random;
    for (j=0;j<16;j++)
    {
        for (i=0;i<256;i++) boolean[i]=1;
        i=0;
        while (i<256)
        {
            random=rand()%256;
            if (boolean[random]==1)
            {
                SBox[j][i]=random;
                boolean[random]=0;
                i++;
            }
        }
    }
    //生成P盒
    for (j=0;j<15;j++)
    {
        for (i=0;i<64;i++) boolean[i]=1;
        i=0;
        while (i<64)
        {
            random=rand()%64;
            if (boolean[random]==1)
            {
                PBox[j][i]=random;
                boolean[random]=0;
                i++;
            }
        }
    }
    //生成初始向量
    for (i=0;i<64;i++)
    {
        unitbit=rand()%2;
        unitbit<<=i;
        startquantity|=unitbit;
    }
    //S,P盒求逆
    for (j=0;j<16;j++)
        for (i=0;i<256;i++)
            oppositeSBox[j][SBox[j][i]]=i;
    for (j=0;j<15;j++)
        for (i=0;i<64;i++)
            oppositePBox[j][PBox[j][i]]=i;

    int len;
    scanf("%d",&len);
    printf("\n");
    //生成明文
    for (i=0;i<len;i++)
    {
        unit[i]=0x0;
        for (j=0;j<64;j++)
        {
            unitbit=rand()%2;
            unitbit<<=j;
            unit[i]|=unitbit;
        }
    }
    //打印原始明文
    for (i=0;i<len;i++)
    {
        for (j=0;j<8;j++)
        {
            unitbit=unit[i]<<j*8;
            unitbit>>=56;
            printf("%c",(int)unitbit);
        }
    }
    printf("\n\n");
    //加密
    unit[0]=encode(unit[0],startquantity,key,SBox,PBox);
    for (i=1;i<len;i++) unit[i]=encode(unit[i],unit[i-1],key,SBox,PBox);
    //打印密文
    for (i=0;i<len;i++)
    {
        for (j=0;j<8;j++)
        {
            unitbit=unit[i]<<j*8;
            unitbit>>=56;
            printf("%c",(int)unitbit);
        }
    }
    printf("\n\n");
    //解密
    for (i=len;i>0;i--) unit[i]=decode(unit[i],unit[i-1],key,oppositeSBox,oppositePBox,PBox);
    unit[0]=decode(unit[0],startquantity,key,oppositeSBox,oppositePBox,PBox);
    //打印密文解密
    for (i=0;i<len;i++)
    {
        for (j=0;j<8;j++)
        {
            unitbit=unit[i]<<j*8;
            unitbit>>=56;
            printf("%c",(int)unitbit);
        }
    }
    printf("\n\n");

    return 0;
}
