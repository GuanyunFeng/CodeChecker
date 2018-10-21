#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<time.h>
#include<assert.h>
#include<unordered_map>

#define LEN 16	  //加密块的bit长度
#define NUM 50000  //密码攻击时获取的明文密文对数量
#define ROUND 5   //加密的轮数


const int s[16] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
const int s_1[16] = { 14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5 };
const int p[16] = { 1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16 };

//函数声明
void Encrypt(bool x[16], bool localKey[5][16]); //加密
void Encrypt(bool x[16], bool key[32]);	        //加密重载
void Decrypt(bool x[16], bool localKey[5][16]); //解密
void Decrypt(bool x[16], bool key[32]);			//解密重载
void GetLocalKey(bool key[32], bool localKey[5][16]);
//加解密过程使用的函数
void XorArry(bool a[], bool b[], int len);
void SMove(bool a[16]);  //S盒代换
void S_1Move(bool a[16]);
void SMove_1(bool a[4]); //S盒逆置换
void PMove(bool a[16]);  //P盒置换
void Print16bit(bool a[16]);  //打印长度为16的bool型数组
bool IsEqual(bool a[], bool b[], int len);
bool IsZero(bool a[], int len);
//类型转换(不必须)
void NumToArry(unsigned num, bool arry[], int len);
void ArryToNum(unsigned num, bool arry[], int len);
//密码攻击与分析
void LineAttack(bool result[32], bool key[32]);       //线性攻击
void DifferAnalysis(bool result[32], bool key[32]);   //差分分析
void ViolentCrack(bool result[32], bool key[32]);	    //暴力破解
void GetData(bool plain[NUM][16], bool crypted[NUM][16], bool key[32]);



void Encrypt(bool x[16], bool k[5][16]) {
	for (int i = 0; i < 4; i++) {
		XorArry(x, k[i], 16);
		//printf("u%d: ", i + 1);
		//Print16bit(x);
		SMove(x);
		//printf("v%d: ", i + 1);
		//Print16bit(x);
		if (i == 3) {
			XorArry(x, k[4], 16);
			//printf("result: ");
			//Print16bit(x);
			break;
		}
		PMove(x);
		//printf("w%d: ", i + 1);
		//Print16bit(x);
	}
}

void Encrypt(bool x[16], bool key[32]){
	bool localKey[5][16];
	GetLocalKey(key, localKey);
	SPN::Encrypt(x, localKey);
}

void Decrypt(bool x[16], bool k[5][16]){
	XorArry(x, k[4], 16);
	S_1Move(x);
	XorArry(x, k[3], 16);
	for(int i = 2; i >= 0; i--){
		PMove(x);
		S_1Move(x);
		XorArry(x, k[i], 16);
	}
}

void Decrypt(bool x[16], bool key[32]){
	bool localKey[5][16];
	GetLocalKey(key, localKey);
	SPN::Decrypt(x, localKey);
}

void XorArry(bool a[], bool b[], int len){
	for(int i = 0; i < len; i++){
		if (a[i] != b[i])
			a[i] = true;
		else a[i] = false;
	}
}

void SMove(bool a[16]) {
	bool tmpBool[16];
	int tmp1, tmp2;
	for (int i = 0; i < 16; i+=4) {
		tmp1 = 8 * a[i] + 4 * a[i + 1] + 2 * a[i + 2] + a[i + 3];
		tmp2 = s[tmp1];
		tmpBool[i] = (tmp2 >> 3) & 0x01;
		tmpBool[i+1] = (tmp2 >> 2) & 0x01;
		tmpBool[i+2] = (tmp2 >> 1) & 0x01;
		tmpBool[i+3] = tmp2 & 0x01;
	}
	memcpy(a, tmpBool, 16*sizeof(bool));
	return;
}

void S_1Move(bool a[16]){
	bool tmpBool[16];
	int tmp1, tmp2;
	for (int i = 0; i < 16; i+=4) {
		tmp1 = 8 * a[i] + 4 * a[i + 1] + 2 * a[i + 2] + a[i + 3];
		tmp2 = s_1[tmp1];
		tmpBool[i] = (tmp2 >> 3) & 0x01;
		tmpBool[i+1] = (tmp2 >> 2) & 0x01;
		tmpBool[i+2] = (tmp2 >> 1) & 0x01;
		tmpBool[i+3] = tmp2 & 0x01;
	}
	memcpy(a, tmpBool, 16*sizeof(bool));
	return;
}

void SMove_1(bool a[4]){
	int tmp1, tmp2;
	tmp1 = 8 * a[0] + 4 * a[1] + 2 * a[2] + a[3];
	tmp2 = s_1[tmp1];
	a[0] = (tmp2 >> 3) & 0x01;
	a[1] = (tmp2 >> 2) & 0x01;
	a[2] = (tmp2 >> 1) & 0x01;
	a[3] = tmp2 & 0x01;
	return;
}

