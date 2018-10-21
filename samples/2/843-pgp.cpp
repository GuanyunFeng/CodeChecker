#pragma once
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<string.h>
#include<assert.h>
#include<openssl/ssl.h>
#include<openssl/ec.h>
#include<openssl/ecdsa.h>
#include<openssl/objects.h>
#include<openssl/err.h>
#include<openssl/sha.h>
#include<openssl/md5.h>
#include<openssl/comp.h>
#include<openssl/evp.h>
#include<openssl/aes.h>
#include<openssl/crypto.h>
#include<openssl/rsa.h>
#include<openssl/bn.h>
#define FILEMAX 1024*1024*10 //10MB

void Encrypt(char * password, const char* filepath_pub, const char* filepath_pri, const char* filepath_in, const char* filepath_out);
bool Decrypt(const char * filepath_pub, const char * filepath_pri, const char * filepath_in, const char * filepath_out);
	void CreateECKeys(const char * filepath_pub, const char * filepath_pri);
	void CreateRSAKeys(const char * filepath_pub, const char * filepath_pri);

void CreateECKeys(const char * filepath_pub, const char * filepath_pri);
int main() {
	CreateECKeys("F:\EC.pub", "F:\EC.pri");
	CreateRSAKeys("F:\RSA.pub", "F:\RSA.pri");
	Encrypt((char*)"fgy9811", "F:\RSA.pub", "F:\EC.pri", "F:\p.txt", "F:\cipher.txt");
	Decrypt("F:\EC.pub", "F:\RSA.pri", "F:\cipher.txt", "F:\plain.txt");
}
void Encrypt(char * password, const char* filepath_pub, const char* filepath_pri, const char* filepath_in, const char* filepath_out)
{
	unsigned char dig[1024], sig[1024], buf3[1024];
	unsigned char *msg = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char)),
		*output = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char)),
		*buf1 = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char)),
		*buf2 = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char));
	int msg_len = 0;
	FILE* fp1 = fopen(filepath_in, "rb");
	FILE* fp2 = fopen(filepath_out, "wb+");
	unsigned char* p = buf1;
	char c = fgetc(fp1);
	while (c != EOF)
	{
		msg[msg_len] = c;
		msg_len++;
		c = fgetc(fp1);
	}
	unsigned degLen, sigLen;
	printf("明文\n");
	for (int i = 0; i < msg_len; i++)
	{
		printf("%02X", msg[i]);
	}
	printf("\n");

	//hash
	EVP_MD_CTX * md_ctx = EVP_MD_CTX_new();
	EVP_MD_CTX_init(md_ctx);
	EVP_DigestInit(md_ctx, EVP_sha1());
	EVP_DigestUpdate(md_ctx, msg, msg_len);
	EVP_DigestFinal(md_ctx, dig, &degLen);
	EVP_MD_CTX_free(md_ctx);
	printf("摘要\n");
	for (int i = 0; i <degLen; i++)
	{
		printf("%02X", dig[i]);
	}
	printf("\n");

	//签名
	EC_KEY * key = NULL;
	BIO *file = BIO_new_file(filepath_pri, "rb");
	key = PEM_read_bio_ECPrivateKey(file, NULL, NULL, NULL);
	ECDSA_sign(0, dig, degLen, sig, &sigLen, key);
	BIO_free(file);
	EC_KEY_free(key);
	printf("签名\n");
	for (int i = 0; i <sigLen; i++)
	{
		printf("%02X", sig[i]);
	}
	printf("\n");

	//将签名与文件信息拼接在一起
	int allLen = sigLen + msg_len + 8;
	memcpy(buf1, &sigLen, sizeof(unsigned int));
	memcpy(buf1 + 4, sig, sigLen * sizeof(unsigned char));
	memcpy(buf1 + sigLen + 4, &msg_len, sizeof(unsigned));
	memcpy(buf1 + sigLen + 8, msg, msg_len * sizeof(char));
	printf("拼接\n");
	for (int i = 0; i <allLen; i++)
	{
		printf("%02X", buf1[i]);
	}
	printf("\n");

	//对称加密
	unsigned char key_s[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	OpenSSL_add_all_ciphers();
	memcpy(buf2, password, strlen(password) * sizeof(char));
	EVP_BytesToKey(EVP_aes_256_cfb(), EVP_md5(), NULL, buf2, strlen(password), 10, key_s, iv);
	EVP_EncryptInit(ctx, EVP_aes_256_cfb(), key_s, iv);
	int cipLen = 0, tmpLen;
	unsigned char *pBuf1 = buf1, *pBuf2 = buf2;
	while (1) {
		if ((int)(allLen - EVP_MAX_BLOCK_LENGTH) > 0)
		{
			EVP_EncryptUpdate(ctx, pBuf2, &tmpLen, pBuf1, EVP_MAX_BLOCK_LENGTH);
			pBuf1 += EVP_MAX_BLOCK_LENGTH;
			pBuf2 += tmpLen;
			cipLen += tmpLen;
			allLen -= EVP_MAX_BLOCK_LENGTH;
		}
		else
		{
			EVP_EncryptUpdate(ctx, pBuf2, &tmpLen, pBuf1, allLen);
			cipLen += tmpLen;
			break;
		}
	}
	EVP_EncryptFinal(ctx, pBuf2, &tmpLen);
	cipLen += tmpLen;
	printf("AES加密\n");
	for (int i = 0; i <cipLen; i++)
	{
		printf("%02X", buf2[i]);
	}
	printf("\n");

	//RSA非对称加密密钥,将加密结果存入buf3中
	memcpy(buf1, key_s, EVP_MAX_KEY_LENGTH * sizeof(unsigned char));
	memcpy(buf1 + EVP_MAX_KEY_LENGTH, iv, EVP_MAX_IV_LENGTH * sizeof(unsigned char));
	unsigned keyLen;
	RSA* rsa = RSA_new();
	file = BIO_new_file(filepath_pub, "rb");
	rsa = PEM_read_bio_RSAPublicKey(file, NULL, NULL, NULL);
	EVP_PKEY * rsaKey = EVP_PKEY_new();
	EVP_PKEY_set1_RSA(rsaKey, rsa);
	RSA_free(rsa);
	BIO_free(file);
	EVP_PKEY_CTX * ctx2 = EVP_PKEY_CTX_new(rsaKey, NULL);
	OpenSSL_add_all_ciphers();
	EVP_PKEY_encrypt_init(ctx2);
	EVP_PKEY_encrypt(ctx2, buf3, (size_t*)&keyLen, buf1, EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH);
	EVP_PKEY_CTX_free(ctx2);
	EVP_PKEY_free(rsaKey);
	printf("RSA\n");
	for (int i = 0; i <16; i++)
	{
		printf("%02X", buf3[i]);
	}
	printf("\n");

	//将加密后的密钥与之前的结果拼接在一起
	memcpy(output, &keyLen, sizeof(unsigned));
	memcpy(output + 4, buf3, keyLen * sizeof(unsigned char));
	memcpy(output + 4 + keyLen, &cipLen, sizeof(int));
	memcpy(output + 4 + keyLen + 4, buf2, cipLen * sizeof(unsigned char));

	//写文件
	fwrite(output, 1, keyLen + cipLen + 8, fp2);
	printf("密文\n");
	for (int i = 0; i <keyLen + allLen + 8; i++)
	{
		printf("%02X", output[i]);
	}
	printf("\n");
	EVP_CIPHER_CTX_free(ctx);
	free(msg);
	free(output);
	free(buf1);
	free(buf2);
	fclose(fp1);
	fclose(fp2);
}


