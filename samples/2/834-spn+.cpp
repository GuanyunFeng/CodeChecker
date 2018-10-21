#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define m 4 //子串个数
#define l 4 //子串长度
#define Nr 4//轮数
#define random(x) (rand()%x)
#define T 500//用于差分分析的明密文对
int allround_key[16*(Nr+1)+1];//轮密钥
int s0[]={0,0,0,0};
int s1[]={0,0,0,1};
int s2[]={0,0,1,0};
int s3[]={0,0,1,1};
int s4[]={0,1,0,0};
int s5[]={0,1,0,1};
int s6[]={0,1,1,0};
int s7[]={0,1,1,1};
int s8[]={1,0,0,0};
int s9[]={1,0,0,1};
int s10[]={1,0,1,0};
int s11[]={1,0,1,1};
int s12[]={1,1,0,0};
int s13[]={1,1,0,1};
int s14[]={1,1,1,0};
int s15[]={1,1,1,1};

void Xor(int* binary,int round)
{
    int i;
    for(i=0;i<16;i++)
    {
        binary[i]=binary[i]^allround_key[16*round+i];
    }
    return;
}

void Xor4(int* v,int* L,int* y)
{
    int i;
    for(i=0;i<4;i++)
    {
        v[i]=L[i]^y[i];
    }
    return;
}
void Xor16(int* v,int* L,int* y)
{
    int i;
    for(i=0;i<16;i++)
    {
        v[i]=L[i]^y[i];
    }
    return;
}
int Xorz(int* x,int* u2,int* u4)
{
    int z;
    z=x[4]^x[6]^x[7]^u2[1]^u2[3]^u4[1]^u4[3];
    return z;
}

int Array_Equal_Judge(int *g,int* s,int b)//判断两个整数数组是否相同
{
    int i;
    int flag=1;
    for(i=0;i<b;i++)
    {
        if(g[i]!=s[i])
        {
            flag=0;
            break;
        }
    }
    return flag;
}
int Array_Equal_Judge_16(int *g,int* s)//判断两个整数数组是否相同
{
    int i;
    int flag=1;
    for(i=0;i<16;i++)
    {
        if(g[i]!=s[i])
        {
            flag=0;
            break;
        }
    }
    return flag;
}

void int_strncpy(int* g,int* s,int size)//整数strncpy
{
    int i;
    for(i=0;i<size;i++)
        g[i]=s[i];
    return;
}
void sub(int* g)//s盒子代换
{
  if(Array_Equal_Judge(g,s0,4)==1)
    {int_strncpy(g,s14,4);return;}
  if(Array_Equal_Judge(g,s1,4)==1)
    {int_strncpy(g,s4,4);return;}
  if(Array_Equal_Judge(g,s2,4)==1)
    {int_strncpy(g,s13,4);return;}
  if(Array_Equal_Judge(g,s3,4)==1)
    {int_strncpy(g,s1,4);return;}
  if(Array_Equal_Judge(g,s4,4)==1)
    {int_strncpy(g,s2,4);return;}
  if(Array_Equal_Judge(g,s5,4)==1)
    {int_strncpy(g,s15,4);return;}
  if(Array_Equal_Judge(g,s6,4)==1)
    {int_strncpy(g,s11,4);return;}
  if(Array_Equal_Judge(g,s7,4)==1)
    {int_strncpy(g,s8,4);return;}
  if(Array_Equal_Judge(g,s8,4)==1)
    {int_strncpy(g,s3,4);return;}
  if(Array_Equal_Judge(g,s9,4)==1)
    {int_strncpy(g,s10,4);return;}
  if(Array_Equal_Judge(g,s10,4)==1)
    {int_strncpy(g,s6,4);return;}
  if(Array_Equal_Judge(g,s11,4)==1)
    {int_strncpy(g,s12,4);return;}
  if(Array_Equal_Judge(g,s12,4)==1)
    {int_strncpy(g,s5,4);return;}
  if(Array_Equal_Judge(g,s13,4)==1)
    {int_strncpy(g,s9,4);return;}
  if(Array_Equal_Judge(g,s14,4)==1)
    {int_strncpy(g,s0,4);return;}
  if(Array_Equal_Judge(g,s15,4)==1)
    {int_strncpy(g,s7,4);return;}
}
void Sbox(int* binary)
{
    int g1[5];
    int g2[5];
    int g3[5];
    int g4[5];
    int_strncpy(g1,binary,4);
    int_strncpy(g2,binary+4,4);
    int_strncpy(g3,binary+8,4);
    int_strncpy(g4,binary+12,4);
    sub(g1);
    sub(g2);
    sub(g3);
    sub(g4);
    int_strncpy(binary,g1,4);
    int_strncpy(binary+4,g2,4);
    int_strncpy(binary+8,g3,4);
    int_strncpy(binary+12,g4,4);
    return;
}

