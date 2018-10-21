#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define False 0
#define nr 4
#define N 8000
 
 
unsigned int K=0x3A94D63F;
unsigned int Key[nr+1] = {0};   //轮秘钥

unsigned int SPN_En(unsigned int Input);
unsigned int SPN_De(unsigned int Code);
unsigned int S_Box(unsigned int In);
unsigned int P_Box(unsigned int In);
unsigned int S1_Box(unsigned int In);
unsigned int P1_Box(unsigned int In);
unsigned int Linear_analysis();
void Bit(int i, unsigned int data, unsigned int b[]);
unsigned int Differential_analysis();


unsigned int S[16]={
		0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 
		0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
unsigned short P[16]={
        1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16
        };
unsigned int S1[16]={
	 	0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 
		0x7,0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5};


int main(){
	
	clock_t start, finish;
	double duratime;
	unsigned int Input, Output;
	printf("\n   SPN_BASE");
	int op=1;
	printf("------------加密-------------");
	printf("\n 请按16进制输入16位明文");
	scanf("%x", &Input);
//				printf("\n 请按16进制输入32位密钥");
//				scanf("%I32x", &K);
	Output = SPN_En(Input);
	printf("\n 密文为（16进制表示）: %x\n", Output);
				
//				printf("\n 密文为：%d\n", Output);
	getchar();getchar();
	printf("------------解密-------------");
	printf("\n 请按16进制输入16位密文");
	scanf("%x", &Input);
//				printf("\n 请按16进制输入32位密钥");
//				scanf("%x", &K);
	Output = SPN_De(Input);
	printf("\n 明文为（16进制表示）: %x\n", Output);
	getchar();getchar();
	start = clock();
	Output = Linear_analysis();
	finish = clock();
	duratime = (double)(finish-start)/CLOCKS_PER_SEC;
	printf("经过线性密码分析得到32位密钥(16进制表示)为：%x\n", Output);
	printf("所用的时间是：%f 秒\n", duratime);
	getchar();getchar(); 
	start = clock();
	Output = Differential_analysis();
	finish = clock();
	duratime = (double)(finish-start)/CLOCKS_PER_SEC;
	printf("经过差分密码分析得到32位密钥(16进制表示)为：%x\n",Output); 
	printf("所用的时间是：%f 秒\n", duratime);
	getchar();getchar(); 
	return 0;
}

unsigned int SPN_En(unsigned int Input){
	unsigned int Output;
	unsigned int i;
	unsigned int w[nr]={0};
	unsigned int u[nr]={0};
	unsigned int v[nr]={0};
	
	for (i=0;i<=nr;i++){
		Key[i]=((K<<(4*i))&0xFFFF0000)>>16;		//产生轮秘钥 
	}
	w[0] = Input;
	for(i=0;i<nr;i++){
		u[i] = w[i]^Key[i];
		v[i] = S_Box(u[i]);
		if(i<nr-1)
		{
			w[i+1]=P_Box(v[i]);
		}
		else
			continue; 
	}
	Output = v[nr-1]^Key[nr];
	return Output;
}

unsigned int S_Box(unsigned int In){
	unsigned int Out=0;
	unsigned int A1,A2,A3,A4;
	A1 = (In&0xF000)>>12;
	A2 = (In&0x0F00)>>8;
	A3 = (In&0x00F0)>>4;
	A4 = (In&0x000F);
	A1 = S[A1];
	A2 = S[A2];
	A3 = S[A3];
	A4 = S[A4];
	A1 = A1<<12;
	A2 = A2<<8;
	A3 = A3<<4;
	Out = A1|A2|A3|A4; 
	return Out;
}
unsigned int P_Box(unsigned int In){
	unsigned int Out=0;
    unsigned int C1[16];
    unsigned int C2[16];
    char i;
    for(i=15;i>=0;i--)
    {
        C1[i]=In%2;			//判断最低位是0还是1 
        In/=2;				//右移一位 
    }
    for(i=0;i<16;i++)
    {
        C2[i]=C1[P[i]-1];
    }
    for(i=0;i<16;i++)
    {
        Out=Out*2+C2[i];
    }
    return Out;
}
unsigned int SPN_De(unsigned int Code){
	unsigned int Output;
	unsigned int w[nr]={0};
	unsigned int u[nr]={0};
	unsigned int v[nr]={0};
	int i;
    for(i=0;i<=nr;i++){
    	Key[i]=(K>>(4*i))&0x0000FFFF;
	}

	w[0]=Code;
	v[0]=w[0]^Key[0];
	u[0]=S1_Box(v[0]);
//	w[1]=Key[1]^u[0];
	for(i=1;i<nr;i++){
		if(i==1){
			w[i]=Key[i]^u[0];
		}
		else{
			w[i]=Key[i]^u[i-1];
		}
		v[i]=P_Box(w[i]);
		u[i]=S1_Box(v[i]);
	}
	Output = u[nr-1]^Key[nr];
	return Output;
} 
unsigned int S1_Box(unsigned int In){
	unsigned int Out=0;
	unsigned int A1,A2,A3,A4;
	A1 = (In&0xF000)>>12;
	A2 = (In&0x0F00)>>8;
	A3 = (In&0x00F0)>>4;
	A4 = (In&0x000F);
	A1 = S1[A1];
	A2 = S1[A2];
	A3 = S1[A3];
	A4 = S1[A4];
	A1 = A1<<12;
	A2 = A2<<8;
	A3 = A3<<4;
	Out = A1|A2|A3|A4; 
	return Out;
}
//  unsigned int P1_Box(unsigned int In){
//	unsigned int Out;
//	unsigned int C1[16];
//    unsigned int C2[16];
//    int i;
//    for(i=15;i>=0;i--)
//    {
//        C1[i]=In%2;			//判断最低位是0还是1 
//        In/=2;				//右移一位 
//    }
//    for(i=0;i<16;i++)
//    {
//        C2[i]=C1[P[i]-1];
//    }
//    for(i=0;i<16;i++)
//    {
//        Out=Out*2+C2[i];
//    }
//    return Out;
//}

void Bit(int i, unsigned int data, unsigned int b[]){
	int j;
	for(j=i-1;j>=0;j--){
		b[j]= data %2;
		data/=2;
	} 
}
unsigned int Linear_analysis(){
	clock_t start, finish;
	double duratime;
	unsigned int L1[4]={0};
    unsigned int L2[4]={0};
	unsigned int Plaintext[16], Plaintext_[16];
	unsigned int Cipher[16];
	int Count[16][16]={0};
	unsigned int v2[4],v4[4],u2[4],u4[4],y2[4],y4[4];
	unsigned int Plain, C, V_2, V_4, U_2, U_4, C_test1, C_test2, C_, Plain_;
    int i,j,k,p,q,z,max;
	unsigned int maxkey,maxkey1,maxkey2;
	unsigned int k1,k2,k3;
	unsigned char flag1=1;
	
	srand(time(NULL));
	start = clock();
	for(k=0;k<N;k++){
		for(p=0; p<16; p++){
			Plaintext[p] = (rand()%2);		//随机生成明文对 
		}
		Plain = Plaintext[0]<<15|Plaintext[1]<<14|Plaintext[2]<<13|Plaintext[3]<<12|Plaintext[4]<<11|Plaintext[5]<<10|Plaintext[6]<<9|Plaintext[7]<<8|Plaintext[8]<<7|Plaintext[9]<<6|Plaintext[10]<<5|Plaintext[11]<<4|Plaintext[12]<<3|Plaintext[13]<<2|Plaintext[14]<<1|Plaintext[15];
		C = SPN_En(Plain);
		
		for(p=0; p<16; p++){
			Plaintext_[p] = (rand()%2);		//随机生成明文对 
		}
		Plain_ = Plaintext_[0]<<15|Plaintext_[1]<<14|Plaintext_[2]<<13|Plaintext_[3]<<12|Plaintext_[4]<<11|Plaintext_[5]<<10|Plaintext_[6]<<9|Plaintext_[7]<<8|Plaintext_[8]<<7|Plaintext_[9]<<6|Plaintext_[10]<<5|Plaintext_[11]<<4|Plaintext_[12]<<3|Plaintext_[13]<<2|Plaintext_[14]<<1|Plaintext_[15];
		C_ = SPN_En(Plain_);
		
	    Bit(16, C, Cipher);
		for(i=0;i<4;i++){
			y2[i]=Cipher[4+i];
		}
		for(j=0;j<4;j++){
			y4[j]=Cipher[12+j];
		}
		for(i=0; i<16; i++){
			for(j=0;j<16;j++){
				Bit(4,i,L1);
				Bit(4,j,L2);
				V_2 =(v2[0]<<3)|(v2[1]<<2)|(v2[2]<<1)|v2[3];
				V_4 =(v4[0]<<3)|(v4[1]<<2)|(v4[2]<<1)|v4[3];
				U_2 = S1[V_2];
				U_4 = S1[V_4];
				Bit(4, U_2, u2);
				Bit(4, U_4, u4);
				z = (Plaintext[4])^(Plaintext[6])^(Plaintext[7])^u2[1]^u2[3]^u4[1]^u4[3];
			}
		}	
	}
	max = -1;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			Count[i][j] = abs(Count[i][j]-N/2);
			if (Count[i][j] > max){
				max = Count[i][j];
				maxkey1 = i;
				maxkey2 = j;
			}
		}
	}
	finish = clock();
	duratime = (double)(finish-start)/CLOCKS_PER_SEC;
	printf("所用的时间是：%f 秒\n", duratime);
	for(k1=0;k1<=0xffff&&flag1==1;k1++){
		for(k2=0;k2<=0xf&&flag1==1;k2++){
			for(k3=0;k3<=0xf&&flag1==1;k3++){
				K=(k1<<16)|(k2<<12)|(maxkey1<<8)|(k3<<4)|maxkey2;
				C_test1= SPN_En(Plain);
				C_test2= SPN_En(Plain_);
				if(C_test1==C&&C_test2==C_)
				{
					maxkey = K;
					flag1=0;
			}
				else 
					flag1=1;
			}
			}
		}
	
	return maxkey;
}
unsigned int Differential_analysis(){
	clock_t start, finish;
	double duratime;
	unsigned int L1[4]={0};
    unsigned int L2[4]={0};
	unsigned int X0,X1,Y1,Y0;
	unsigned int V2_0,V4_0,V2_1,V4_1,U2_0,U4_0,U2_1,U4_1,U2,U4,maxkey,maxkey1,maxkey2,C_test1,C_test2;
	int check[4]={0,1,1,0};
	unsigned int y0[16],y1[16];
	int x0[16],x1[16];
	int x[16]={0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0};
	int Cipher0[16]={0}, Cipher1[16]={0};
	unsigned int v2_0[4],v4_0[4],u2[4],u4[4],v2_1[4],v4_1[4],u2_1[4],u4_1[4],u2_0[4],u4_0[4];
	int y0_2[4],y0_4[4],y1_2[4],y1_4[4];
	int Count[16][16]={0};
	int i,j,k,max,p,q;
	unsigned int k1,k2,k3;
	unsigned char flag1=1;
	
	
	srand(time(0));
	start = clock();
	for(k=0;k<N;k++){
		for(p=0;p<16;p++){
			x0[p]= rand()%2;
			x1[p]= x0[p]^x[p];
		}
		X0=x0[0]<<15|x0[1]<<14|x0[2]<<13|x0[3]<<12|x0[4]<<11|x0[5]<<10|x0[6]<<9|x0[7]<<8|x0[8]<<7|x0[9]<<6|x0[10]<<5|x0[11]<<4|x0[12]<<3|x0[13]<<2|x0[14]<<1|x0[15];
		X1=x1[0]<<15|x1[1]<<14|x1[2]<<13|x1[3]<<12|x1[4]<<11|x1[5]<<10|x1[6]<<9|x1[7]<<8|x1[8]<<7|x1[9]<<6|x1[10]<<5|x1[11]<<4|x1[12]<<3|x1[13]<<2|x1[14]<<1|x1[15];
		Y0=SPN_En(X0);
		Y1=SPN_En(X1);
		Bit(16, Y0, y0);
		Bit(16, Y1, y1);
		for(i=0;i<4;i++){
			y0_2[i]=y0[i+4];
		}
		for(j=0;j<4;j++){
			y0_4[j]=y0[j+12];
		}
		for(i=0;i<4;i++){
			y1_2[i]=y1[i+4];
		}
		for(j=0;j<4;j++){
			y1_4[j]=y1[j+12];
		}
		if((y0[0]==y1[0])&&(y0[1]==y1[1])&&(y0[2]==y1[2])&&(y0[3]==y1[3])&&(y0[8]==y1[8])&&(y0[9]==y1[9])&&(y0[10]==y1[10])&&(y0[11]==y1[11]))
			for(i=0;i<16;i++)
				for(j=0;j<16;j++){
					Bit(4,i,L1);
					Bit(4,j,L2);
					for(p=0;p<4;p++){
						v2_0[p]=L1[p]^y0_2[p];
					}
					for(q=0;q<4;q++){
						v4_0[q]=L2[q]^y0_4[q];
					}
					V2_0 =(v2_0[0]<<3)|(v2_0[1]<<2)|(v2_0[2]<<1)|v2_0[3];
					V4_0 =(v4_0[0]<<3)|(v4_0[1]<<2)|(v4_0[2]<<1)|v4_0[3];
					U2_0=S1[V2_0];
					U4_0=S1[V4_0];
					
					for(p=0;p<4;p++){
						v2_1[p]=L1[p]^y1_2[p];
					}
					for(q=0;q<4;q++){
						v4_1[q]=L2[q]^y1_4[q];
					}
					V2_1 =(v2_1[0]<<3)|(v2_1[1]<<2)|(v2_1[2]<<1)|v2_1[3];
					V4_1 =(v4_1[0]<<3)|(v4_1[1]<<2)|(v4_1[2]<<1)|v4_1[3];
					U2_1=S1[V2_1];
					U4_1=S1[V4_1];
					U2=U2_0^U2_1;
					U4=U4_0^U4_1;
					Bit(4, U2, u2);
					Bit(4, U4, u4);
					if(((u2[0]==0)&&(u2[1]==1)&&(u2[2]==1)&&(u2[3]==0))&&((u4[0]==0)&&(u4[1]==1)&&(u4[2]==1)&&(u4[3]==0))){
						Count[i][j]++;
					}
				}
	}
	max = -1;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			if(Count[i][j]>max){
				max = Count[i][j];
				maxkey1=  i;
				maxkey2=  j;
			}
		}
	}
	finish = clock();
	duratime = (double)(finish-start)/CLOCKS_PER_SEC;
	printf("所用的时间是：%f 秒\n", duratime);
	for(k1=0;k1<=0xffff&&flag1==1;k1++){
		for(k2=0;k2<=0xf&&flag1==1;k2++){
			for(k3=0;k3<=0xf&&flag1==1;k3++){
				K=(k1<<16)|(k2<<12)|(maxkey1<<8)|(k3<<4)|maxkey2;
				C_test1= SPN_En(X0);
				C_test2= SPN_En(X1);
				if(C_test1==Y0&&C_test2==Y1)
				{
					maxkey = K;
					flag1=0;
			}
				else 
					flag1=1;
			}
			}
		}
	return maxkey;
}
