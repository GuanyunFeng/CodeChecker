#include "head.h"
int LEN = 36;//共36个字符

void RainBitget(unsigned char p[6])
{
	int i, a;
	for (i = 0; i < 6; i++) {
		a = rand() % 36;
		p[i] = PASSCHAR[a];
	}
	return;
}

void GetNode(unsigned char p0[6], unsigned char pn[6])
{
	int i;
	unsigned char digest[16];
	RainBitget(p0);
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, p0, 6);
	MD5_Final(digest, &ctx);
	rhanshu(pn, digest, 0);
	for (i = 1; i < 36; i++) {
		MD5_Init(&ctx);
		MD5_Update(&ctx, pn, 6);
		MD5_Final(digest, &ctx);
		rhanshu(pn, digest, 0);
	}
	return;
}

void GetRainbow(struct P0_PN *first)
{
	int i;
	int Round = 100000;
	struct  P0_PN* pos = first;
	GetNode(first->p0, first->pn);
	for (i = 0; i < Round; i++)
	{
		struct  P0_PN* newprior = (struct P0_PN*)malloc(sizeof(P0_PN));
		newprior->pre = pos;
		newprior->next = NULL;
		pos->next = newprior;
		GetNode(newprior->p0, newprior->pn);
		pos = newprior;
	}
	return;
}

struct P0_PN *hit(unsigned char* rainbow, struct P0_PN * first)//判断彩虹链中储存的末尾值是否符合
{
	int i, flag = 1;
	struct P0_PN *pos = (struct P0_PN*)malloc(sizeof(P0_PN));
	pos = first;
	while (pos->next != NULL) 
	{
		for (i = 0; i < 6; i++) {
			if (rainbow[i] != pos->pn[i]) {
				flag = 0;
				break;
			}
		}
		if (flag == 0) {
			flag = 1;
			pos = pos->next;
		}
		else return pos;
	}

	return NULL;
}

int LookUp(unsigned char hash[16], struct P0_PN * first)
{
	int i, j = 0, k;
	unsigned char *temp = (unsigned char*)malloc(6 * sizeof(unsigned char));
	unsigned char *digest = (unsigned char*)malloc(16 * sizeof(unsigned char));
	struct P0_PN *pos;
	rhanshu(temp, hash, 0);
	pos = hit(temp, first);
	for (k = 0; k <36; k++) {
		if (pos != NULL)
		{
			printf("找到对应明文为:");
			for (j = 0; j < (35 - k); j++) {
				MD5_CTX ctx;
				MD5_Init(&ctx);
				MD5_Update(&ctx, pos->p0, 6);
				MD5_Final(digest, &ctx);
				rhanshu(pos->p0, digest, 0);
			}
			for (i = 0; i < 6; i++)
				printf("%c", pos->p0[i]);
			return 1;
		}
		MD5_CTX ctx;
		MD5_Init(&ctx);
		MD5_Update(&ctx, temp, 6);
		MD5_Final(digest, &ctx);
		rhanshu(temp, digest, 0);
		pos = hit(temp, first);
	}
	printf("未找到");
	return 2;
}






void rhanshu(unsigned char *encrypt, unsigned char *decrypt, int k)
{
	int i;
	unsigned char x, y;
	k = k % 36;
	for (i = 0; i < 6; i++) {
		x = decrypt[i] ^ decrypt[(i + 1) & 0xf] ^ decrypt[(i + 2) & 0xf] ^ decrypt[(i + 3) & 0xf];
		y = decrypt[k & 0xf] + decrypt[(k - 1) & 0xf] + decrypt[(k - 2) & 0xf] + decrypt[(k - 3) & 0xf];
		encrypt[i] = PASSCHAR[(x + y) % 36];
		k = k + k + k + 1;
	}
}

//#include <stdio.h>
//#include <openssl/bn.h>
#include "head.h"