void Pbox(int* binary)
{
    int temp[17];
    int_strncpy(temp,binary,16);
    binary[0]=temp[0];
    binary[1]=temp[4];
    binary[2]=temp[8];
    binary[3]=temp[12];
    binary[4]=temp[1];
    binary[5]=temp[5];
    binary[6]=temp[9];
    binary[7]=temp[13];
    binary[8]=temp[2];
    binary[9]=temp[6];
    binary[10]=temp[10];
    binary[11]=temp[14];
    binary[12]=temp[3];
    binary[13]=temp[7];
    binary[14]=temp[11];
    binary[15]=temp[15];
    return;
}

void Roundkey_Generation(int key[])//密钥生成算法
{
  int oneround_key[17];
  int i,j,k;
  for(i=0;i<=Nr;i++)
  {
    for(j=0;j<=15;j++)
    {
        oneround_key[j]=key[4*i+j];
    }
    for(k=16*i;k<16*(i+1);k++)
    {
       allround_key[k]=oneround_key[k%16];
    }
  }
  return;
}


void Spn_Encryption(int* clear_text)//对两个字符进行操作
{
  int i;
  for(i=0;i<(Nr-1);i++)
  {
      Xor(clear_text,i);
      Sbox(clear_text);
      Pbox(clear_text);
  }
  Xor(clear_text,Nr-1);
  Sbox(clear_text);
  Xor(clear_text,Nr);
  return ;
}

void sub_reserve(int* g)
{
  if(Array_Equal_Judge(g,s0,4)==1)
    {int_strncpy(g,s14,4);return;}
  if(Array_Equal_Judge(g,s1,4)==1)
    {int_strncpy(g,s3,4);return;}
  if(Array_Equal_Judge(g,s2,4)==1)
    {int_strncpy(g,s4,4);return;}
  if(Array_Equal_Judge(g,s3,4)==1)
    {int_strncpy(g,s8,4);return;}
  if(Array_Equal_Judge(g,s4,4)==1)
    {int_strncpy(g,s1,4);return;}
  if(Array_Equal_Judge(g,s5,4)==1)
    {int_strncpy(g,s12,4);return;}
  if(Array_Equal_Judge(g,s6,4)==1)
    {int_strncpy(g,s10,4);return;}
  if(Array_Equal_Judge(g,s7,4)==1)
    {int_strncpy(g,s15,4);return;}
  if(Array_Equal_Judge(g,s8,4)==1)
    {int_strncpy(g,s7,4);return;}
  if(Array_Equal_Judge(g,s9,4)==1)
    {int_strncpy(g,s13,4);return;}
  if(Array_Equal_Judge(g,s10,4)==1)
    {int_strncpy(g,s9,4);return;}
  if(Array_Equal_Judge(g,s11,4)==1)
    {int_strncpy(g,s6,4);return;}
  if(Array_Equal_Judge(g,s12,4)==1)
    {int_strncpy(g,s11,4);return;}
  if(Array_Equal_Judge(g,s13,4)==1)
    {int_strncpy(g,s2,4);return;}
  if(Array_Equal_Judge(g,s14,4)==1)
    {int_strncpy(g,s0,4);return;}
  if(Array_Equal_Judge(g,s15,4)==1)
    {int_strncpy(g,s5,4);return;}
}

