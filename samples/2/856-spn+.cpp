
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define true 1
#define false 0
#define nr 10

//分组长度128 轮秘钥长度128 轮数10 
unsigned short AES_S[16][16]={
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
        };//S盒子替代
unsigned short AES_S1[16][16]={
			{0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
    		{0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
    		{0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
			{0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
			
			{0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
			{0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84}, 
			{0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
			{0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
			
			{0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
			{0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
   			{0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
    		{0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
    		
			{0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
    		{0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
    		{0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
    		{0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}};

     
        
unsigned int S[16]={
		0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 
		0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
unsigned int S1[16]={
	 	0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 
		0x7,0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5};
//unsigned short p[128]={
//		1,5,9,13,17,21,25,29,33,37,41,45,49,53,57,61,65,69,73,77,81,85,89,93,97,101,105,109,113,117,121,125,
//		2,6,10,14,18,22,26,30,34,38,42,46,50,54,58,62,66,70,74,78,82,86,90,94,98,102,106,110,114,118,122,126,
//		3,7,11,15,19,23,27,31,35,39,43,47,51,55,59,63,67,71,75,79,83,87,91,95,99,103,107,111,115,119,123,127,
//		4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128
//};
unsigned short p[128]={
	 1,9,17,25,33,41,49,57,65,73,81,89,97,105,113,121,
	2,10,18,26,34,42,50,58,66,74,82,90,98,106,114,122,
	3,11,19,27,35,43,51,59,67,75,83,91,99,107,115,123,
	4,12,20,28,36,44,52,60,68,76,84,92,100,108,116,124,
	5,13,21,29,37,45,53,61,69,77,85,93,101,109,117,125,
	6,14,22,30,38,46,54,62,70,78,86,94,102,110,118,126,
	7,15,23,31,39,47,55,63,71,79,87,95,103,111,119,127,
	8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
};
//unsigned short p1[128]={
//		1,33,65,97,2,34,66,98,3,35,67,99,4,36,68,100,5,37,69,101,6,38,70,102,7,39,71,103,8,40,72,104,
//		9,41,73,105,10,42,74,106,11,43,75,107,12,44,76,108,13,45,77,109,14,46,78,110,15,47,79,111,16,48,80,112,
//		17,49,81,113,18,50,82,114,19,51,83,115,20,52,84,116,21,53,85,117,22,54,86,118,23,55,87,119,24,56,88,120,
//		25,57,89,121,26,58,90,122,27,59,91,123,28,60,92,124,29,61,93,125,30,62,94,126,31,63,95,127,32,64,96,128
//};
unsigned short p1[128]={
	1, 17, 33, 49, 65, 81, 97, 113,
	2, 18, 34, 50, 66, 82, 98, 114,
	3, 19, 35, 51, 67, 83, 99, 115,
	4, 20, 36, 52, 68, 84, 100, 116,
	5, 21, 37, 53, 69, 85, 101, 117,
	6, 22, 38, 54, 70, 86, 102, 118,
	7, 23, 39, 55, 71, 87, 103, 119,
	8, 24, 40, 56, 72, 88, 104, 120,
	9, 25, 41, 57, 73, 89, 105, 121,
	10, 26, 42, 58, 74, 90, 106, 122,
	11, 27, 43, 59, 75, 91, 107, 123,
	12, 28, 44, 60, 76, 92, 108, 124,
	13, 29, 45, 61, 77, 93, 109, 125,
	14, 30, 46, 62, 78, 94, 110, 126,
	15, 31, 47, 63, 79, 95, 111, 127,
	16, 32, 48, 64, 80, 96, 112, 128
	 
};
unsigned short K[15]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00};		//密钥
//unsigned short K[11]={0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,0x0000, 0x0000};		//密钥
unsigned int w[44]={0};
unsigned int Key[nr+1][4]={0};  //轮秘钥
unsigned short K1[44]={0};
unsigned short  Plain[32]={0};
unsigned int RCon[10]={0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1B000000, 0x36000000};
//unsigned int Input[nr+1][4];
void SPN_Cry(unsigned int In[],unsigned int Out[]);
void SPN_DE(unsigned int In[],unsigned int Out[]);
unsigned int S_Box(unsigned int In);
void P_Box(unsigned int In[10][4], unsigned int Out[10][4], unsigned int num);
void P1_Box(unsigned int In[10][4], unsigned int Out[10][4], unsigned int num);
void Key_Bit(unsigned short In[]);			//生成轮秘钥 
void Plain_Bit(unsigned short In[], unsigned int Out[]);
unsigned int S1_Box(unsigned int In);
unsigned int AES_S_Box(unsigned int In);
unsigned int AES_S1_Box(unsigned int In);

int main(){

	FILE *fp1, *fp2;
	fp1 = fopen("cipher.txt","w");
	fp2 = fopen("RandTest1.txt","r"); 
    unsigned int MesIn[4],MesOut[4],temp[4]={0};
    unsigned int OK;
    printf("SPN_PLUS\n");
    int op,i;
    op=1;
    while(op)
    {
//        system("cls");
        printf("\n  1.Encryption");
        printf("\n  2.Decryption");
        printf("\n  0.Quit\n");
        printf("  please choose:\n");
        scanf("%d",&op);
        getchar();
        switch(op)
        {
        case 1:
            printf("\n 加密:\n");                               
            printf("\n 请按16进制以八位为间隔输入128位明文\n");
//			while(!feof(fp2)){
//				for(i=0;i<32;i++){
//					Plain[i]=fgetc(fp2);	
//				}
//				Plain_Bit(Plain, MesIn);
//				for(i=0;i<4;i++){
//					MesIn[i]=temp[i]^MesIn[i];
//				}
//				SPN_Cry(MesIn,MesOut);
//				for(i=0;i<4;i++){
//	            	fprintf(fp1, "%x", MesOut[i]);
//	            	temp[i]=MesOut[i];
//        	}  	
//		}
		    for(i=0;i<4;i++){
            	scanf("%x",MesIn+i);
        	}
//            printf("请按16进制输入144位秘钥\n");
//            scanf("%I64x",&K);
            SPN_Cry(MesIn,MesOut);
            printf("密文为(16进制表示)：\n");
            for(i=0;i<4;i++){
	            printf("%x ",MesOut[i]);
        	}
//        	
		    getchar();getchar();
            break;
        case 2:
            printf("\n 解密:\n");
            printf("\n 请按16进制输入128位密文\n");
            for(i=0;i<4;i++){
            	scanf("%x",MesIn+i);
        	}
//            printf("请按16进制输入64位秘钥\n");
//            scanf("%I64x",&K);
            SPN_DE(MesIn,MesOut);
            printf("\n 明文为(16进制表示)：\n"); 
            for(i=0;i<4;i++){
	            printf("%x ",MesOut[i]);
        	}
            getchar();getchar();
            break;
        case 0:
            exit;
        }
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}
void Plain_Bit(unsigned short In[], unsigned int Out[]){
//	unsigned int P1,P2,P3,P4,P5,P6,P7,P8;  
	int i;
	for(i=0;i<4;i++){
		Out[i]=In[i*8+0]|In[i*8+1]|In[i*8+2]|In[i*8+3]|In[i*8+4]|In[i*8+5]|In[i*8+6]|In[i*8+7];
	} 
}
void Key_Bit(unsigned short In[])
{
//	int i,j;
//	unsigned short L1;
//	unsigned int temp;
//	for(i=0;i<4;i++){
//		w[i]=K[4*i]<<24|K[4*i+1]<<16|K[4*i+2]<<8|K[4*i+3];
//	}
//	for(i=4;i<44;i++){
//		temp = w[i-1];
//		if(i % 4==0){
//			L1= (temp&0xff000000)>>24;
//			temp=(temp<<8)|L1;
//			temp = AES_S_Box(temp);
//			temp = temp ^ RCon[i/4];
//		}
//		w[i]=w[i-4]^temp;
//	}
//	for(i=0;i<nr;i++){
//		for(j=0;j<4;j++){
//			Key[i][j]=w[i*4+j+4];
//		}
//	}
 	unsigned int A1,A2,A3,A4,A5,A6,A7,A8;
	int i,j;
	for(i=0;i<11;i++){
		for(j=0;j<4;j++){
			K1[i*4+j]=(In[i]&0xf000)>>12;
			In[i]=In[i]<<4;
		}
	}
	for(i=0;i<=nr;i++){
		for(j=0;j<4;j++){
			Key[i][j]=(K1[i+j*8+0]<<28)|(K1[i+j*8+1]<<24)|(K1[i+j*8+2]<<20)|(K1[i+j*8+3]<<16)|(K1[i+j*8+4]<<12)|(K1[i+j*8+5]<<8)|(K1[i+j*8+6]<<4)|(K1[i+j*8+7]);
//			A1 = K1[i+j*8+0];
//			A2 = K1[i+j*8+1];
//			A3 = K1[i+j*8+2];
//			A4 = K1[i+j*8+3];
//			A5 = K1[i+j*8+4];
//			A6 = K1[i+j*8+5];
//			A7 = K1[i+j*8+6];
//			A8 = K1[i+j*8+7];
		}
	}	
}
unsigned int AES_S_Box(unsigned int In)
{
    unsigned int Out;
    unsigned int M1,M2,M3,M4,M5,M6,M7,M8;
    M1=(In&0xF0000000)>>28;
    M2=(In&0x0F000000)>>24;
    M3=(In&0x00F00000)>>20;
    M4=(In&0x000F0000)>>16;
    M5=(In&0x0000F000)>>12;
    M6=(In&0x00000F00)>>8;
    M7=(In&0x000000F0)>>4;
    M8=In&0x0000000F;
    M1=AES_S[M1][M2];
    M2=AES_S[M3][M4];
    M3=AES_S[M5][M6];
    M4=AES_S[M7][M8];
    M1=M1<<24;
    M2=M2<<16;
    M3=M3<<8;
    Out=M1|M2|M3|M4;
    return Out;
}
unsigned int AES_S1_Box(unsigned int In)
{
    unsigned int Out;
    unsigned int M1,M2,M3,M4,M5,M6,M7,M8;
    M1=(In&0xF0000000)>>28;
    M2=(In&0x0F000000)>>24;
    M3=(In&0x00F00000)>>20;
    M4=(In&0x000F0000)>>16;
    M5=(In&0x0000F000)>>12;
    M6=(In&0x00000F00)>>8;
    M7=(In&0x000000F0)>>4;
    M8=In&0x0000000F;
    M1=AES_S1[M1][M2];
    M2=AES_S1[M3][M4];
    M3=AES_S1[M5][M6];
    M4=AES_S1[M7][M8];
    M1=M1<<24;
    M2=M2<<16;
    M3=M3<<8;
    Out=M1|M2|M3|M4;
    return Out;
}

void SPN_Cry(unsigned int In[], unsigned int Out[])
{
	
	int i,j;
	unsigned int w[nr][4]={0};
	unsigned int u[nr][4]={0};
	unsigned int v[nr][4]={0};
	
	Key_Bit(K);
	for(i=0;i<4;i++){
		w[0][i]=In[i];
	}
	for(i=0;i<nr;i++){
		for(j=0;j<4;j++){
			u[i][j]=w[i][j]^Key[i][j];
//			v[i][j]=S_Box(u[i][j]);
			v[i][j]=AES_S_Box(u[i][j]);
		}
		if(i<(nr-1)){
				P_Box(v,w,i);
			}
		else
			continue;
	} 
	for(i=0;i<4;i++){
		Out[i]=v[nr-1][i]^Key[nr][i];
	}
} 
void SPN_DE(unsigned int In[], unsigned int Out[]){
	int i,j;
	unsigned int w[nr][4]={0};
	unsigned int u[nr][4]={0};
	unsigned int v[nr][4]={0};
	
	Key_Bit(K);
	for(i=0;i<4;i++){
		w[nr-1][i]=In[i];
		v[nr-1][i]=w[nr-1][i]^Key[nr][i];
		u[nr-1][i] = AES_S1_Box(v[nr-1][i]);
	}
	for(i=nr-2;i>=0;i--){
		for(j=0;j<4;j++){
			w[i][j]=Key[i+1][j]^u[i+1][j];
		}
		P1_Box(w,v,i);
		for(j=0;j<4;j++){
			u[i][j]= AES_S1_Box(v[i][j]);
		}
	}
	for(i=0;i<4;i++){
		Out[i]=u[0][i]^Key[0][i];
	}
	
}
unsigned int S_Box(unsigned int In){
	 unsigned int Out;
	 unsigned int M1,M2,M3,M4,M5,M6,M7,M8;
    M1=(In&0xF0000000)>>28;
    M2=(In&0x0F000000)>>24;
    M3=(In&0x00F00000)>>20;
    M4=(In&0x000F0000)>>16;
    M5=(In&0x0000F000)>>12;
    M6=(In&0x00000F00)>>8;
    M7=(In&0x000000F0)>>4;
    M8=In&0x0000000F;
	M1 = S[M1];
	M2 = S[M2];
	M3 = S[M3];
	M4 = S[M4];
	M5 = S[M5];
	M6 = S[M6];
	M7 = S[M7];
	M8 = S[M8];
	M1 = M1<<28;
	M2 = M2<<24;
	M3 = M3<<20;
	M4 = M4<<16;
	M5 = M5<<12;
	M6 = M6<<8;
	M7 = M7<<4;
	Out = M1|M2|M3|M4|M5|M6|M7|M8; 
	return Out;
}
void P_Box(unsigned int In[10][4], unsigned int Out[10][4], unsigned int num){
//	unsigned int J[10][4];
	unsigned int M1[128];
	unsigned int M2[128];
	int i,j;
	for(i=3;i>=0;i--){
		for(j=31;j>=0;j--){
			M1[i*32+j] = In[num][i]%2;
			In[num][i]/=2;
		}
	}
	for(i=0;i<128;i++){
		M2[i]=M1[p[i]-1];
	}
	for(i=0;i<4;i++){
		for(j=0;j<32;j++){
			Out[num+1][i]=Out[num+1][i]*2+M2[i*32+j];
//			w[num][i]=w[num][i]*2+M2[i*32+j];
		}
	}
}
unsigned int  S1_Box(unsigned int In){
	 unsigned int Out;
	 unsigned int M1,M2,M3,M4,M5,M6,M7,M8;
    M1=(In&0xF0000000)>>28;
    M2=(In&0x0F000000)>>24;
    M3=(In&0x00F00000)>>20;
    M4=(In&0x000F0000)>>16;
    M5=(In&0x0000F000)>>12;
    M6=(In&0x00000F00)>>8;
    M7=(In&0x000000F0)>>4;
    M8=In&0x0000000F;
	M1 = S1[M1];
	M2 = S1[M2];
	M3 = S1[M3];
	M4 = S1[M4];
	M5 = S1[M5];
	M6 = S1[M6];
	M7 = S1[M7];
	M8 = S1[M8];
	M1 = M1<<28;
	M2 = M2<<24;
	M3 = M3<<20;
	M4 = M4<<16;
	M5 = M5<<12;
	M6 = M6<<8;
	M7 = M7<<4;
	Out = M1|M2|M3|M4|M5|M6|M7|M8; 
	return Out;
}

void P1_Box(unsigned int In[10][4], unsigned int Out[10][4], unsigned int num)
{
	unsigned int B1[128]={1};
	unsigned int B2[128]={1};
	int i,j;
	for(i=3;i>=0;i--){
		for(j=31;j>=0;j--){
			B1[i*32+j] = In[num][i]%2;
			In[num][i]/=2;
		}
	}
	for(i=0;i<128;i++){
		B2[i]=B1[p1[i]-1];
	}
	for(i=0;i<4;i++){
		for(j=0;j<32;j++){
			Out[num][i]=Out[num][i]*2+B2[i*32+j];
		}
	}
}







