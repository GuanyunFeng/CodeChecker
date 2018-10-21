#include <stdio.h>
#include <stdlib.h>
#define Nr 16
#define K_ini 0x3a94d63f98564782;
unsigned short S[16][16]={
            {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
			{0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
			{0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
			{0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},

			{0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
			{0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
			{0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
			{0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},

			{0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
			{0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
			{0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
			{0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},

			{0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
			{0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
			{0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
			{0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
        };
unsigned short PBox[16]={1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
unsigned short Si[16][16]={0};//S盒子的逆
unsigned long long key[Nr+1]={0};
unsigned long long w=0;
unsigned long long u=0;
unsigned long long v=0;
unsigned long long SPN_S (unsigned long long Pro);//S盒子替代
unsigned long long SPN_P (unsigned long long Pro);//P盒子置换
unsigned long long SPN_Si (unsigned long long Pro);//S盒子逆替代
unsigned long long SPN_Pi (unsigned long long Pro);//P盒子逆置换
void Creat_Si (unsigned short S[16][16],unsigned short Si[16][16]);//生成S盒子逆替代
void Creat_key (unsigned long long K,unsigned long long key[Nr+1]);//密钥编排方案
unsigned long long SPN_EN(unsigned long long x,unsigned long long K);
unsigned long long SPN_DE(unsigned long long y,unsigned long long K);
unsigned long long bit_to_num1(unsigned short bit[64]);
void num_to_bit1(unsigned short bit[64],unsigned long long pro);
unsigned short bit_to_num(unsigned short bit[16]);
void num_to_bit(unsigned short bit[16],unsigned short pro);
unsigned long long P_i(unsigned short pro);
int main()
{
    unsigned int op=1;
    unsigned long long X,Y;
    unsigned long long K=K_ini;
    system("color 07");
    while(op)
    {
        system("cls");	printf("\n\n");
        printf("                SPN_EXTERN_TEST \n");
        printf("-------------------------------------------------\n");
	    printf("    	     1. Extern_Encrypt\n\n");
	    printf("    	     2. Extern_Decrypt\n\n");
	    printf("    	     0. Quit\n");
	    printf("-------------------------------------------------\n");
	    printf("\t请选择你的操作[0~2]: ");
	    scanf("%d",&op);
	    getchar();
	    switch(op)
	    {
             case 1:
                 printf("明文 X:0x");
                 scanf("%llx",&X);
                 Y=SPN_EN(X,K);
                 printf("密文 Y:0x%llx\n",Y);
                 system("pause");
                 op=1;
		         break;
             case 2:
                 //printf("密文 Y:0x");
                 //scanf("%llx",&Y);
                 X=SPN_DE(Y,K);
                 printf("明文 X:0x%llx\n",X);
                 system("pause");
                 op=1;
		         break;
             case 0:
                printf("\nWelcome to use again!\n");
                system("pause");
                break;
             default:
                printf("\nERROR!\n");
                system("pause");
                break;
	    }
    }
    return 0;
}
void Creat_Si(unsigned short S[16][16],unsigned short Si[16][16])
{
    unsigned short i,j,temp,th,tl;
    for(i=0;i<16;i++)
    {
        for(j=0;j<16;j++)            //举例说明：S[1][3]=0x23;
        {                            //          则Si[2][3]=0x13;
            temp=S[i][j];
            th=(temp & 0x00f0)>>4;
            tl=temp & 0x000f;
            temp=(i<<4)|j;
            Si[th][tl]=temp;
        }
    }
}
unsigned long long SPN_EN(unsigned long long x,unsigned long long K)
{
    unsigned long long y;
    int i;
    Creat_key(K,key);
    w=x;
    for(i=1;i<=Nr-1;i++)
    {
        u=key[i-1]^w;
        v=SPN_S(u);
        w=SPN_P(v);
    }
        u=key[Nr-1]^w;
        v=SPN_S(u);
        y=key[Nr]^v;
        return y;
}
unsigned long long SPN_DE(unsigned long long y,unsigned long long K)
{
    unsigned long long x;
    int i;
    Creat_key(K,key);
    Creat_Si(S,Si);
    v=key[16]^y;
    u=SPN_Si(v);
    w=key[15]^u;
    for(i=1;i<=Nr-1;i++)
    {
        v=SPN_Pi(w);
        u=SPN_Si(v);
        w=key[Nr-i-1]^u;
    }
    x=w;
        return x;
}
/*将64bit明文分为8组代换*/
unsigned long long SPN_S (unsigned long long Pro)
{
    int i;
    unsigned long long res=0;
    unsigned long long t0[8]={0xff00000000000000,0x00ff000000000000,0x0000ff0000000000,0x000000ff00000000,
    0x00000000ff000000,0x0000000000ff0000,0x000000000000ff00,0x00000000000000ff};
    unsigned long long t[8];
    unsigned short low,high,temp;
    for(i=0;i<8;i++)
    {
        t[i]=(Pro & t0[i])>>(56-i*8);
    }
    for(i=0;i<8;i++)
    {
        low=(t[i] & 0x0f);
        high=(t[i] & 0xf0)>>4;
        temp=S[high][low];
        t[i]=(unsigned long long)temp;
        t[i]=t[i] << (56-i*8);
        res=res|t[i];
    }
    return res;
}
unsigned long long SPN_Si (unsigned long long Pro)
{
    char i;
    unsigned long long res=0;
    unsigned long long t0[8]={0xff00000000000000,0x00ff000000000000,0x0000ff0000000000,0x000000ff00000000,
    0x00000000ff000000,0x0000000000ff0000,0x000000000000ff00,0x00000000000000ff};
    unsigned long long t[8];
    unsigned short low,high,temp;
    for(i=0;i<8;i++)
    {
        t[i]=(Pro & t0[i])>>(56-i*8);
    }
    for(i=0;i<8;i++)
    {
        low=(t[i] & 0x0f);
        high=(t[i] & 0xf0)>>4;
        temp=Si[high][low];
        t[i]=(unsigned long long)temp;
        t[i]=t[i] << (56-i*8);
        res=res|t[i];
    }
    return res;
}
unsigned long long SPN_P (unsigned long long Pro)//p盒置换，将64位数据分为8组
{
    unsigned long long b[4];//处理字节相当于P[8]={4,8,7,3,2,6,5,1}
    unsigned long long t[8];
    t[0]=0xff00000000000000 & Pro; t[4]=0x00000000ff000000 & Pro;
    t[1]=0x00ff000000000000 & Pro; t[5]=0x0000000000ff0000 & Pro;
    t[2]=0x0000ff0000000000 & Pro; t[6]=0x000000000000ff00 & Pro;
    t[3]=0x000000ff00000000 & Pro; t[7]=0x00000000000000ff & Pro;

	Pro=(t[0]>>56)|(t[1]>>24)|(t[2]>>8)|(t[3]<<24)|(t[4]>>16)|(t[5])|(t[6]<<32)|(t[7]<<48);
	b[0]=(0x000000000000ffff&Pro);
	b[1]=(0x00000000ffff0000&Pro)>>16;
	b[2]=(0x0000ffff00000000&Pro)>>32;
	b[3]=(0xffff000000000000&Pro)>>48;
	Pro=(P_i(b[3])<<48)|(P_i(b[2])<<32)|(P_i(b[1])<<16)|P_i(b[0]);
	return Pro;
}
unsigned long long SPN_Pi (unsigned long long Pro)//p盒逆置换，将64位数据分为8组
{
    unsigned long long b[4];//处理字节相当于Pi[8]={8,5,4,1,7,6,3,2}
    unsigned long long t[8];

    b[0]=(0x000000000000ffff&Pro);
	b[1]=(0x00000000ffff0000&Pro)>>16;
	b[2]=(0x0000ffff00000000&Pro)>>32;
	b[3]=(0xffff000000000000&Pro)>>48;
	Pro=(P_i(b[3])<<48)|(P_i(b[2])<<32)|(P_i(b[1])<<16)|P_i(b[0]);

    t[0]=0xff00000000000000 & Pro; t[4]=0x00000000ff000000 & Pro;
    t[1]=0x00ff000000000000 & Pro; t[5]=0x0000000000ff0000 & Pro;
    t[2]=0x0000ff0000000000 & Pro; t[6]=0x000000000000ff00 & Pro;
    t[3]=0x000000ff00000000 & Pro; t[7]=0x00000000000000ff & Pro;

	Pro=(t[0]>>24)|(t[1]>>48)|(t[2]>>32)|(t[3]<<8)|(t[4]<<24)|(t[5])|(t[6]<<16)|(t[7]<<56);
	return Pro;
}
/*P盒子*/
unsigned long long P_i(unsigned short pro)
{
    int i;
    unsigned short  P_temp[16]={0};
    unsigned short  p_bit[16]={0};
    num_to_bit(P_temp,pro);
    for(i=0;i<16;i++)
    {
        p_bit[i]=P_temp[PBox[i]-1];
    }
    pro=bit_to_num(p_bit);
    pro=(unsigned long long)pro;
    return pro;
}
/*2进制串转化为16进制数*/
unsigned long long bit_to_num1(unsigned short bit[64])
{
    unsigned long long num=0;
     int i;
    for(i=0;i<64;i++)
    {
        num=num*2+bit[i];
    }
    return num;
}

/*16进制数转化为2进制串*/
void num_to_bit1(unsigned short bit[64],unsigned long long pro)
{
    int i;
    for(i=0;i<64;i++)
    {
        bit[i]=0;
    }
    i=63;
    unsigned long long  con;//商
    do
    {
        con=pro/2;
        bit[i]=pro%2;
        pro=pro/2;
        i--;
    }
    while(con&&(i>=0));
}
void Creat_key (unsigned long long K,unsigned long long key[Nr+1])
{
    int i,j;
    unsigned short P_K[64]={57,49,41,33,25,17,9,8,1,58,50,42,34,26,18,16,10,2,59,51,43,35,27,24,19,11,3,60,52,44,36,32,63,55,47,39,31,23,15,40,7,62,54,46,38,30,22,48,14,6,61,53,45,37,29,56,21,13,5,28,20,12,4,64};
    K=K_ini;
    unsigned long long KL,KR;
    unsigned short P_temp[64]={0};
    unsigned short p_bit[64]={0};
    num_to_bit1(P_temp,K);
    for(i=0;i<64;i++)
    {
        p_bit[i]=P_temp[P_K[i]-1];
    }
    K=bit_to_num1(p_bit);//初始置换；
    KL=K&0xffffffff00000000;
    KR=K&0x00000000ffffffff;
    KL=KL<<1;
    KR=KR<<1;
    key[0]=KL|KR;
    KL=KL<<1;
    KR=KR<<1;
    key[1]=KL|KR;
    for(i=2;i<=8;i++)
    {
        KL=KL<<1;
        KR=KR<<1;
        key[i]=KL|KR;
    }
    for(i=9;i<=15;i++)
    {
        KL=KL<<2;
        KR=KR<<2;
        key[i]=KL|KR;
    }
    key[16]=K;
}
unsigned short bit_to_num(unsigned short bit[16])
{
    unsigned short num=0;
     int i;
    for(i=0;i<16;i++)
    {
        num=num*2+bit[i];
    }
    return num;
}

/*16进制数转化为2进制串*/
void num_to_bit(unsigned short bit[16],unsigned short pro)
{
    int i;
    for(i=0;i<16;i++)
    {
        bit[i]=0;
    }
    i=15;
    unsigned short con;//商
    do
    {
        con=pro/2;
        bit[i]=pro%2;
        pro=pro/2;
        i--;
    }
    while(con&&(i>=0));
}