void Sbox_Reserve(int* binary)
{
    int g1[5];
    int g2[5];
    int g3[5];
    int g4[5];
    int_strncpy(g1,binary,4);
    int_strncpy(g2,binary+4,4);
    int_strncpy(g3,binary+8,4);
    int_strncpy(g4,binary+12,4);
    sub_reserve(g1);
    sub_reserve(g2);
    sub_reserve(g3);
    sub_reserve(g4);
    int_strncpy(binary,g1,4);
    int_strncpy(binary+4,g2,4);
    int_strncpy(binary+8,g3,4);
    int_strncpy(binary+12,g4,4);
    return;
}

void Pbox_Reserve(int* binary)
{
    int temp[17];
    int_strncpy(temp,binary,16);
    binary[0]=temp[0];
    binary[1]=temp[4];
    binary[2]=temp[8];
    binary[3]=temp[12];
    binary[4]=temp[1];
    binary[5]=temp[5];
    binary[6]=temp[9];
    binary[7]=temp[13];
    binary[8]=temp[2];
    binary[9]=temp[6];
    binary[10]=temp[10];
    binary[11]=temp[14];
    binary[12]=temp[3];
    binary[13]=temp[7];
    binary[14]=temp[11];
    binary[15]=temp[15];
    return;
}

void Spn_Decryption(int* cipher_text)//对两个字符进行操作
{
  int i;
  Xor(cipher_text,Nr);
  Sbox_Reserve(cipher_text);
  Xor(cipher_text,Nr-1);
  for(i=Nr-2;i>=0;i--)
  {
      Pbox_Reserve(cipher_text);
      Sbox_Reserve(cipher_text);
      Xor(cipher_text,i);
  }
  return ;
}
void  X_Bit_Padding(char* string,int b)//将二进制字符串填充到x位
{
    int len;
    int difference;
    int i;
    char* temp;
    temp=(char*)malloc((b+1)*sizeof(char));
    len=strlen(string);
    if(len!=b)
    {
        difference=b-len;
        for(i=0;i<difference;i++)
            temp[i]='0';
        temp[difference]='\0';
        strncpy(temp+difference,string,len);
        temp[b]='\0';
        strncpy(string,temp,b);
        string[b]='\0';
    }
    return;
}

void num_to_intstr(int num,int* string)
{    char temp[9];
     int j;
     itoa(num,temp,2);
     X_Bit_Padding(temp,8);
     for(j=0;j<8;j++)
      {
       string[j]=(unsigned char)temp[j]-48;
      }
    return;
}
void num_to_intstr16(unsigned int num,int* string)//将整数转换为字符串
{    char temp[17];
     int j;
     itoa(num,temp,2);
     X_Bit_Padding(temp,16);
     for(j=0;j<16;j++)
      {
       string[j]=(unsigned char)temp[j]-48;
      }
    return;
}
void ordinary(int*k,int*k5)
{
  int_strncpy(k+20,k5,4);
  int_strncpy(k+28,k5+4,4);
  return;
}

void satis(int* x,int* xx)
{   int_strncpy(xx,x,16);
    int i;
    for(i=0;i<4;i++)
    {
        xx[i+4]=x[i+4]^s11[i];
    }
    return;
}

