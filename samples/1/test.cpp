#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define m 4 //子串个数
#define l 4 //子串长度
#define Nr 4//轮数
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

void  Eight_Bit_Padding(char* string)//将二进制字符串填充到八位
{
    int len;
    int difference;
    int i;
    char* temp;
    temp=(char*)malloc(9*sizeof(char));
    len=strlen(string);
    if(len!=8)
    {
        difference=8-len;
        for(i=0;i<difference;i++)
            temp[i]='0';
        temp[difference]='\0';
        strncpy(temp+difference,string,len);
        temp[8]='\0';
        strncpy(string,temp,8);
        string[8]='\0';
    }
    return;
}

int* Character_to_Binary(char* clear_text)//对两个字节进行处理，返回值为16位整数串
{ int* binary;
  binary=(int*)malloc(16*sizeof(int));
  char string1[9];
  char string2[9];
  char* string;
  string=(char*)malloc(16*sizeof(char));
  int a,b,i;
  a=(unsigned char)clear_text[0];//得到十进制ascll
  b=(unsigned char)clear_text[1];
  //itoa(a,string1,2);
  Eight_Bit_Padding(string1);
  //itoa(b,string2,2);
  Eight_Bit_Padding(string2);
  strncpy(string,string1,8);
  strncpy(string+8,string2,8);
  string[16]='\0';
     for(i=0;i<16;i++)
   {
       binary[i]=(unsigned char)string[i]-48;
   }
  return binary;
}

void Xor(int* binary,int round)
{
    int i;
    for(i=0;i<16;i++)
    {
        binary[i]=binary[i]^allround_key[16*round+i];
    }
    return;
}

int Array_Equal_Judge(int *g,int* s)
{
    int i;
    int flag=1;
    for(i=0;i<4;i++)
    {
        if(g[i]!=s[i])
        {
            flag=0;
            break;
        }
    }
    return flag;
}