void PMove(bool a[16]) {
	bool tmpBool[16];
	int tmp1, tmp2;
	for (int i = 0; i < 16; i ++) {
		tmpBool[i] = a[SPN::p[i]-1];
	}
	memcpy(a, tmpBool, 16 * sizeof(bool));
	return;
}

bool IsEqual(bool a[], bool b[], int len){
	for(int i = 0; i < len; i++)
		if(a[i] != b[i]) return false;
	return true;
}

bool IsZero(bool a[], int len){
	for(int i = 0; i < len; i++)
		if(a[i]) return false;
	return true;
}

void Print16bit(bool a[16]) {
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0) printf(" ");
		printf("%d", a[i]);
	}
	printf("\n");
}

void GetLocalKey(bool key[32], bool localKey[5][16]){
	memcpy(localKey[0], key, 16 * sizeof(bool));
	memcpy(localKey[1], key + 4, 16 * sizeof(bool));
	memcpy(localKey[2], key + 8, 16 * sizeof(bool));
	memcpy(localKey[3], key + 12, 16 * sizeof(bool));
	memcpy(localKey[4], key + 16, 16 * sizeof(bool));
}

//类型转换函数

void NumToArry(unsigned num, bool arry[], int len){
	//if(len >= 16) return; // overflow error!
	for(int i = 0; i < len; i++){
		if((num>>i)&0x01)
			arry[i] = true;
		else
			arry[i] = false;
	}
}

void ArryToNum(unsigned num, bool arry[], int len){
	if(len >= 16) return; //overflow error!
	int count = 1;
	num = 0; //num置零，通过加法得到值
	for(int i = 0; i < 16; i++, count*=2){
		if(arry[i]) num += count;
	}
}

// Functions to crake the key

void GetData(bool plain[NUM][16], bool crypted[NUM][16], bool key[32]){
	unsigned short tmp;
	bool tmpBool[16], isUsed[65535];
	memset(isUsed, 0, 65535*sizeof(bool));
	srand((int)time(NULL));
	for(int i = 0; i < NUM; i++){
		tmp = rand()%65535; //unsigned short类型最大65535
		if(!isUsed[tmp]){
			isUsed[tmp] = true;
			SPN::NumToArry((unsigned)tmp, tmpBool, 16);
			memcpy(plain[i], tmpBool, 16*sizeof(bool));
			SPN::Encrypt(tmpBool, key);
			memcpy(crypted[i], tmpBool, 16*sizeof(bool));
		}
		else i--;
	}
	return;
}

//key为加密密钥，result为线性攻击得到的结果(通过线性攻击可得到21-28这8bit，其余使用暴力破解)
void LineAttack(bool result[32], bool key[32]){
	int l1, l2, res, best = 0, count[16][16]; //l1, l2为子密钥, count为计数器
	bool arryL1[4], arryL2[4];      //子密钥的bool数组形式
	bool plain[NUM][16], crypted[NUM][16], tmp[16]; //plain[i]和crypted[i]为一个明密文对。
	//初始化
	SPN::GetData(plain, crypted, key);
	for(int i = 0; i < 16; i++)
		memset(count, 0, 16 * sizeof(int));
	memset(result, 0, 32 * sizeof(bool));
	//算法计数
	for(int i = 0; i < NUM; i++){
		for(l1 = 0; l1 < 16; l1++){
			for(l2 = 0; l2 < 16; l2++){
				SPN::NumToArry(l1, arryL1, 4);
				SPN::NumToArry(l2, arryL2, 4);
				SPN::XorArry(arryL1, crypted[i] + 4, 4);
				SPN::XorArry(arryL2, crypted[i] + 12, 4);
				SPN::SMove_1(arryL1);
				SPN::SMove_1(arryL2);	
				res = plain[i][4]^plain[i][6]^plain[i][7]^arryL1[1]^arryL1[3]^arryL2[1]^arryL2[3];
				if(!res) 
					count[l1][l2]++;
			}
		}
	}

	//set subkey
	for(l1 = 0; l1 < 16; l1++){
		for(l2 = 0; l2 < 16; l2++){
			if(best < abs(count[l1][l2] - NUM/2)){
				best = abs(count[l1][l2] - NUM/2);
				SPN::NumToArry(l1, result + 20, 4);
				SPN::NumToArry(l2, result + 28, 4);
			}
		}
	}
	return;
}