int main()
{
    int i,j;
    double start,finish;
    int key[32]={0,0,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,1,1};//初始密钥数组，32位二进制
    Roundkey_Generation(key);

    unsigned short ran;

    int x[17],y[17],xx[17],yx[17],count[256],L[16],L1[8],L2[4],v2[4],v4[4],u2[4],u4[4],v2x[4],v4x[4],u2x[4],u4x[4],maxkey,max,k[32],k5[8];
    int u2p[4],u4p[4];
    char rans[17];
    memset(count,0,sizeof(int)*256);
    memset(k,0,sizeof(int)*32);
    srand((unsigned)(time(NULL)));
    start=clock();
    for(i=0;i<T;i++)
    {

     ran=random(65536);
     itoa(ran,rans,2);
     X_Bit_Padding(rans,16);
     for(j=0;j<16;j++)
      {
       x[j]=(unsigned char)rans[j]-48;
       //printf("%d",x[j]);
       y[j]=x[j];
      }
     Spn_Encryption(y);//y为加密后的数组

     //printf("\n");

     satis(x,xx);
     for(j=0;j<16;j++)
      {
       //printf("%d",xx[j]);
       yx[j]=xx[j];
      }

     Spn_Encryption(yx);//y为加密后的数组


     //printf("\n");
    /* int_strncpy(y1,y,4);
     int_strncpy(y1x,yx,4);
     int_strncpy(y3,y+8,4);
     int_strncpy(y3x,yx+8,4);*/

     if(Array_Equal_Judge(y,yx,4)&&Array_Equal_Judge(y+8,yx+8,4))
     {
      for(j=0;j<256;j++)
      {  num_to_intstr(j,L);
         int_strncpy(L1,L,4);
         int_strncpy(L2,L+4,4);

         Xor4(v2,L1,y+4);
         Xor4(v4,L2,y+12);
         int_strncpy(u2,v2,4);
         int_strncpy(u4,v4,4);
         sub_reserve(u2);
         sub_reserve(u4);

         Xor4(v2x,L1,yx+4);
         Xor4(v4x,L2,yx+12);
         int_strncpy(u2x,v2x,4);
         int_strncpy(u4x,v4x,4);
         sub_reserve(u2x);
         sub_reserve(u4x);

         Xor4(u2p,u2,u2x);
         Xor4(u4p,u4,u4x);

         if(Array_Equal_Judge(u2p,s6,4)&&Array_Equal_Judge(u4p,s6,4))
            count[j]++;
      }
     }

    }

    max=-1;
    for(j=0;j<256;j++)
      { //printf("%d\n",count[j]);
        if(count[j]>max)
        {
            max=count[j];
            maxkey=j;
        }
      }
    finish=clock();
    printf("%d\n",maxkey);
    printf("差分分析用时：%f seconds\n\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之


    num_to_intstr(maxkey,k5);
    ordinary(k,k5);

    int a[160],b[160],L8[8],L16[16],temp[16],t,p;//随机生成十组明密文对

     for(i=0;i<10;i++)
    {
     ran=random(65536);
     itoa(ran,rans,2);
     //printf("%s\n",rans);
     X_Bit_Padding(rans,16);
     //printf("%s",rans);
     for(j=0;j<16;j++)
      {
       a[16*i+j]=(unsigned char)rans[j]-48;
       //printf("%d",a[16*i+j]);
       b[16*i+j]=a[16*i+j];
      }
      //printf("\n");
      Spn_Encryption(b+16*i);
       /* for(j=0;j<16;j++)
      {

       printf("%d",b[16*i+j]);
      }
      printf("\n");*/
    }
    start=clock();
        //i=14996,j=211;
    for(i=0;i<65536;i++)
    {
        for(j=0;j<256;j++)
        {
          //sum=0;
          num_to_intstr16(i,L16);
          num_to_intstr(j,L8);
          int_strncpy(k,L16,16);
          int_strncpy(k+16,L8,4);
          int_strncpy(k+24,L8+4,4);
          Roundkey_Generation(k);
          for(t=0;t<10;t++)
          {
            int_strncpy(temp,a+16*t,16);
            Spn_Encryption(temp);
            if(Array_Equal_Judge_16(temp,b+16*t)!=1)
                break;
            if(t==9)
               printf("破解出的密钥为：");
              for(p=0;p<32;p++)
                printf("%d",k[p]);
              printf("\n");
              return 0;
          }
        }
    }
    finish=clock();
    printf("暴力破解用时：%f seconds\n\n",(finish-start)/CLOCKS_PER_SEC);//以秒为单位显示之



    return 0;
}
