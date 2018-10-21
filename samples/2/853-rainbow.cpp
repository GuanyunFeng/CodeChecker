#define _CRT_SECURE_NO_DEPRECATE
#pragma warning( disable : 4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <openssl/sha.h>

#define LEN 999999
unsigned long *Digest = (unsigned long *)malloc(sizeof(unsigned long) * 5);//存放hash值,SHA1生成的LM_HASH长度为40
char mdString[SHA_DIGEST_LENGTH * 2 + 1];//存放LM_HASH值
unsigned char digest[SHA_DIGEST_LENGTH];
char index[LEN] = { '0' };						  //彩虹表明文索引,已经R得到了的值就不再成为表头
void Generatehash();
void GenerateTable();
int  Ri(unsigned long *Digest, int i);
int R(int s, unsigned long *Digest, int t);
void Hash(int p, unsigned long *Digest);
unsigned long char_to_hex(char input[41], int t);
int Crack();
//unsigned char AscToHex(unsigned char aHex);

int main() {
	int op = 1, plain;
	char P[7];
	while (op){
		printf("\n\n");
		printf("              彩虹表破解HASH                 \n\n");
		printf("-----------------测试菜单--------------------\n\n");
		printf("            1.SHA1得到hash值                 \n\n");
		printf("            2.生成彩虹表                     \n\n");
		printf("            3.破解HASH                       \n\n");
		printf("            0.退出                           \n\n");
		printf("\n\n");
		printf("请选择你的操作:");
		scanf("%d", &op);
		getchar();
		switch (op) {
		case 1:
			Generatehash();
			break;
		case 2:
			GenerateTable();
			break;
		case 3:
			plain = Crack();
			if (plain < 0)
				printf("破解HASH失败！\n");
			else {
				printf("\n破解HASH成功！\n");
				int i,res=0,rem=0;
				char x[7];
				//printf("\n破解出的明文为：%d", plain);
				for (i = 5; i >= 0; i--){
					res = plain % 36;
					plain= plain / 36;
					if ((res >= 0) && (res <= 9))
						x[i] = res + '0';
					else if ((res >= 10) && (res <= 35))
						x[i] = res - 10 + 'a';
					//printf("\n破解出的明文为：%c\n", x[i]);
					//res = rem;
				}
				x[6] = '\0';
				printf("\n破解出的明文为：%06s ", x);
			}
			break;
		case 0:
			printf("\n欢迎下次访问\n");
			break;
		default:
			printf("\n输入错误!\n");
			break;
		}
		printf("\n\n");
		system("pause");
		system("cls");
	} 
}

/*对指定明文生成hash值*/
void Generatehash() {
	int  j, i;
	char *x;//明文x
	x = (char *)malloc(sizeof(char) * 7);
	int x1,sum=0,mul;
	printf("请输入明文：");
	scanf("%s", x);
	getchar();
	for (i = 0; i < 6; i++){
		if ((x[i] >= '0') && (x[i] <= '9'))
			x1 = x[i] - '0';
		else if ((x[i] >= 'a') && (x[i] <= 'z'))
			x1 = x[i] - 'a' + 10;
		
		/*mul = pow(36.0,5-i);
		sum *= mul;*/
		sum =sum * 36 + x1;
	}
	//printf("%d\n", sum);
	sprintf(x, "%06d", sum);
	//printf("x=%s", x);
	x[6] = '\0';
	SHA1((unsigned char *)x, 7, (unsigned char *)&digest);
	printf("\n生成的HASH值为：\n");
	for (j = 0; j < 5; j++)
		for (i = 0; i < 4; i++)
			sprintf(&mdString[(i + j * 4) * 2], "%02x", (unsigned int)digest[3 - i + j * 4]);
	//for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
	//sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);
	printf("%s\n", mdString);
	printf("\n生成hash值成功！\n");
}

//unsigned char AscToHex(unsigned int x){
//	char aHex;
//	if ((aHex >= '0') && (aHex <= '9'))
//		x = aHex -'0';
//	else if ((aHex >= 'a') && (aHex <= 'z'))
//		x = aHex-'a' + 10;
//	return x;
//}

