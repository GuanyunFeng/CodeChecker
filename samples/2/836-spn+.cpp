//
//  main.c
//  SPNlevelup
//
//  Created by 林斌 on 2018/8/7.
//  Copyright © 2018年 林斌. All rights reserved.
//

#include "math.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include "time.h"

#define T 80000

//SPN加强版包括以下增强部分
//1 分组长度增加，由于之前SPN网络是建立在int型数字数组上实现的，所以此时读取的大文件也由int型数字组成
//2 密钥长度增加，密钥编排方案改变（使用cbc）
//3 s盒长度增加，s盒使用了AES的S盒（输入8位，输出8位）
//由于此时文件数量大幅度增加，若此时还是按原来使用数组方法存入数据有可能导致栈溢出（系统给出的栈空间只有2M），采取另外方案
//从文件中读取一组数据并将其进行加密，然后将该组数据写入文件中
//
//

static int s[256]={
    99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,
    202,130,201,125,250,89,71,240,173,212,162,175,156,164,114,192,
    183,253,147,38,54,63,247,204,52,165,229,241,113,216,49,21,
    4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,
    9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,
    83,209,0,237,32,252,177,91,106,203,190,57,74,76,88,207,
    208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168,
    81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,
    205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115,
    96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219,
    224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,
    231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8,
    186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138,
    112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158,
    225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,
    140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22};	//s盒
 