bool Decrypt(const char * filepath_pub, const char * filepath_pri, const char * filepath_in, const char * filepath_out) {
	unsigned char dig[1024], sig[1024];
	unsigned len1, len2;
	unsigned sig_len, msg_len, dig_len;
	unsigned char *msg = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char)),
		*buf1 = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char)),
		*buf2 = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char));

	FILE *fp1 = fopen(filepath_in, "rb");
	FILE *fp2 = fopen(filepath_out, "wb");

	//读取密钥密文长度
	char size[4];
	fread(size, 1, 4, fp1);
	memcpy(&len1, size, sizeof(unsigned));
	fread(buf1, 1, len1, fp1);
	printf("读取密钥\n");
	for (int i = 0; i <16; i++)
	{
		printf("%02X", buf1[i]);
	}
	printf("\n");
	//Rsa解密出密钥
	unsigned char key_s[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];

	RSA* rsa = RSA_new();
	BIO* file = NULL;
	file = BIO_new_file(filepath_pri, "rb");
	rsa = PEM_read_bio_RSAPrivateKey(file, NULL, NULL, NULL);
	EVP_PKEY * key = EVP_PKEY_new();
	EVP_PKEY_set1_RSA(key, rsa);
	RSA_free(rsa);
	BIO_free(file);
	EVP_PKEY_CTX * ctx = NULL;
	OpenSSL_add_all_ciphers();
	ctx = EVP_PKEY_CTX_new(key, NULL);
	EVP_PKEY_decrypt_init(ctx);
	EVP_PKEY_decrypt(ctx, buf2, (size_t *)&len2, buf1, len1);
	EVP_PKEY_CTX_free(ctx);
	EVP_PKEY_free(key);
	memcpy(key_s, buf2, EVP_MAX_KEY_LENGTH * sizeof(unsigned char));
	memcpy(iv, buf2 + EVP_MAX_KEY_LENGTH, EVP_MAX_IV_LENGTH * sizeof(unsigned char));

	//读取长度
	fread(size, 1, 4, fp1);
	memcpy(&len1, size, sizeof(unsigned));
	fread(buf1, 1, len1, fp1);
	printf("读取加密明文及摘要\n");
	for (int i = 0; i <len1; i++)
	{
		printf("%02X", buf1[i]);
	}
	printf("\n");
	//对称密钥解密
	EVP_CIPHER_CTX* ctx2 = EVP_CIPHER_CTX_new();
	OpenSSL_add_all_ciphers();
	EVP_DecryptInit_ex(ctx2, EVP_aes_256_cfb(), NULL, key_s, iv);
	int plainLen = 0, tmpLen;
	unsigned char *pBuf1 = buf1, *pBuf2 = buf2;
	while (1) {
		if ((int)(len1 - EVP_MAX_BLOCK_LENGTH) > 0)
		{
			EVP_DecryptUpdate(ctx2, pBuf2, &tmpLen, pBuf1, EVP_MAX_BLOCK_LENGTH);
			pBuf1 += EVP_MAX_BLOCK_LENGTH;
			pBuf2 += tmpLen;
			plainLen += tmpLen;
			len1 -= EVP_MAX_BLOCK_LENGTH;
		}
		else
		{
			EVP_DecryptUpdate(ctx2, pBuf2, &tmpLen, pBuf1, len1);
			plainLen += tmpLen;
			break;
		}
	}
	EVP_DecryptFinal_ex(ctx2, pBuf2, &tmpLen);
	plainLen += tmpLen;
	EVP_CIPHER_CTX_free(ctx2);
	memcpy(&sig_len, buf2, sizeof(unsigned));
	memcpy(sig, buf2 + 4, sig_len * sizeof(unsigned char));
	memcpy(&msg_len, buf2 + sig_len + 4, sizeof(unsigned));
	memcpy(msg, buf2 + sig_len + 8, msg_len * sizeof(unsigned char));
	fwrite(msg, 1, msg_len, fp2);
	printf("明文\n");
	for (int i = 0; i <msg_len; i++)
	{
		printf("%02X", msg[i]);
	}
	printf("\n");
	//计算hash值
	EVP_MD_CTX * md_ctx = EVP_MD_CTX_new();
	EVP_MD_CTX_init(md_ctx);
	EVP_DigestInit(md_ctx, EVP_sha1());
	EVP_DigestUpdate(md_ctx, msg, msg_len);
	EVP_DigestFinal(md_ctx, dig, &dig_len);
	EVP_MD_CTX_free(md_ctx);

	//验证签名
	EC_KEY * eckey = NULL;
	BIO *file2 = BIO_new_file(filepath_pub, "rb");
	eckey = PEM_read_bio_EC_PUBKEY(file2, NULL, NULL, NULL);
	int n = ECDSA_verify(0, dig, dig_len, sig, sig_len, eckey);
	BIO_free(file2);
	EC_KEY_free(eckey);

	free(msg);
	free(buf1);
	free(buf2);

	return 0;
}

