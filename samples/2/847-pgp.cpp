#include "stdafx.h"
#include <openssl/md5.h>
#include <openssl/des.h>
#include <openssl/rsa.h>   
#include <openssl/pem.h>
#include <openssl/evp.h>  
#include <openssl/ecdsa.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/ecdsa.h>
#include <openssl/objects.h>
#include <openssl/err.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <stdio.h>
#include <vector>  

#pragma warning(disable:4996)
#define KEY_LENGTH  2048               // 密钥长度   

using namespace std;

typedef struct RSAKEY
{
	unsigned char pubkey[65];
	string deskey;
	string rsaprikey;
} RSAKEY;

typedef struct signcheck
{
	unsigned char * sign;
	unsigned int len;
}signcheck;

RSAKEY pgp_system;
signcheck signfile;


void md5(string &srcStr, string &encodedHexStr)
{  
	unsigned char mdStr[33];
	MD5((const unsigned char *)srcStr.c_str(), srcStr.length(), mdStr);

	char buf[65];
	char tmp[3];
	for (int i = 0; i < 32; i++)
	{
		sprintf(tmp, "%02x", mdStr[i]);
		strcat(buf, tmp);
	}
	buf[32] = '\0'; // 截断   
	encodedHexStr = string(buf);
}

int ecc_sign(string &encodedHexStr)
{
	EC_KEY *ec_key;
	EC_KEY *ec_pri, *ec_pub;
	EC_GROUP *ec_group, *ec_group1;
	unsigned char buf1[1024];
	unsigned char buf2[1024];
	unsigned char *pri, *pub;
	unsigned int len1, len2, sign_len, digest_len, size;

	unsigned char *digest, *sig;
	digest_len = encodedHexStr.length();
	digest = (unsigned char*)encodedHexStr.c_str();

	if ((ec_key = EC_KEY_new()) == NULL)
	{
		printf("Error：EC_KEY_new()\n");
		return 0;
	}

	/* 选择一条椭圆曲线 */
	if ((ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1)) == NULL)
	{
		printf("Error：EC_GROUP_new_by_curve_name()\n");
		EC_KEY_free(ec_key);
		return -1;
	}

	/* 设置密钥参数 */
	int ret;
	ret = EC_KEY_set_group(ec_key, ec_group);
	if (ret != 1)
	{
		printf("Error：EC_KEY_set_group()\n");
		return -1;
	}

	/* 生成密钥对 */
	if (!EC_KEY_generate_key(ec_key))
	{
		printf("Error：EC_KEY_generate_key()\n");
		EC_KEY_free(ec_key);
		return -1;
	}

	pri = buf1;
	len1 = i2d_ECPrivateKey(ec_key, &pri);
	if (!len1)
	{
		printf("Error：i2d_ECPrivateKey()\n");
		EC_KEY_free(ec_key);
		return -1;
	}

	pub = buf2;
	len2 = i2o_ECPublicKey(ec_key, &pub);
	if (!len2)
	{
		printf("Error：i2o_ECPublicKey()\n");
		EC_KEY_free(ec_key);
		return -1;
	}

	unsigned char privkey[279];
	for (int i = 0; i < len1; i++)
		privkey[i] = buf1[i];

	for (int j = 0; j < len2; j++)
		pgp_system.pubkey[j] = buf2[j];


	pri = privkey;
	size = ECDSA_size(ec_key);
	sig = (unsigned char *)malloc(size);
	ec_pri = NULL;
	ec_pri = d2i_ECPrivateKey(&ec_pri, (const unsigned char**)&pri, sizeof(privkey));

	if (ec_pri == NULL)
	{
		printf("Error：d2i_ECPrivateKey()\n");
		return -1;
	}

	if (!ECDSA_sign(0, digest, digest_len, sig, &sign_len, ec_pri))
	{
		printf("Error：ECDSA_sign()\n");
		EC_KEY_free(ec_pri);
		return -1;
	}

	signfile.sign = sig;
	signfile.len = sign_len;

	EC_KEY_free(ec_key);
	EC_KEY_free(ec_pri);
}



