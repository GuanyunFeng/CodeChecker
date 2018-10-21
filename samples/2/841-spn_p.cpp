#include"mmxkeshe.h"

void FileDecryption(FILE *fp1,FILE *fp2)
{
    unsigned char c[16]={0};
    int i,j;
    int flag=0;
    int mw[128];
    int temp[128];
    int miw[128];
    int wi[16];
    if((fp1==NULL)||(fp2==NULL)){
        printf("解密或存储文件不存在\n");
        return ;
    }
    while(1){
    for(i=0;i<16;i++){
        if((fscanf(fp1,"%c",&c[i]))!=EOF);
        else{
            for(j=i;j<16;j++) c[j]=0;
            flag=1;}
    }
    if(flag==0){
        for(i=0;i<16;i++){
            for(j=8*i;j<8*i+8;j++){
                temp[j]=(int)c[i]&0x01;
                c[i]>>=1;
            }
        }
        for(i=0;i<16;i++)
            for (j=8*i;j<8*(i+1);j++)
                miw[j]=temp[8*(i+1)-j-1+8*i];
        Spn_Pjm(mw,miw,K_P);
        for(i=0;i<16;i++){
            for (j=8*i;j<8*(i+1);j++){
            wi[i]<<=1;
            wi[i]=wi[i]|mw[j];
            }
        }
        for(i=0;i<16;i++){
            fprintf(fp2,"%c",wi[i]);
        }
    }
    else{
        for(i=0;i<16;i++){
            for(j=8*i;j<8*i+8;j++){
                temp[j]=(int)c[i]&0x01;
                c[i]>>=1;
            }
        }
        for(i=0;i<16;i++)
            for (j=8*i;j<8*(i+1);j++)
                miw[j]=temp[8*(i+1)-j-1+8*i];
        Spn_Pjm(mw,miw,K_P);
        for(i=0;i<16;i++){
            for (j=8*i;j<8*(i+1);j++){
            wi[i]<<=1;
            wi[i]=wi[i]|mw[j];
            }
        }
        for(i=0;i<16;i++){
            fprintf(fp2,"%c",wi[i]);
        }
        fprintf(fp2,"%d",1);
        return ;
    }}
    return ;
}

