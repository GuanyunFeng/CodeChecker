#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
int S_Plus_ni[256]={
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

int S_Plus[256]={
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
140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22};

int P_Plus[128]={
41,35,62,97,108,86,46,82,16,73,113,59,105,107,51,38,
91,60,7,12,25,94,13,28,55,71,18,77,72,67,11,31,
3,90,125,56,37,93,84,75,124,22,117,69,19,10,50,32,
26,80,110,64,120,54,118,30,92,45,79,20,114,68,102,48,
33,34,17,29,74,2,57,44,126,85,106,76,83,63,103,14,
111,87,127,15,81,112,119,104,116,89,53,42,101,43,1,95,
123,5,21,24,100,78,49,70,4,115,47,122,36,8,121,88,
40,96,61,23,39,66,65,27,6,98,52,9,99,109,58,0
};

int P_Plusni[128]={
127,94,69,32,104,97,120,18,109,123,45,30,19,22,79,83,
8,66,26,44,59,98,41,115,99,20,48,119,23,67,55,31,
47,64,65,1,108,36,15,116,112,0,91,93,71,57,6,106,
63,102,46,14,122,90,53,24,35,70,126,11,17,114,2,77,
51,118,117,29,61,43,103,25,28,9,68,39,75,27,101,58,
49,84,7,76,38,73,5,81,111,89,33,16,56,37,21,95,
113,3,121,124,100,92,62,78,87,12,74,13,4,125,50,80,
85,10,60,105,88,42,54,86,52,110,107,96,40,34,72,82};


int SPNplus(int ClearText[],int CipherText[],int key[]);
void FileWrite(int w[128],FILE *fp);
void SPNplus_jie(int ClearText[],int CipherText[],int key[]);



int SPNplus(int ClearText[],int CipherText[],int key[])
{
   int temp1[128];//将置换后的还原成0,1储存 128位明文
   int round,t1,t2;//t1为代换序号 t2为代换后结果 共16轮与128位密钥的异或

   for(int i=0;i<128;i++)
   {
       ClearText[i]=ClearText[i]^CipherText[i];
   }

   for(round=0;round<15;round++)
   {
      for(int i=0;i<128;i++)
      {
          ClearText[i]=ClearText[i]^key[round*4+i];
      }
       /*for(int j=0;j<128;j++)
        {
           printf("%d  ",ClearText[j]);
        }
        printf("\n");
      for(int j=0;j<128;j++)
        {
           printf("%d  ",CipherText[j]);
        }
        printf("\n");
        for(int j=0;j<128;j++)
        {
           printf("%d  ",key[j]);
        }
        printf("\n");*/

       for(int j=0;j<128;j=j+8)
        {
            t1 = 128*ClearText[j]+64*ClearText[j+1]+32*ClearText[j+2]+16*ClearText[j+3]+8*ClearText[j+4]+4*ClearText[j+5]+2*ClearText[j+6]+ClearText[j+7];
            t2 = S_Plus[t1];
            for(int m=0;m<8;m++)
            {
                temp1[j-m+7]=t2%2;
                t2=t2/2;
            }
        }

       if(round<14)
       {
        for(int g=0;g<128;g++)
        {
           ClearText[g] = temp1[P_Plus[g]];
        }
       }
       else
       {
         for(int g=0;g<128;g++)
        {
           ClearText[g] = temp1[g];
        }
       }

   }
   for(int i=0;i<128;i++)
   {
       CipherText[i]=ClearText[i]^key[round*4+i];
    }

   return 0;
}

void SPNplus_jie(int ClearText[],int CipherText[],int k[])
{
    int b,t,m;
    int temp1[128];
    for(int j=0; j<128;j++){
        ClearText[j]=CipherText[j];
    }
    for(int q=128;q<128;q++){
        ClearText[q]=ClearText[q]^k[q+64];
    }

    for(int round=15;round>=0;round--)
        {
        for(int c=0;c<128;c++)
        {
           temp1[c]=ClearText[c];
        }
        if(round!=15){
            for(b=0;b<128;b++)
                ClearText[b]=temp1[P_Plusni[b]];
        }

        for(b=0;b<128;b=b+8)
            {
              t=128*ClearText[b]+64*ClearText[b+1]+32*ClearText[b+2]+16*ClearText[b+3]+8*ClearText[b+4]+4*ClearText[b+5]+2*ClearText[b+6]+ClearText[b+7];
              m=S_Plus_ni[t];
              for(int g=0;g<8;g++)
            {
                ClearText[b+7-g]=m%2;
                m=m/2;
            }
        }//逆S盒

        for(b=0;b<128;b++){
            ClearText[b]=ClearText[b]^k[b+round*4];
        }

    }

}


void FileWrite(int w[128],FILE *fp)
{

    if(fp==NULL){
        printf("文件不存在");
    return ;
    }
    int wi[16],i,j;
    for(i=0;i<16;i++){
        for (j=8*i;j<8*(i+1);j++){
            wi[i]<<=1;
            wi[i]=wi[i]|w[j];
        }
    }
    for(i=0;i<16;i++){
        fprintf(fp,"%c",wi[i]);
    }

}


int main()
{
    int k[32] = {0,0,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,1,1};
    int ClearText[16];
    int CipherText[16];
    int ClearText1[16];
    int CipherText1[16];
    int ClearText2[128];
    int CipherText2[128];
    int ktest[196];
    int Maxkey1,Maxkey2;
    int temp[2];
    int kd[32];
    int i=1;
    int kp[196];//
    int ctp[128];//明文为0
    int cip[128];//miwen
    int kp1[32];
    int ctp1[16];
    int cip1[16];




    while(i==1){
        int op,ip;
        system("cls");
        printf("请选择要执行的操作：\n");
        printf("*************************************************\n");
        printf("      1、原始SPN算法\n");
        printf("      2、原始SPN算法线性分析\n");
        printf("      3、原始SPN算法差分分析\n");
        printf("      4、暴力破解\n");
        printf("      5、 SPN解密\n");
        printf("      6、 SPN增强解密\n");
        printf("      7、 SPN增强随机性检测\n");
        printf("*************************************************\n");
        scanf("%d",&op);
        switch(op){
        case 1:
            system("cls");
            printf("请输入明文（二进制，16位）");
            for(int m=0;m<16;m++)
            {
            scanf("%d",&ClearText[m]);
            }

            SPN(ClearText,CipherText,k);

            printf("经过SPN加密前为:");
           for(int i=0;i<16;i++)
          {
             printf("%d ",ClearText[i]);
          }
           printf("\n");

            printf("经过SPN加密后为:");
           for(int i=0;i<16;i++)
          {
             printf("%d ",CipherText[i]);
          }
           printf("\n");
           getchar();
           getchar();
            break;
        case 2:
            double start, finish;
            system("cls");
            start=clock();
            XianXing(temp);
            finish=clock();
            printf("线性分析出候选子密钥为%d %d\n",temp[0],temp[1]);
            printf("线性分析用时：%f seconds\n\n", (finish - start) / CLOCKS_PER_SEC);
            getchar();
            getchar();
            break;
        case 3:
            double start1, finish1;
            system("cls");
            start1=clock();
            ChaFeng();
            printf("\n");
            finish1=clock();
            printf("差分分析用时：%f seconds\n", (finish1 - start1) / CLOCKS_PER_SEC);
            getchar();
            getchar();
            break;
        case 4:
            double start2, finish2;
            system("cls");
            start2=clock();
            VioDecrypt(kd);
            finish2=clock();
            printf("解密：");
            for(int i=0;i<32;i++)
            {
                printf("%d ",kd[i]);
            }
            printf("暴力破解用时：%f seconds\n", (finish2 - start2) / CLOCKS_PER_SEC);
            getchar();
            getchar();
            break;


        case 5:
            system("cls");
            printf("请输入明文（二进制，16位）");
            for(int m=0;m<16;m++)
            {
            scanf("%d",&ClearText1[m]);
            }
            SPN(ClearText1,CipherText1,k);

            printf("经过SPN加密前为:");
           for(int i=0;i<16;i++)
          {
             printf("%d ",ClearText1[i]);
          }
           printf("\n");

            printf("经过SPN加密后为:");
           for(int i=0;i<16;i++)
          {
             printf("%d ",CipherText1[i]);
          }
           printf("\n");

            SPNjie(ClearText1,CipherText1,k);

            printf("加密再解密得明文");
            for(int m=0;m<16;m++)
            {
             printf("%d ",ClearText1[m]);
            }
            printf("\n");

            getchar();
            getchar();
            break;

    /*case 6:
        system("cls");
            FILE *fp=fopen("test2.txt","w");
               for(int i=0;i<32;i++)
             {
                kp1[i]=0;
              }//首轮密钥为0；

                for(int i=0;i<16;i++)
             {
               cip1[i]=0;
             }
               for(int m=0;m<300000;m++)
            {
               for(int b=0;b<16;b++)
               {
                 ctp1[b] = 0;
                 //cip[b] = 0;
                }
                   //输入置为0
               SPN(ctp1,cip1,kp1);
               /*for(int j=0;j<128;j++)
               {
                   printf("%d",cip[j]);
               }
               printf("\n");
               FileWrite(cip1,fp);
            }
            fclose(fp);
            printf("加密成功");
            getchar();
            getchar();
            break;*/
        case 6:
            system("cls");
            for(int n=0;n<196;n++)
            {
                ktest[n]=0;
            }
            for(int m=0;m<128;m++)
            {
               ClearText2[m]=0;
            }
            for(int m=0;m<128;m++)
            {
               CipherText2[m]=0;
            }
            printf("经过SPN加密前为:");
            printf("\n");
           for(int i=0;i<128;i++)
           {
             printf("%d ",ClearText2[i]);
           }
            printf("\n");

            SPNplus(ClearText2,CipherText2,ktest);
            printf("经过SPN加密后为:");
           for(int i=0;i<128;i++)
          {
             printf("%d ",CipherText2[i]);
          }
           printf("\n");
           printf("加密再解密得明文");
            printf("\n");

            SPNplus_jie(ClearText2,CipherText2,ktest);
            for(int m=0;m<128;m++)
            {
                ClearText2[m]=0;
            }
            for(int i=0;i<128;i++)
           {
             printf("%d ",ClearText2[i]);
           }
            printf("\n");
            getchar();
            getchar();
            break;


        case 7:
            system("cls");
            FILE *fp=fopen("test.txt","w");
               for(int i=0;i<196;i++)
             {
                kp[i]=0;
              }//首轮密钥为0；

                for(int i=0;i<128;i++)
             {
               cip[i]=0;
             }
               for(int m=0;m<600000;m++)
            {
               for(int b=0;b<128;b++)
               {
                 ctp[b] = 0;
                }
               SPNplus(ctp,cip,kp);

               FileWrite(cip,fp);
            }
            fclose(fp);
            printf("SPN增强加密成功");
            getchar();
            getchar();
            break;

        }
    }
    return 0 ;
}
