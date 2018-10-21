#include<stdio.h>
#pragma once
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string>
#include <unordered_map>
#include<memory.h>
#include<openssl/ssl.h>
#include<openssl/ec.h>
#include<openssl/ecdsa.h>
#include<openssl/objects.h>
#include<openssl/err.h>
#include<openssl/sha.h>
#include<openssl/comp.h>
#include <openssl/aes.h>
#include <openssl/crypto.h>
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#define STRLEN 6
#define K 36
#define N 1000000
using namespace std;

unordered_map<string, string> rainbowlist;

unsigned char RGetChar(unsigned char sum);
unsigned char GetRandChar();
void GetRandStr(unsigned char *str, int len);
void RFunction(unsigned char* rainbow, unsigned char* hash, int round);
void BuildList(unsigned char *plain, unsigned char *cipher);
void BuildTxt();
bool SearchTable(unsigned char *digest, unsigned char *plain);
void LoadTxt(char *filepath);
int main() {
	//BuildTxt();
	LoadTxt((char*)"F:\RainbowTable.txt");
	unsigned char plain[7] = "7hrsrd";
	unsigned char digest[SHA_DIGEST_LENGTH], tmp[STRLEN];
	memcpy(tmp, plain, 6 * sizeof(unsigned char));
	memset(plain, 0, 6 * sizeof(unsigned char));
	SHA1(tmp, STRLEN, digest);
	//R(tmp, digest, 0);
	for (int i = 0; i < 20; i++)
		printf("%x", digest[i]);
	printf("\n");
	//SHA1(tmp, STRLEN, digest);
	if (SearchTable(digest, plain))
		printf("the plain is:%s", plain);
	else
		printf("not found\n");
	getchar();
	return 0;
}

unsigned char RGetChar(unsigned char sum) {
	unsigned char result = sum % 36;
	if (result >= 0 && result < 10)
		result = result % 10 + 48;
	else
		result = result % 26 + 97;
	return result;
}

unsigned char GetRandChar() {
	unsigned char result = rand() % 36;
	if (result < 26) result += 97;
	else result += 22;
	return result;
}

void GetRandStr(unsigned char *str, int len) {
	for (int i = 0; i < len; i++)
		str[i] = GetRandChar();
}

void RFunction(unsigned char* rainbow, unsigned char* hash, int round) {
	unsigned char tmp[6];
	tmp[0] = hash[(0 + round * 3) % 20] + hash[(19 + round * 3) % 20] + hash[(6 + round * 3) % 20] + hash[(13 + round * 3) % 20];
	tmp[1] = hash[(1 + round * 3) % 20] + hash[(18 + round * 3) % 20] + hash[(7 + round * 3) % 20] + hash[(12 + round * 3) % 20];
	tmp[2] = hash[(2 + round * 3) % 20] + hash[(17 + round * 3) % 20] + hash[(8 + round * 3) % 20];
	tmp[3] = hash[(3 + round * 3) % 20] + hash[(16 + round * 3) % 20] + hash[(9 + round * 3) % 20];
	tmp[4] = hash[(4 + round * 3) % 20] + hash[(15 + round * 3) % 20] + hash[(10 + round * 3) % 20];
	tmp[5] = hash[(5 + round * 3) % 20] + hash[(14 + round * 3) % 20] + hash[(11 + round * 3) % 20];
	for (int i = 0; i < 6; i++)
		rainbow[i] = RGetChar(tmp[i]);
	return;
}

void BuildList(unsigned char *plain, unsigned char *cipher) {
	unsigned char digest[SHA_DIGEST_LENGTH], tmp[STRLEN];
	memcpy(tmp, plain, 6 * sizeof(unsigned char));
	for (int i = 0; i < K; i++) {
		SHA1(tmp, STRLEN, digest);
		RFunction(cipher, digest, i % 4);
		memcpy(tmp, cipher, 6 * sizeof(unsigned char));
	}
}

void BuildTxt() {
	unsigned char plain[STRLEN], cipher[STRLEN];
	FILE *fp = fopen("F:\RainbowTable.txt", "w");
	if (fp == NULL) return;
	for (int i = 0; i < N; i++) {
		GetRandStr(plain, 6);
		BuildList(plain, cipher);
		/*	for(int j =0; j < 6; j++)
		printf("%c", plain[j]);
		printf(" ");
		for(int j =0; j < 6; j++)
		printf("%c", cipher[j]);
		printf("\n");
		getchar();*/
		fwrite(plain, 1, 6, fp);
		fputc(' ', fp);
		fwrite(cipher, 1, 6, fp);
		fputc('\n', fp);
	}
}

void LoadTxt(char *filepath) {
	unsigned char plain[STRLEN + 1], cipher[STRLEN + 1];
	FILE *fp = fopen(filepath, "r");
	if (fp == NULL) {
		printf("open failed!\n");
		return;
	}
	for (int i = 0; i < N; i++) {
		fread(plain, 1, 6, fp);
		fgetc(fp);
		fread(cipher, 1, 6, fp);
		fgetc(fp);
		plain[6] = '\0';
		cipher[6] = '\0';
		string start = (char*)plain, end = (char*)cipher;
		rainbowlist[end] = start;
		//		cout << rainbowlist.size() << endl;
	}
}

bool SearchTable(unsigned char *digest, unsigned char *plain) {
	unsigned char tmp[STRLEN + 1], tmpDig[SHA_DIGEST_LENGTH];
	int i, j, count = 1;
	bool flag = false;

	for (j = 0; j < 4; j++) {
		memcpy(tmpDig, digest, 20 * sizeof(unsigned char));
		for (i = 0, count = 0; i < K; i++, count++) {
			RFunction(tmp, tmpDig, (i + j) % 4);
			tmp[6] = '\0';
			if ((i + j + 1) % 4 == 0) {
				string p = (char*)tmp;
				cout << i << " " << j << " " << p << endl;
				unordered_map<string, string>::iterator iter;
				iter = rainbowlist.find(p);
				if (iter != rainbowlist.end()) {
					strcpy((char*)plain, iter->second.c_str());
					cout << iter->second << endl << iter->first << endl;
					flag = true;
					goto END;
				}
			}
			SHA1(tmp, STRLEN, tmpDig);
		}
		printf("\n");
	}
END:
	if (!flag) return false;

	for (int n = 0; n < K - count - 1; n++) {
		SHA1(plain, STRLEN, tmpDig);
		RFunction(plain, tmpDig, n % 4);
	}
	return true;
}