void FileClock(FILE *fp1,FILE *fp2)
{
    unsigned char c[16]={0};
    int i,j;
    int flag=0;
    int mw[128];
    int temp[128];
    int miw[128];
    int wi[16];
    if((fp1==NULL)||(fp2==NULL)){
        printf("加密或存储文件不存在\n");
        return ;
    }
    while(1){
    for(i=0;i<16;i++){
        if((fscanf(fp1,"%c",&c[i]))!=EOF);
        else{
            for(j=i;j<16;j++) c[j]=0;
            flag=1;}
    }
    if(flag==0){
        for(i=0;i<16;i++){
            for(j=8*i;j<8*i+8;j++){
                temp[j]=(int)c[i]&0x01;
                c[i]>>=1;
            }
        }
        for(i=0;i<16;i++)
            for (j=8*i;j<8*(i+1);j++)
                mw[j]=temp[8*(i+1)-j-1+8*i];
        Spn_P(mw,miw,K_P);
        for(i=0;i<16;i++){
            for (j=8*i;j<8*(i+1);j++){
            wi[i]<<=1;
            wi[i]=wi[i]|miw[j];
            }
        }
        for(i=0;i<16;i++){
            fprintf(fp2,"%c",wi[i]);
        }
    }
    else{
        for(i=0;i<16;i++){
            for(j=8*i;j<8*i+8;j++){
                temp[j]=(int)c[i]&0x01;
                c[i]>>=1;
            }
        }
        for(i=0;i<16;i++)
            for (j=8*i;j<8*(i+1);j++)
                mw[j]=temp[8*(i+1)-j-1+8*i];
        Spn_P(mw,miw,K_P);
        for(i=0;i<16;i++){
            for (j=8*i;j<8*(i+1);j++){
            wi[i]<<=1;
            wi[i]=wi[i]|miw[j];
            }
        }
        for(i=0;i<16;i++){
            fprintf(fp2,"%c",wi[i]);
        }
        fprintf(fp2,"%d",1);
        return ;
    }}
    return ;


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

void Spn_Pjm(int mw[128],int miw[128],int K[128])
{
    printf("\n");
     printf("\n");
    int r,i,m,n,j;
    int k[128],u[128];
    int w[44][32];
    for(r=0;r<128;r++){
        mw[r]=miw[r];
    }

    Key_exten(K,w);

    for(i=0;i<4;i++){
        for(j=0;j<32;j++){
                k[32*i+j]=w[40+i][j];
        }
    }

    for(i=0;i<128;i++)
            mw[i]=mw[i]^k[i];


    for(r=9;r>=0;r--){
        for(i=0;i<128;i++) u[i]=mw[i];

        if(r!=9){
            for(i=0;i<128;i++) mw[P_Plusni[i]]=u[i];
        }



        for(i=0;i<16;i++){
            m=128*mw[8*i]+64*mw[8*i+1]+32*mw[8*i+2]+16*mw[8*i+3]+8*mw[8*i+4]+4*mw[8*i+5]+2*mw[8*i+6]+mw[8*i+7];
            n=S_Plus_ni[m];
            for(j=7;j>=0;j--){
                mw[8*i+j]=n%2;
                n=n/2;
            }
        }

        for(i=0;i<4;i++)
            for(j=0;j<32;j++)
                k[32*i+j]=w[4*r+i][j];
        for(i=0;i<128;i++)
            mw[i]=mw[i]^k[i];


    }


}


void Spn_P(int mw[128],int miw[128],int K[128])//加强版SPN
{
    int r,i,m,n,j;
    int k[128],u[128];
    int w[44][32];//11个128位密钥
    for(r=0;r<128;r++){
        miw[r]=mw[r];
    }
    Key_exten(K,w);

    for(r=0;r<10;r++){
        for(i=0;i<4;i++)
            for(j=0;j<32;j++)
                k[32*i+j]=w[4*r+i][j];

        for(i=0;i<128;i++)
            miw[i]=miw[i]^k[i];//异或



        for(i=0;i<16;i++){
            m=128*miw[8*i]+64*miw[8*i+1]+32*miw[8*i+2]+16*miw[8*i+3]+8*miw[8*i+4]+4*miw[8*i+5]+2*miw[8*i+6]+miw[8*i+7];
            n=S_Plus[m];
            for(j=7;j>=0;j--){
                u[8*i+j]=n%2;
                n=n/2;
            }
        }// S盒


       if(r!=9){
            for(i=0;i<128;i++)
                miw[P_Plus[i]]=u[i];
        }//P盒
        else {
            for(i=0;i<128;i++) miw[i]=u[i];
        }


    }

    for(i=0;i<4;i++)
            for(j=0;j<32;j++)
                k[32*i+j]=w[40+i][j];

    for(i=0;i<128;i++)
            miw[i]=miw[i]^k[i];
}

void Key_exten(int k[128],int w[44][32])//密钥扩展
{
    int i,j;
    int w0[32];

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
}

void Key_change(int r,int w[32])//密钥轮加
{
    int i,m,n,j;
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
        n=S_Plus[m];
        for(j=7;j>0;j--){
            w[8*i+j]=n%2;
            n=n/2;
        }
    }//字节代换

    for(i=0;i<32;i++)
        w[i]=w[i]^Rcon[r][j];//轮常量异或


}