void generateRSAKey(string strKey[2])
{
	// 公私密钥对    
	size_t pri_len;
	size_t pub_len;
	char *pri_key = NULL;
	char *pub_key = NULL;

	// 生成密钥对    
	RSA *keypair = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);

	BIO *pri = BIO_new(BIO_s_mem());
	BIO *pub = BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
	PEM_write_bio_RSAPublicKey(pub, keypair);

	// 获取长度    
	pri_len = BIO_pending(pri);
	pub_len = BIO_pending(pub);

	// 密钥对读取到字符串    
	pri_key = (char *)malloc(pri_len + 1);
	pub_key = (char *)malloc(pub_len + 1);

	BIO_read(pri, pri_key, pri_len);
	BIO_read(pub, pub_key, pub_len);

	pri_key[pri_len] = '\0';
	pub_key[pub_len] = '\0';

	// 存储密钥对    
	strKey[0] = pub_key;
	strKey[1] = pri_key;

	// 内存释放  
	RSA_free(keypair);
	BIO_free_all(pub);
	BIO_free_all(pri);

	free(pri_key);
	free(pub_key);
}

// 公钥加密    
string rsa_pub_encrypt(string &clearText, string &pubKey)
{
	string strRet;
	RSA *rsa = NULL;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)pubKey.c_str(), -1);

	RSA* pRSAPublicKey = RSA_new();
	rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);

	int len = RSA_size(rsa);
	char *encryptedText = (char *)malloc(len + 1);
	memset(encryptedText, 0, len + 1);

	// 加密函数  
	int ret = RSA_public_encrypt(clearText.length(), (const unsigned char*)clearText.c_str(), (unsigned char*)encryptedText, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0)
		strRet = string(encryptedText, ret);

	// 释放内存  
	free(encryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return strRet;
}

string rsa_pri_decrypt(string &cipherText, string &priKey)
{
	string strRet;
	RSA *rsa = RSA_new();
	BIO *keybio;
	keybio = BIO_new_mem_buf((unsigned char *)priKey.c_str(), -1);

	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);

	int len = RSA_size(rsa);
	char *decryptedText = (char *)malloc(len + 1);
	memset(decryptedText, 0, len + 1);

	// 解密函数  
	int ret = RSA_private_decrypt(cipherText.length(), (const unsigned char*)cipherText.c_str(), (unsigned char*)decryptedText, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0)
		strRet = string(decryptedText, ret);

	// 释放内存  
	free(decryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return strRet;
}


string des_encrypt(string clearText)
{
	string cipherText; // 密文   
	string key;
	DES_cblock keyEncrypt;
	DES_random_key(&keyEncrypt);
	key = string((const char *)keyEncrypt);
	// 密钥置换    
	DES_key_schedule keySchedule;
	DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	// 循环加密，每8字节一次    
	const_DES_cblock inputText;
	DES_cblock outputText;
	std::vector<unsigned char> vecCiphertext;
	unsigned char tmp[8];

	if (clearText.length() % 8 != 0)
	{
		int tmp1 = clearText.length() / 8;
		int tmp2 = ((tmp1 + 1) * 8) - clearText.length();
		for (int n = 0; n < tmp2; n++)
			clearText = clearText + "#";
	}

	for (int i = 0; i < clearText.length() / 8; i++)
	{
		memcpy(inputText, clearText.c_str() + i * 8, 8);
		DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
		memcpy(tmp, outputText, 8);

		for (int j = 0; j < 8; j++)
			vecCiphertext.push_back(tmp[j]);
	}

	cipherText.clear();
	cipherText.assign(vecCiphertext.begin(), vecCiphertext.end());

	//使用rsa对密钥进行加密
	string key_rsa[2];
	generateRSAKey(key_rsa);
	pgp_system.deskey = rsa_pub_encrypt(key, key_rsa[0]);
	pgp_system.rsaprikey = key_rsa[1];
	return cipherText;
}