/*生成彩虹表*/
void GenerateTable() {
	FILE *fp;
	if ((fp = fopen("rainbow.txt", "wb+")) == NULL) {
		fputs("File error", stderr);
		exit(1); //若文件错误，退出1
	}
	int i;
	int temp;
	for (i = 0; i < LEN; i++) {
		if (index[i] == 1) continue;//判断节点是否出现过
		temp = R(i, Digest, 10);
		if (temp != 0) {
			fwrite(&i, sizeof(int), 1, fp);
			fwrite(&temp, sizeof(int), 1, fp);
		}
	}
	fclose(fp);
	printf("\n生成彩虹表成功！\n");
}

/*彩虹表R函数*/
int  Ri(unsigned long *Digest, int i) {
	unsigned long point = 0;
	point += Digest[1] + i - 1;
	index[point % LEN] = 1;
	return point % LEN;
}

/*彩虹表R函数链:减少碰撞概率，分布均匀*/
int R(int s, unsigned long *Digest, int t) {//i和t构成给定的字符长度范围
	int i;
	unsigned long point = 0;
	for (i = 0; i < t; i++) {
		Hash(s, Digest);
		point = 0;
		point += Digest[1] + i % 5 + i / 5 ;//明文空间为6，R函数要保证运算完后要与铭文空间一致，不存在冗余现象
		index[point % LEN] = 1;
		s = point % LEN;
	}
	return s;
}

/*SHA1方式HASH*/
void Hash(int p, unsigned long *Digest) {
	char *x;
	unsigned char *temp1;
	unsigned long *temp2;
	int j, temp;
	x = (char *)malloc(sizeof(char) * 7);
	for (j = 0; j < 6; j++) {
		x[5 - j] = p % 10 + '0';
		p = p / 10;
	}//将p按位输入到明文数组x中存放
	x[6] = '\0';
	SHA1((unsigned char *)x, 7, (unsigned char *)&digest);
	//char string[];SHA1((unsigned char*)&string, strlen(string), (unsigned char*)&digest);
	temp1 = digest;
	for (j = 0; j < 5; j++) {
		temp2 = (unsigned long*)(temp1);
		Digest[j] = *temp2;
		temp1 += 4;//两位一存
	}//hash值存放到Digest数组中
}

/*字符串转换为32比特十六进制*/
unsigned long char_to_hex(char input[41], int t) {
	int i;
	char temp[8];
	unsigned long sum = 0;
	for (i = 0; i < 8; i++) {
		temp[i] = input[i + 8 * t];
	}
	sscanf(temp, "%lx", &sum);
	return sum;
}

/*破解hash*/
int Crack() {
	int i, j, t, n, flag = 1, k, temp;
	unsigned long *CYPHER;//LM_HASH转换为16进制
	char cypher[41];//LM_HASH值字符串
	int head, tail;
	long lsize;                                   //文件大小
	FILE *fp;
	if ((fp = fopen("rainbow.txt", "rb")) == NULL) {
		fputs("File error", stderr);
		exit(1);
	}
	fseek(fp, 0, SEEK_END);//定位到文件末尾
	lsize = ftell(fp);//得到文件大小
	rewind(fp);//将文件指针重新指向一个流的开头
	n = lsize / (sizeof(int) * 2);                 //有n对表头表尾
	CYPHER = (unsigned long *)malloc(sizeof(unsigned long) * 5);
	printf("请输入密文：");
	scanf("%s", cypher);
	for (j = 0; j < 5; j++)
		CYPHER[j] = char_to_hex(cypher, j);
	for (i = 9; i >= 0; i--) {
		for (j = i; j < 10; j++) {
			k = Ri(Digest, j % 5 + j / 5 + 1);     //对密文R-H重复
			Hash(k, Digest);
		}
		for (j = 0; j < n; j++) {
			fread(&head, sizeof(int), 1, fp);
			fread(&tail, sizeof(int), 1, fp);
			flag = 1;
			for (t = 0; t < 5; t++)
				Digest[t] = CYPHER[t];
			if (tail == k) {
				temp = R(head, Digest, i); //如果相同，将彩虹表的相应表头赋给temp
				Hash(temp, Digest);                //将得到的明文hash，再与输入的密文对比
				for (t = 0; t < 5 ; t++)
					if (Digest[t] != CYPHER[t]) flag = 0;
				if (flag)
					return temp;
			}
		}
		rewind(fp);//将文件指针重新指向一个流的开头
	}
	return -1;
}