int main()
{
    int mw[16];
    int miw[16];
    int KeyO[32];//攻击所得密钥
    int i=1,b,m,op,ip;
    int mw2[128];
    int miw2[128];
    int KP[128];
    FILE *fp;
    FILE *FP1;
    FILE *FP2;
    clock_t Start,End;
    char FileName[20];
    while(i==1){
        system("cls");
        printf("请选择要执行的操作：\n");
        printf("*************************************************\n");
        printf("      1、原始SPN算法\n");
        printf("      2、原始SPN算法线性分析和差分分析\n");
        printf("      3、增强SPN算法\n");
        printf("      4、随机性检测\n");
        printf("      0、退出\n");
        printf("*************************************************\n");
        scanf("%d",&op);
        switch(op){
        case 1:
            printf("请输入明文（二进制、16位）");
            for(int a=0;a<16;a++){
            scanf("%d",&mw[a]);
            }
            SPN(mw,miw,K);
            printf("加密后的密文为:\n");
            for(b=0;b<4;b++){
                printf("%d%d%d%d  ",miw[b*4],miw[b*4+1],miw[b*4+2],miw[b*4+3]);
            }
            printf("/n");
            SPN_jm(mw,miw,K);
            printf("解密后的明文为:\n");
            for(b=0;b<4;b++){
                printf("%d%d%d%d  ",mw[b*4],mw[b*4+1],mw[b*4+2],mw[b*4+3]);
            }

            getchar();
            getchar();
            break;
        case 2:
            printf("请选择分析方式：\n");
            printf("*************************\n");
            printf("    1、线性分析\n");
            printf("    2、差分分析\n");
            printf("*************************\n");
            scanf("%d",&ip);
            if(ip==1){
                for(b=0;b<32;b++) KeyO[b]=0;
                Start = clock();
                LineA(KeyO);
                End = clock();
                printf("线性分析所得密钥为：\n");
                printf("%d%d%d%d  ",KeyO[20],KeyO[21],KeyO[22],KeyO[23]);
                printf("%d%d%d%d  ",KeyO[28],KeyO[29],KeyO[30],KeyO[31]);
                printf("\n线性分析所用的时间：%g ms\n", double(End - Start));
                Start = clock();
                VioA(KeyO);
                End=clock();
                printf("加以暴力破解所得密钥为：\n");
                for(b=0;b<8;b++){
                    printf("%d%d%d%d  ",KeyO[b*4],KeyO[b*4+1],KeyO[b*4+2],KeyO[b*4+3]);
                }
                printf("\n暴力破解所用的时间：%g ms", double(End - Start));
                getchar();
                getchar();
            }
            else if(ip==2){
                for(b=0;b<32;b++) KeyO[b]=0;
                printf("差分分析所得密钥为：\n");
                Start = clock();
                DifA(KeyO);
                End = clock();
                printf("%d%d%d%d  ",KeyO[20],KeyO[21],KeyO[22],KeyO[23]);
                printf("%d%d%d%d  ",KeyO[28],KeyO[29],KeyO[30],KeyO[31]);
                printf("\n差分分析所用的时间：%g ms\n", double(End - Start));
                Start = clock();
                VioA(KeyO);
                End = clock();
                printf("加以暴力破解所得密钥为：\n");
                for(b=0;b<8;b++){
                    printf("%d%d%d%d  ",KeyO[b*4],KeyO[b*4+1],KeyO[b*4+2],KeyO[b*4+3]);
                }
                printf("\n暴力破解所用的时间：%g ms", double(End - Start));
                getchar();
                getchar();
            }
            else{
                printf("输入无效\n");
                getchar();
                getchar();
            }
            break;
        case 3:
            printf("请输入完成功能：\n");
            printf("*************************\n");
            printf("    1、加密文件\n");
            printf("    2、解密文件\n");
            printf("*************************\n");
            scanf("%d",&ip);
            if(ip==1){
                printf("请输入需要加密的文件名：\n");
                scanf("%s",FileName);
                FP1=fopen(FileName,"r");
                printf("请输入密文文件名：\n");
                scanf("%s",FileName);
                FP2=fopen(FileName,"w");
                FileClock(FP1,FP2);
                printf("加密完成\n");
                fclose(FP1);
                fclose(FP2);
            }
            else if(ip==2){
                printf("请输入需要解密的文件名：\n");
                scanf("%s",FileName);
                FP1=fopen(FileName,"r");
                printf("请输入明文文件名：\n");
                scanf("%s",FileName);
                FP2=fopen(FileName,"w");
                FileDecryption(FP1,FP2);
                printf("解密完成\n");
                fclose(FP1);
                fclose(FP2);
            }
            else  printf("无效输入\n");
            getchar();
            getchar();
            break;
        case 4:
            for(b=0;b<128;b++) KP[b]=0;//首轮密钥为0；
            fp=fopen("123.txt","w");
            for(m=0;m<700000;m++){
                for(b=0;b<128;b++) mw2[b]=1;//输入置为0
                Spn_P(mw2,miw2,KP);
                for(i=0;i<128;i++)
                    KP[i]=miw2[i];
                FileWrite(miw2,fp);
            }
            fclose(fp);
            printf("生成校验文件完成！");
            getchar();
            getchar();
            break;
        case 0:
            getchar();
            getchar();
            return 0;
        default:
            printf("无效输入\n");
            getchar();
            getchar();
            break;
        }
    }
    return 0;
}