string des_decrypt(string cipherText)
{
	string clearText; // 明文    
	string key;

	//解密得出密钥
	key = rsa_pri_decrypt(pgp_system.deskey, pgp_system.rsaprikey);


	DES_cblock keyEncrypt;
	memset(keyEncrypt, 0, 8);

	if (key.length() <= 8)
		memcpy(keyEncrypt, key.c_str(), key.length());
	else
		memcpy(keyEncrypt, key.c_str(), 8);

	DES_key_schedule keySchedule;
	DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	const_DES_cblock inputText;
	DES_cblock outputText;
	std::vector<unsigned char> vecCleartext;
	unsigned char tmp[8];

	for (int i = 0; i < cipherText.length() / 8; i++)
	{
		memcpy(inputText, cipherText.c_str() + i * 8, 8);
		DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
		memcpy(tmp, outputText, 8);

		for (int j = 0; j < 8; j++)
			vecCleartext.push_back(tmp[j]);
	}

	clearText.clear();
	clearText.assign(vecCleartext.begin(), vecCleartext.end());

	int pos = clearText.find('#', 0);
	clearText = clearText.erase(pos, clearText.size() - pos);

	return clearText;
}

int ecc_ver(string &encodedHexStr)
{
	int ret;
	unsigned int digest_len;
	EC_KEY *ec_key = NULL;
	EC_GROUP *ec_group;
	unsigned char *pub = (unsigned char*)pgp_system.pubkey;

	unsigned char *digest, *sig;
	digest_len = encodedHexStr.length();
	digest = (unsigned char*)encodedHexStr.c_str();

	if ((ec_key = EC_KEY_new()) == NULL)
	{
		printf("Error：EC_KEY_new()\n");
		return -1;
	}
	if ((ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1)) == NULL)
	{
		printf("Error：EC_GROUP_new_by_curve_name()\n");
		EC_KEY_free(ec_key);
		return -1;
	}

	/* 设置密钥参数 */
	ret = EC_KEY_set_group(ec_key, ec_group);
	if (ret != 1)
	{
		printf("Error：EC_KEY_set_group\n");
		EC_KEY_free(ec_key);
		return -1;
	}
	/* 导入公钥 */
	ec_key = o2i_ECPublicKey(&ec_key, (const unsigned char**)&pub, sizeof(pgp_system.pubkey));
	if (ec_key == NULL)
	{
		printf("Error：o2i_ECPublicKey\n");
		EC_KEY_free(ec_key);
		return -1;
	}

	/* 验证签名 */
	if (ECDSA_verify(0, digest, digest_len, signfile.sign, signfile.len, ec_key) == 1)
	{
		printf("sig true!\n");
	}
	else {
		printf("sig err!\n");
	}
	EC_KEY_free(ec_key);
	return ret;
}


int main(void)
{
	string filename, SrcText;
	string HashStr, encodedHashStr;
	string encodedText, clearText;
	string HashCheckStr;
	filename = "D:\\001.txt";

	FILE *fp = fopen(filename.c_str(), "r");
	fseek(fp, 0, SEEK_END);
	size_t file_size;
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *input = (char *)malloc(file_size);
	fread(input, file_size, sizeof(char), fp);
	SrcText = string(input);
	cout << SrcText.c_str();
	fclose(fp);

	printf("signing the file\n");
	md5(SrcText, HashStr);//md5求摘要

	
	encodedHashStr = HashStr;
	ecc_sign(encodedHashStr);//ECC对摘要签名

	//DES文件加密
	printf("des the file\n");
	encodedText = des_encrypt(SrcText);
	printf("save des\n");
	ofstream writefile1("des.txt");
	writefile1 << encodedText.c_str();
	writefile1.close();
	//DES对文件解密
	printf("des encode the file\n");
	clearText = des_decrypt(encodedText);
	printf("saving the desencode\n");
	ofstream writefile2("desencode.txt");
	writefile2 << clearText.c_str();
	writefile2.close();
	//数据完整性及签名验证
	printf("check signature\n");
	md5(clearText, HashCheckStr);
	ecc_ver(HashCheckStr);

	getchar();
	return 0;
}