void CreateECKeys(const char * filepath_pub, const char * filepath_pri)
{
	EC_KEY * key = EC_KEY_new();
	EC_builtin_curve * curves;
	EC_GROUP* group;
	int crv_len;
	unsigned int nid;
	crv_len = EC_get_builtin_curves(NULL, 0);
	curves = (EC_builtin_curve*)malloc(sizeof(EC_builtin_curve) * crv_len);
	EC_get_builtin_curves(curves, crv_len);
	srand((unsigned)time(NULL));
	nid = curves[rand() % crv_len].nid;
	group = EC_GROUP_new_by_curve_name(nid);
	EC_KEY_set_group(key, group);
	EC_KEY_generate_key(key);
	BIO* f1, *f2;
	f1 = BIO_new_file(filepath_pub, "wb");
	f2 = BIO_new_file(filepath_pri, "wb");
	PEM_write_bio_EC_PUBKEY(f1, key);
	PEM_write_bio_ECPrivateKey(f2, key, NULL, NULL, 0, NULL, NULL);
	BIO_flush(f1);
	BIO_flush(f2);
	BIO_free(f1);
	BIO_free(f2);
	EC_KEY_free(key);
	free(curves);
}

void CreateRSAKeys(const char * filepath_pub, const char * filepath_pri)
{
	RSA *r = RSA_new();
	BIGNUM * e = BN_new();
	BN_set_word(e, 65537);
	RSA_generate_key_ex(r, 1024, e, NULL);
	BN_free(e);
	BIO* f1, *f2;
	f1 = BIO_new_file(filepath_pub, "wb");
	f2 = BIO_new_file(filepath_pri, "wb");
	PEM_write_bio_RSAPublicKey(f1, r);
	PEM_write_bio_RSAPrivateKey(f2, r, NULL, NULL, 0, NULL, NULL);
	BIO_flush(f1);
	BIO_flush(f2);
	BIO_free(f1);
	BIO_free(f2);
	RSA_free(r);
}