void DifferAnalysis(bool result[32], bool key[32]){
	int l1, l2, res, best = 0, count[16][16]; //l1, l2为子密钥, count为计数器
	bool arryL1[4], arryL2[4], tmpArry[4], consta[4] = { 0, 1, 1, 0};      //子密钥的bool数组形式
	bool x[16]={0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0};
	bool plain[NUM][16], crypted[NUM][16], tmp[16]; //plain[i]和crypted[i]为一个明密文对。
	//初始化
	SPN::GetData(plain, crypted, key);
	for(int i = 0; i < 16; i++)
		memset(count, 0, 16 * sizeof(int));
	memset(result, 0, 32 * sizeof(bool));
	//算法计数
	for(int i = 0; i < NUM; i++){
		memcpy(tmp, plain[i], 16*sizeof(bool));
		XorArry(tmp, x, 16);
		SPN::Encrypt(tmp, key);
		if(IsEqual(tmp, crypted[i], 4) && IsEqual(tmp + 8, crypted[i] + 8, 4)){
					SPN::NumToArry(l1, arryL1, 4);
					SPN::NumToArry(l2, arryL2, 4);
					memcpy(tmpArry, arryL1, 4*sizeof(bool));
					SPN::XorArry(arryL1, crypted[i] + 4, 4);
					SPN::SMove_1(arryL1);
					SPN::XorArry(tmpArry, tmp + 4, 4);
					SPN::SMove_1(tmpArry);
					XorArry(tmpArry, arryL1, 4);
					if(IsEqual(tmpArry, consta, 4)){
						memcpy(tmpArry, arryL2, 4*sizeof(bool));
						SPN::XorArry(arryL2, crypted[i] + 12, 4);
						SPN::SMove_1(arryL2);
						SPN::XorArry(tmpArry, tmp + 12, 4);
						SPN::SMove_1(tmpArry);
						XorArry(tmpArry, arryL2, 4);
						if(IsEqual(tmpArry, consta, 4))
							count[l1][l2]++;
				
					}
		}	
		}
	}
	//set subkey
	for(l1 = 0; l1 < 16; l1++){
		for(l2 = 0; l2 < 16; l2++){
			//printf("%d\n", count[l1][l2]);
			if(best < count[l1][l2]){
				best = count[l1][l2];
				SPN::NumToArry(l1, result + 20, 4);
				SPN::NumToArry(l2, result + 28, 4);
			}
		}
	}
	return;
}

void ViolentCrack(bool result[32], bool key[32]){
	int tmp;
	bool tmpBool[16], plain[16], crypted[16];
	srand((int)time(NULL));
	tmp = rand()%65536; //unsigned short类型最大65535
	//printf("%d\n", tmp);
	SPN::NumToArry((unsigned)tmp, tmpBool, 16);
	memcpy(plain, tmpBool, 16*sizeof(bool));
	SPN::Encrypt(tmpBool, key);
	memcpy(crypted, tmpBool, 16*sizeof(bool));
	for(unsigned i = 0; i <= 0xfffff; i++){ //0-20bit
		NumToArry(i, result, 20);
		for(unsigned j = 0;  j <= 0x0f; j++ ){ //25-28bit
			memcpy(tmpBool, plain, 16*sizeof(bool));
			NumToArry(j, result + 24, 4);
			SPN::Encrypt(tmpBool, result);
			if(IsEqual(tmpBool, crypted, 16)){
				if(IsEqual(result, key, 32))
					return;
			}
		}
	}
	assert(1); //未穷举出正确的子密钥
}

int main() {
	BuildTxt();
	bool result[32];
	bool key[32] = {1,0,0,1,0,1,1,1,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,0,1,0,0,0,1,0,1};
	unsigned char k[16]{5,10,3,13,4,1,7,11,12,9,7,5,15,8,6,1};
	bool input[16] = { 1,0,0,1,1,0,0,0,1,0,1,1,0,1,0,1 };

	printf("\n\nspn16bit:\n");
	printf("明文：");
	for(int i = 0; i < 16; i++)
		printf("%d", input[i]);
	printf("\n");
	Encrypt(input, key);
	printf("密文：");
	for(int i = 0; i < 16; i++)
		printf("%d", input[i]);
	printf("\n");
	SPN::Decrypt(input, key);
	printf("解密：");
	for(int i = 0; i < 16; i++)
		printf("%d", input[i]);
	printf("\n");

	struct timeval start,end;	
    gettimeofday(&start, NULL );
	printf("\n\n\n线性分析:\n");
	LineAttack(result, key);
	for(int i = 0; i < 32; i++)
		printf("%d", result[i]);
	printf("\n");
    gettimeofday(&end, NULL );
    long timeuse =1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    printf("time=%fms\n",timeuse /1000.0);
	gettimeofday(&start, NULL );		
	ViolentCrack(result, key);
	for(int i = 0; i < 32; i++)
		printf("%d", result[i]);
	printf("\n");
    gettimeofday(&end, NULL );
    timeuse =1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    printf("time=%fms\n",timeuse /1000.0);
	
    gettimeofday(&start, NULL );
	printf("\n\n\n差分分析:\n");
	DifferAnalysis(result, key);
	for(int i = 0; i < 32; i++)
		printf("%d", result[i]);
	printf("\n");
    gettimeofday(&end, NULL );
    timeuse =1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    printf("time=%fms\n",timeuse /1000.0);
	gettimeofday(&start, NULL );		
	ViolentCrack(result, key);
	for(int i = 0; i < 32; i++)
		printf("%d", result[i]);
	printf("\n");
    gettimeofday(&end, NULL );
    timeuse =1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    printf("time=%fms\n",timeuse /1000.0);

	//assert(SPN::IsEqual(result, key, 32));
	return 0;
}