void int_strncpy(int* g,int* s,int size)
{
    int i;
    for(i=0;i<size;i++)
        g[i]=s[i];
    return;
}
void sub(int* g)
{
  if(Array_Equal_Judge(g,s0)==1)
    {int_strncpy(g,s14,4);return;}
  if(Array_Equal_Judge(g,s1)==1)
    {int_strncpy(g,s4,4);return;}
  if(Array_Equal_Judge(g,s2)==1)
    {int_strncpy(g,s13,4);return;}
  if(Array_Equal_Judge(g,s3)==1)
    {int_strncpy(g,s1,4);return;}
  if(Array_Equal_Judge(g,s4)==1)
    {int_strncpy(g,s2,4);return;}
  if(Array_Equal_Judge(g,s5)==1)
    {int_strncpy(g,s15,4);return;}
  if(Array_Equal_Judge(g,s6)==1)
    {int_strncpy(g,s11,4);return;}
  if(Array_Equal_Judge(g,s7)==1)
    {int_strncpy(g,s8,4);return;}
  if(Array_Equal_Judge(g,s8)==1)
    {int_strncpy(g,s3,4);return;}
  if(Array_Equal_Judge(g,s9)==1)
    {int_strncpy(g,s10,4);return;}
  if(Array_Equal_Judge(g,s10)==1)
    {int_strncpy(g,s6,4);return;}
  if(Array_Equal_Judge(g,s11)==1)
    {int_strncpy(g,s12,4);return;}
  if(Array_Equal_Judge(g,s12)==1)
    {int_strncpy(g,s5,4);return;}
  if(Array_Equal_Judge(g,s13)==1)
    {int_strncpy(g,s9,4);return;}
  if(Array_Equal_Judge(g,s14)==1)
    {int_strncpy(g,s0,4);return;}
  if(Array_Equal_Judge(g,s15)==1)
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

char b_to_c(int* g)//将八位二进制数组转换为对应的字符
{
    int decimal;
    char b;
    decimal=g[0]*128+g[1]*64+g[2]*32+g[3]*16+g[4]*8+g[5]*4+g[6]*2+g[7];//8位二进制转换为十进制
    b=(char)decimal;
    return b;
}
char* Binary_to_Character(int* binary)
{   char* string;
    string=(char*)malloc(2*sizeof(char));
    int g1[9];
    int g2[9];
    int_strncpy(g1,binary,8);
    int_strncpy(g2,binary+8,8);
    /*if(Array_Equal_Judge(g1,s0))
        int_strncpy(g1,s9,4);
    if(Array_Equal_Judge(g1,s1))
        int_strncpy(g1,s8,4);
    if(Array_Equal_Judge(g2,s0))
        int_strncpy(g2,s9,4);
    if(Array_Equal_Judge(g2,s1))
        int_strncpy(g2,s8,4);*/

    string[0]=b_to_c(g1);
    string[1]=b_to_c(g2);
    string[2]='\0';
    return string;
}
char* Spn_Encryption(char* clear_text)//对两个字符进行操作
{ int* binary;
  int i;
  binary=Character_to_Binary(clear_text);//将两个字符转换为16位二进制整数数组
  for(i=0;i<(Nr-1);i++)
  {
      Xor(binary,i);
      Sbox(binary);
      Pbox(binary);
  }
  Xor(binary,Nr-1);
  Sbox(binary);
  Xor(binary,Nr);
  char* string;
  string=(char*)malloc(2*sizeof(char));
  string=Binary_to_Character(binary);
  return string;
}
void sub_reserve(int* g)
{
  if(Array_Equal_Judge(g,s0)==1)
    {int_strncpy(g,s14,4);return;}
  if(Array_Equal_Judge(g,s1)==1)
    {int_strncpy(g,s3,4);return;}
  if(Array_Equal_Judge(g,s2)==1)
    {int_strncpy(g,s4,4);return;}
  if(Array_Equal_Judge(g,s3)==1)
    {int_strncpy(g,s8,4);return;}
  if(Array_Equal_Judge(g,s4)==1)
    {int_strncpy(g,s1,4);return;}
  if(Array_Equal_Judge(g,s5)==1)
    {int_strncpy(g,s12,4);return;}
  if(Array_Equal_Judge(g,s6)==1)
    {int_strncpy(g,s10,4);return;}
  if(Array_Equal_Judge(g,s7)==1)
    {int_strncpy(g,s15,4);return;}
  if(Array_Equal_Judge(g,s8)==1)
    {int_strncpy(g,s7,4);return;}
  if(Array_Equal_Judge(g,s9)==1)
    {int_strncpy(g,s13,4);return;}
  if(Array_Equal_Judge(g,s10)==1)
    {int_strncpy(g,s9,4);return;}
  if(Array_Equal_Judge(g,s11)==1)
    {int_strncpy(g,s6,4);return;}
  if(Array_Equal_Judge(g,s12)==1)
    {int_strncpy(g,s11,4);return;}
  if(Array_Equal_Judge(g,s13)==1)
    {int_strncpy(g,s2,4);return;}
  if(Array_Equal_Judge(g,s14)==1)
    {int_strncpy(g,s0,4);return;}
  if(Array_Equal_Judge(g,s15)==1)
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

char* Spn_Decryption(char* cipher_text)//对两个字符进行操作
{ int* binary;
  int i;
  binary=Character_to_Binary(cipher_text);//将两个字符转换为16位二进制整数数组
  /*  if(Array_Equal_Judge(binary,s9))
        int_strncpy(binary,s0,4);
    if(Array_Equal_Judge(binary,s8))
        int_strncpy(binary,s1,4);
    if(Array_Equal_Judge(binary+8,s9))
        int_strncpy(binary+8,s0,4);
    if(Array_Equal_Judge(binary+8,s8))
        int_strncpy(binary+8,s1,4);*/

  Xor(binary,Nr);
  Sbox_Reserve(binary);
  Xor(binary,Nr-1);
  for(i=Nr-2;i>=0;i--)
  {
      Pbox_Reserve(binary);
      Sbox_Reserve(binary);
      Xor(binary,i);
  }
  char* string;
  string=(char*)malloc(2*sizeof(char));
  string=Binary_to_Character(binary);
  return string;
}

int main(){   
    char input[100];//输入的需要加密的明文
    char output[100];//输出的加密完成的密文
    char opt[100];//优化后的输出防止输出控制字符
    char decrypt[100];//对密文解密输出的明文
    char clear_text[3];//每次进行加密的2个字符明文
    char cipher_text[3];//每次进行加密后的2个字符密文
    int key[33]={0,0,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,1,1};//初始密钥数组，32位二进制
    int len;//输入的明文长度（字符数)
    int group_len;//分组后的组数（每组两个字符）
    int i;//分组加密计数
    printf("请输入您要加密的明文：");
    scanf("%s",input);
//spn加密 每次加密明文为16位二进制，对应的为4位十六进制，转换为字符即为2个字符，每次加密取2个字符进行加密
    len=strlen(input);
    if(len%2!=0)
    { char padding[2]="0";//字符数为奇数时末尾补0
      strcat(input,padding);
      len++;
    }
    group_len=len/2;
    Roundkey_Generation(key);
    for(i=0;i<group_len;i++)
    {
        strncpy(clear_text,input+2*i,2);
        clear_text[2]='\0';
        strncpy(cipher_text,Spn_Encryption(clear_text),2);
        cipher_text[2]='\0';
        strncpy(output+2*i,cipher_text,2);
        //output[2*(i+1)]='\0';
    }
    printf("%s\n",output);

    for(i=0;i<group_len;i++)
    {
        strncpy(clear_text,output+2*i,2);
        clear_text[2]='\0';
        strncpy(cipher_text,Spn_Decryption(clear_text),2);
        cipher_text[2]='\0';
        strncpy(decrypt+2*i,cipher_text,2);
        decrypt[2*(i+1)]='\0';
    }
    printf("%s",decrypt);

    return 0;
}