int main(void)
{
	int i=1;
	while (i == 1) {
		int op;
		BIGNUM * p = BN_new();
		BIGNUM * q = BN_new();
		BIGNUM * n = BN_new();
		BIGNUM * e = BN_new();
		BIGNUM * d = BN_new();
		BIGNUM * m = BN_new();//m为要加密的数据
		BIGNUM * c = BN_new();//密文
		BIGNUM * m1 = BN_new();//模重复解密结果
		BIGNUM * m2 = BN_new();//中国剩余定理解密结果
		BIGNUM * m3 = BN_new();//蒙哥马利模重复平方解密
		EC_KEY * key_ecc =  EC_KEY_new();
	    unsigned char key_s[EVP_MAX_KEY_LENGTH];//AES密钥
		unsigned char dekey_s[EVP_MAX_KEY_LENGTH];//存储AES解密后得到的RSA密钥
	    unsigned char iv[EVP_MAX_IV_LENGTH];//AES初始向量
		unsigned char mima[6];
		unsigned char hash[50];
		unsigned char hash1[50];
		unsigned int sig_len = 0;
		unsigned char * signature = NULL;
		unsigned char * string = NULL;
		//unsigned char * string2 = NULL;
		int strlen = 0;
		//FILE *fp = fopen("hash.txt", "w");
		FILE *fp = fopen("test2.txt", "w");
		
		unsigned char * string2 = (unsigned char*)malloc(1024 * sizeof(unsigned char));
		unsigned char * string3 = (unsigned char*)malloc(1024 * sizeof(unsigned char));//用于存储AES加密后的密文
		unsigned char * string4 = (unsigned char*)malloc(1024 * sizeof(unsigned char));//用于存储AES解密后的密文；
		unsigned char * pubkey = (unsigned char*)malloc(1024 * sizeof(unsigned char));//用于存储rsa公钥
		unsigned char * prikey = (unsigned char*)malloc(1024 * sizeof(unsigned char));//用于存储rsa私钥
		unsigned char * key_rsaed = (unsigned char*)malloc(1024 * sizeof(unsigned char));
		unsigned char * aim = (unsigned char*)malloc(1024 * sizeof(unsigned char));
		unsigned char * last = (unsigned char*)malloc(1024 * sizeof(unsigned char));
		RSA *rsaKey = RSA_generate_key(1024, 65537, NULL, NULL);
		//BIGNUM * zhishu = BN_new();
		//BIGNUM * dishu = BN_new();
		//BIGNUM * t = BN_new();
		//BIGNUM * zhi = BN_new();
		system("cls");
		printf("请选择要执行的操作：\n");
		printf("*************************************************\n");
		printf("      1、RSA参数生成\n");
		printf("      2、模重复平方、中国剩余定理、蒙特利尔解密\n");
		printf("      3、PGP文件加解密\n");
		printf("      4、彩虹表\n");
		printf("*************************************************\n");
		scanf_s("%d", &op);
		switch (op) {

		case 4:
			system("cls");
			printf("请选择操作：\n");
			printf("1、生成彩虹表\n");
			printf("2、解密\n");
			scanf("%d", &op);
			if (op == 1) {
				first->pre = NULL;
				first->next = NULL;
				srand(time(0));//设定随机数种子
				GetRainbow(first);
				struct P0_PN *poss = first;
				FILE *FP;
				fopen_s(&FP, "rainbow.txt", "w");
				while (poss->next != NULL) {
					for (int i = 0; i < 6; i++) fprintf(FP, "%c", poss->p0[i]);
					fprintf(FP, " ");
					for (int j = 0; j < 6; j++) fprintf(FP, "%c", poss->pn[j]);
					fprintf(FP, "\n");
					poss = poss->next;
				}
				fclose(FP);
				printf("彩虹表生成成功\n");
			}
			else if (op == 2) {
				printf("请输入要查找的六位哈希值:");
				scanf("%s", mima);
				for (int i = 0; i < 23; i++) {
					MD5_CTX ctx;
					MD5_Init(&ctx);
					MD5_Update(&ctx, mima, 6);
					MD5_Final(hash, &ctx);
					rhanshu(mima, hash, 0);
				}
				for (op = 0; op < 6; op++) printf("%c", mima[op]);
				printf("\n");
				MD5_CTX ctx;
				MD5_Init(&ctx);
				MD5_Update(&ctx, mima, 6);
				MD5_Final(hash, &ctx);
				LookUp(hash, first);
			}
			else printf("无效输入\n");

			getchar();
			getchar();
		}
	}
	return 0;
}