static int Rcon[10][32]={
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int s_1[256]={
    82,9,106,213,48,54,165,56,191,64,163,158,129,243,215,251,
    124,227,57,130,155,47,255,135,52,142,67,68,196,222,233,203,
    84,123,148,50,166,194,35,61,238,76,149,11,66,250,195,78,
    8,46,161,102,40,217,36,178,118,91,162,73,109,139,209,37,
    114,248,246,100,134,104,152,22,212,164,92,204,93,101,182,146,
    108,112,72,80,253,237,185,218,94,21,70,87,167,141,157,132,
    144,216,171,0,140,188,211,10,247,228,88,5,184,179,69,6,
    208,44,30,143,202,63,15,2,193,175,189,3,1,19,138,107,
    58,145,17,65,79,103,220,234,151,242,207,206,240,180,230,115,
    150,172,116,34,231,173,53,133,226,249,55,232,28,117,223,110,
    71,241,26,113,29,41,197,137,111,183,98,14,170,24,190,27,
    252,86,62,75,198,210,121,32,154,219,192,254,120,205,90,244,
    31,221,168,51,136,7,199,49,177,18,16,89,39,128,236,95,
    96,81,127,169,25,181,74,13,45,229,122,159,147,201,156,239,
    160,24,59,77,174,42,245,176,200,235,187,60,131,83,153,97,
    23,43,4,126,186,119,214,38,225,105,20,99,85,33,12,125};
 


int p[128]={
    41,35,62,97,108,86,46,82,16,73,113,59,105,107,51,38,
    91,60,7,12,25,94,13,28,55,71,18,77,72,67,11,31,
    3,90,125,56,37,93,84,75,124,22,117,69,19,10,50,32,
    26,80,110,64,120,54,118,30,92,45,79,20,114,68,102,48,
    33,34,17,29,74,2,57,44,126,85,106,76,83,63,103,14,
    111,87,127,15,81,112,119,104,116,89,53,42,101,43,1,95,
    123,5,21,24,100,78,49,70,4,115,47,122,36,8,121,88,
    40,96,61,23,39,66,65,27,6,98,52,9,99,109,58,0};
int p_1[128]={
    127,94,69,32,104,97,120,18,109,123,45,30,19,22,79,83,
    8,66,26,44,59,98,41,115,99,20,48,119,23,67,55,31,
    47,64,65,1,108,36,15,116,112,0,91,93,71,57,6,106,
    63,102,46,14,122,90,53,24,35,70,126,11,17,114,2,77,
    51,118,117,29,61,43,103,25,28,9,68,39,75,27,101,58,
    49,84,7,76,38,73,5,81,111,89,33,16,56,37,21,95,
    113,3,121,124,100,92,62,78,87,12,74,13,4,125,50,80,
    85,10,60,105,88,42,54,86,52,110,107,96,40,34,72,82};

int key_lun[10][128];

void hextoDecmical(char hexs[512],int *decs){
    
    int temp[2];
    
    for (int i=0; i<512; i+=2) {
        if (hexs[i]>'9'&&hexs[i]<='f') {
            temp[0]=hexs[i]-'a'+1;
        }
        else temp[0]=hexs[i]-'0';
        
            if (hexs[i+1]>'9'&&hexs[i+1]<='f') {
                temp[1]=hexs[i+1]-'a'+10;
            }
            else temp[1]=hexs[i+1]-'0';
        decs[i/2]=temp[0]*16+temp[1];
    }
}

int BinarytoDecmical(int b[],int len){    //将len位2进制转化为十进制
    int i=0,s=0;
    while(i<len)
    {
        s=s*2+b[i];
        i++;
    }
    return s;
    
}
void DecmicaltoBinary(int dec,int *bit){   //将一个十进制转化为4位2进制
    int len=0;
    int i,t[4];
    while(dec!=0)
    {
        t[len]=dec%2;
        len++;
        dec=dec/2;
    }
    if (len==4) {
        for(i=0;i<4;i++)
            bit[i]=t[3-i];
    }
    else if(len<4){
        
        for (i=len; i<4; i++)
            t[i]=0;
        
        for(i=0;i<4;i++)
            bit[i]=t[3-i];
        
    }
    else printf("转化的十进制超过16\n");
}

void DecmicaltoBinary_8bits(int dec,int *bit){   //将一个十进制转化为8位2进制
    int len=0;
    int i,t[8];
    while(dec!=0)
    {
        t[len]=dec%2;
        len++;
        dec=dec/2;
    }
    if (len==8) {
        for(i=0;i<4;i++)
            bit[i]=t[7-i];
    }
    else if(len<8){
        
        for (i=len; i<8; i++)
            t[i]=0;
        
        for(i=0;i<8;i++)
            bit[i]=t[7-i];
        
    }
    else printf("转化的十进制超过16\n");
}

void Substitution(int u[]){      //置换16位输入数组u
    int i;
    int temp[16];
    for (i=0; i<16;i++) {
        temp[i]=BinarytoDecmical(u+8*i, 8);
    }
    for (i=0; i<16; i++) {
        temp[i]=s[(temp[i])];
    }
    for (i=0; i<16; i++) {
        DecmicaltoBinary_8bits(temp[i], u+8*i);
    }
}
void Substitution_1(int u[]){      //置换16位输入数组u

    int i;
    int temp[16];
    
    for (i=0; i<16;i++) {
        temp[i]=BinarytoDecmical(u+8*i, 8);
    }
    for (i=0; i<16; i++) {
        temp[i]=s_1[(temp[i])];
    }
    for (i=0; i<16; i++) {
        DecmicaltoBinary_8bits(temp[i], u+8*i);
    }

}

void Permutation(int *v){  //输入为128位2进制数组
    int temp[128];
    for (int i=0; i<128; i++) {
        temp[(p[i])]=v[i];
    }
    for (int i=0; i<128; i++) {
        v[i]=temp[i];
    }
}
void Permutation_1(int *v){  //输入为16位2进制数组
    
    int temp[128];
    for (int i=0; i<128; i++) {
        temp[(p_1[i])]=v[i];
    }
    for (int i=0; i<128; i++) {
        v[i]=temp[i];
    }
    
}

void Key_change(int r,int w[32])//密钥轮加
{
    int i,m,n,j = 0;
    int w0[8];
    for(i=0;i<8;i++){
        w0[i]=w[i];
    }
    for(i=0;i<24;i++)
        w[i]=w[i+8];
    for(i=0;i<8;i++)
        w[24+i]=w0[i];//密钥字循环
    
    for(i=0;i<4;i++){
        m=128*w[8*i]+64*w[8*i+1]+32*w[8*i+2]+16*w[8*i+3]+8*w[8*i+4]+4*w[8*i+5]+2*w[8*i+6]+w[8*i+7];
        n=s[m];
        for(j=7;j>0;j--){
            w[8*i+j]=n%2;
            n=n/2;
        }
    }//字节代换
    
    for(i=0;i<32;i++)
        w[i]=w[i]^Rcon[r][j];//轮常量异或
}

void Key_exten(int k[128],int key[][128])//密钥扩展
{
    int i,j;
    int w0[32];
    int w[44][32];
    
    for(i=0;i<4;i++){
        for(j=0;j<32;j++){
            w[i][j]=k[32*i+j];
        }
    }
    for(i=4;i<44;i++){
        if(i%4!=0){
            for(j=0;j<32;j++) w[i][j]=w[i-4][j]^w[i-1][j];
        }
        else {
            for(j=0;j<32;j++) w0[j]=w[i-1][j];
            Key_change(i/4-1,w0);
            for(j=0;j<32;j++)
                w[i][j]=w[i-4][j]^w0[j];
            
        }
    }
    for (int i=0; i<44; i++) {
        for (int j=0; j<32; j++) {
            key[i/4][(i%4)*32+j]=w[i][j];
        }
    }
}


void SPNnetwork(int *x,int *y,int l,int key[128]){    //输入为2进制的16位明文数组
    
    int u[128];
    int keyexten[11][128];
    
    Key_exten(key,  keyexten);
    
    for (int i=0; i<128; i++) {
        u[i] = x[i];
    }
   
    for (int i=0; i<l; i++) {
        //此处为白化操作
        for (int j=0; j<128; j++)
            u[j] = u[j]^keyexten[i][j];
        //此处为代换操作
        Substitution(u);
        //此处为置换操作,若此时为第l-1轮则不进行置换操作
        if(i<l-1)
            Permutation(u);
    }
    //最后一轮输出密文
    for (int i=0; i<128; i++) {
        y[i] = u[i]^keyexten[10][i];
    }
}

void SPNnetwork_1(int *x,int *y,int l,int key[128],FILE *fp){
    int u[128];
    
    for (int i=0; i<128; i++) {
        u[i] = y[i]^key[i];
    }
    //
    for (int i=0; i<128; i++) {
        fprintf(fp,"%d",u[i]);
    }
    fprintf(fp,"\n");
    //
    //此处i表示不同的轮数
    for (int i=0; i<l; i++) {
        //密钥重排
        //Key(key,s_box);
        //此处为白化操作
        if(i>0)
            Permutation_1(u);
        
        //
        for (int z=0; z<128; z++) {
            fprintf(fp,"%d",u[z]);
        }
        fprintf(fp,"\n");
        //
        //
        Substitution_1(u);
        //
        for (int z=0; z<128; z++) {
            fprintf(fp,"%d",u[z]);
        }
        fprintf(fp,"\n");
        //
        //
        for (int j=0; j<128; j++)
            u[j] = u[j]^key[j];
        //
        for (int z=0; z<128; z++) {
            fprintf(fp,"%d",u[z]);
        }
        fprintf(fp,"\n");
        //
    }
    //最后一轮输出密文
    for (int i=0; i<128; i++) {
        x[i] = u[i];
    }
    for (int z=0; z<128; z++) {
        fprintf(fp,"%d",x[z]);
    }
    fprintf(fp,"\n");

}


void InttoChar(int plaintext[128],unsigned char writein[16]){
    int i,j;
    for ( i=0; i<16; i++) {
        for ( j=8*i; j<8*(i+1); j++) {
            writein[i]<<=1;
            writein[i]=writein[i]|plaintext[j];
        }
    }
}
//
void ChartoInt(unsigned char writein[16],int plaintext[128] ){
    int i,j;
    int temp[128];
    for ( i=0; i<16; i++) {
        for ( j=8*i; j<8*(i+1); j++) {
            temp[j]=(int)writein[i]&0x01;
            writein[i]>>=1;
            
        }
    }
    for ( i=0; i<16; i++) {
        for ( j=8*i; j<8*(i+1); j++) {
            plaintext[j]=temp[8*(i+1)-j-1+8*i];
        }
    }
}

void writeintoFile(int plaintext[][128],FILE *fp,int len){
    
    if (fp==NULL) {
        printf("cannot open the file !");
    }
    for (int i=0; i<len; i++) {
        for (int j=0; j<128; j++) {
            fprintf(fp,"%d",plaintext[i][j]);
        }
        fprintf(fp, "\n");
    }
    
}

void writeintoFile_randtest(int plaintext[128],FILE *fp){
    
    if (fp==NULL) {
        printf("cannot open the file !");
    }
    unsigned char write_in[16];
    InttoChar(plaintext, write_in);
    for (int i=0; i<16; i++) {
        fprintf(fp, "%c",write_in[i]);
    }
}

void readfromFile(int plaintext[],FILE *fp){
    unsigned char c[16]={0};
    int d;
    if (fp==NULL) {
        printf("cannot open the file !");
    }
    for (int j=0; j<16; j++) {
        fscanf(fp, "%c",&c[j]);
        
    }
    ChartoInt(c, plaintext);
}

int main() {
    FILE *fp_x;
    FILE *fp_y;
    FILE *fp_x_randtest;
    FILE *fp_y_randtest;
    
    fp_x = fopen("/Users/bin/Desktop/SPNnetwork/生成文本/生成文本/x.txt","r");
    fp_y = fopen("/Users/bin/Desktop/SPNnetwork/SPNlevelup/SPNlevelup/y.txt","w");
    fp_x_randtest = fopen("/Users/bin/Desktop/SPNnetwork/SPNlevelup/SPNlevelup/x_randtest.txt","w+");
    fp_y_randtest = fopen("/Users/bin/Desktop/SPNnetwork/SPNlevelup/SPNlevelup/y_randtest.txt","w+");
    
    int x[128]={0};
    int y[128]={0};
    int key[128]=
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    int i,j=0;
    
    //初始化s盒
    //hextoDecmical(s,s_box);
    //初始化P盒
    //
    //SPNnetwork(x, y, 10, key,fp_y);
    //SPNnetwork_1(x, y, 10, key,fp_y);
    //将明文加密,循环80000次
    for (i=0; i<T; i++) {
        readfromFile(x, fp_x);
    
        SPNnetwork(x, y, 10, key);
        //1轮 0000100111011001000101000001000……
        //写入文件
        writeintoFile_randtest(y, fp_y_randtest);
        //将密文变为下一轮密钥
        for (int k=0; k<128; k++) {
            key[k]=y[k];
        }
        
    }
    
    /*fseek(fp_y_randtest, -256, SEEK_END);
    readfromFile(key, fp_y_randtest);
    fseek(fp_y_randtest, 0, SEEK_SET);
    
    //将密文解密，循环80000次
    for (i=0; i<T; i++) {
        readfromFile(y, fp_y_randtest);
        SPNnetwork_1(x, y, 10, key );
        writeintoFile_randtest(x, fp_x_randtest);
        fseek(fp_y_randtest, -((i+3)*128), SEEK_END);
        readfromFile(key, fp_y_randtest);
        fseek(fp_y_randtest, (i+1)*128, SEEK_SET);
    }*/
    //写入文件
    
    fclose(fp_x);
    fclose(fp_y);
    fclose(fp_y_randtest);
    fclose(fp_x_randtest);
    return 0;
}


