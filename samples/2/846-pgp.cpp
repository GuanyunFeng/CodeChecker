// pgp1.cpp: 定义控制台应用程序的入口点。
//



#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/objects.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/comp.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <zlib.h>


int padding(unsigned char* z, unsigned long len)
{
	unsigned char* temp;
	unsigned long plen;
	int i;
	int pad;
	pad = 16 - len % 16;
	if ((len % 16) != 0)
	{
		temp = (unsigned char*)malloc(sizeof(unsigned char)*(pad + 16));
		for (i = 0; i < pad + 15; i++)
		{
			temp[i] = '0';
		}
		temp[pad + 15] = len;
		for (i = 0; i < pad + 16; i++)
		{
			z[len + i] = temp[i];
		}
		plen = len + pad + 16;
	}
	else
	{
		for (i = 0; i < 15; i++)
		{
			z[len + i] = temp[i];
		}
		z[len + 15] = len;
		plen = len + 16;
	}
	return plen;
}
int main()
{
	unsigned char input[100];
	unsigned char outmd[20];//sha1摘要值,注意这里的字符个数为20
	unsigned char string[200];//连接后的数据
	unsigned long xlen, plen = 0;
	SHA_CTX stx;
	unsigned char buffer[200];
	char filename[32];
	int len = 0;
	FILE * fp = NULL;
	EC_KEY *key;
	EC_GROUP *group;
	unsigned int ret, nid, size, i, sig_len;
	unsigned char *signature;
	BIO *berr;
	EC_builtin_curve *curves;
	int crv_len, n;
	memset(outmd, 0, sizeof(outmd));
	memset(filename, 0, sizeof(filename));
	memset(buffer, 0, sizeof(buffer));
	printf("请输入文件名:");
	scanf("%s", filename);
	getchar();
	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("Can't open file\n");
		getchar();
		return 0;
	}
	SHA1_Init(&stx);
	while ((len = fread(buffer, 1, 100, fp))>0)
	{
		printf("该加密文件的信息为：");
		xlen = len;
		for (i = 0;i < len; i++)
		{
			input[i] = buffer[i];
			printf("%02X", input[i]);
		}
		printf("\n");
		SHA1_Update(&stx, buffer, len);
	}
	memset(buffer, 0, sizeof(buffer));
	SHA1_Final(outmd, &stx);
	printf("使用SHA1加密后的摘要为:");
	for (i = 0;i < 20; i++)
	{
		printf("%02X", outmd[i]);
	}
	printf("\n");
	key = EC_KEY_new();//构造EC_KEY数据结构
	if (key == NULL)
	{
		printf("EC_KEY_new error!\n");
		return -1;
	}
	/* 构造 EC_KEY 数据结构 */
	key = EC_KEY_new();
	if (key == NULL)
	{
		printf("EC_KEY_new err!\n");
		return -1;
	}
	/* 获取实现的椭圆曲线个数 */
	crv_len = EC_get_builtin_curves(NULL, 0);
	curves = (EC_builtin_curve *)malloc(sizeof(EC_builtin_curve) * crv_len);
	/* 获取椭圆曲线列表 */
	EC_get_builtin_curves(curves, crv_len);
	/* 选取一种椭圆曲线 */
	nid = curves[25].nid;
	/* 根据选择的椭圆曲线生成密钥参数 group */
	group = EC_GROUP_new_by_curve_name(nid);
	if (group == NULL)
	{
		printf("EC_GROUP_new_by_curve_name err!\n");
		return -1;
	}
	/* 设置密钥参数 */
	ret = EC_KEY_set_group(key, group);
	if (ret != 1)
	{
		printf("EC_KEY_set_group err.\n");
		return -1;
	}
	/* 生成密钥 */
	ret = EC_KEY_generate_key(key);
	if (ret != 1)
	{
		printf("EC_KEY_generate_key err.\n");
		return -1;
	}
	/* 检查密钥 */
	ret = EC_KEY_check_key(key);
	if (ret != 1)
	{
		printf("check key err.\n");
		return -1;
	}
	/* 获取密钥大小 */
	size = ECDSA_size(key);
	//for (i = 0;i<20;i++)
	//	memset(&outmd[i], i + 1, 1);
	signature = (unsigned char*)malloc(size);
	ERR_load_crypto_strings();
	berr = BIO_new(BIO_s_file());
	//BIO_set_fp(berr,stdout,BIO_NOCLOSE);
	/* 签名数据，outmd 中的数据是 sha1 摘要结果 */
	ret = ECDSA_sign(0, outmd, 20, signature, &sig_len, key);
	if (ret != 1)
	{
		ERR_print_errors(berr);
		printf("sign err!\n");
		return -1;
	}
	/* 验证签名 */
	ret = ECDSA_verify(0, outmd, 20, signature, sig_len, key);
	if (ret != 1)
	{
		ERR_print_errors(berr);
		printf("ECDSA_verify err!\n");
		getchar();
		return -1;
	}
	printf("ECC椭圆曲线加密后的数据长度为：%d\n", sig_len);
	printf("经过ECC椭圆曲线加密后的信息为：");
	for (i = 0; i < sig_len; i++)
	{
		printf("%02X", signature[i]);
	}
	printf("\n");

	for (i = 0; i < sig_len; i++)
	{
		string[i] = signature[i];
	}
	for (i = 0; i < xlen; i++)
	{
		string[sig_len + i] = input[i];
	}
	printf("连接后的信息为：");
	for (i = 0; i < (xlen + sig_len); i++)
	{
		printf("%02X", string[i]);
	}
	printf("\n");
	unsigned char z[1024];//压缩后的数据
	unsigned char expand[1024];//解压后的数据
	unsigned long slen, zlen;//原长度，压缩后的长度
	slen = xlen + sig_len;
	printf("原串长度为:%ld\n", slen);
	//压缩
	compress(z, &zlen, string, slen);
	printf("压缩后的信息为：");
	for (i = 0; i < zlen; i++)
	{
		printf("%02X", z[i]);
	}
	printf("\n");
	printf("压缩后信息长度为:%ld\n", zlen);
	/*测试解压
	unsigned long exlen,pplen = zlen;
	uncompress(expand, &exlen, z, pplen);
	printf("解压出的长度为:%ld\n", exlen);
	//printf("%s", expand);
	printf("解压出的信息为:");
	for (i = 0; i < exlen; ++i)
	{
	printf("%02X", expand[i]);
	}
	printf("\n");
	*/
	plen = padding(z, zlen);
	printf("填充后的长度为：%ld字节\n", plen);
	printf("补零填充后的信息为：");
	for (i = 0; i < plen; i++)
	{
		printf("%02X", z[i]);
	}
	printf("\n");
	/*AES加密*/
	unsigned char userkey[16] = "1234567890abcde";//size为16个字节
	AES_KEY Akey;
	unsigned char* encrypt;
	unsigned char* plain;
	encrypt = (unsigned char*)malloc(sizeof(unsigned char)*(plen));
	plain = (unsigned char*)malloc(sizeof(unsigned char)*(plen));
	AES_set_encrypt_key((const unsigned char*)userkey, 128, &Akey);//设置加密密钥及其长度
	printf("AES密钥为：");
	for (i = 0; i < 16; i++)
	{
		printf("%c", userkey[i]);
	}
	printf("\n");
	for (i = 0;i<(plen / 16);i++)
	{
		AES_encrypt(z + i * 16, encrypt + i * 16, &Akey);
	}
	printf("经过AES加密后的信息为：");
	for (i = 0; i < plen; i++)
	{
		printf("%02X", encrypt[i]);
	}
	printf("\n");
	/*RSA加密*/
	RSA* pRSAPublicKey = RSA_generate_key(1024, 65537, NULL, NULL);
	int keySize = RSA_size(pRSAPublicKey);
	unsigned char rsa_en[128];
	unsigned char rsa_dn[128];
	ret = RSA_public_encrypt(plen, encrypt, rsa_en, pRSAPublicKey, RSA_PKCS1_PADDING);
	printf("经过RSA公钥加密后的信息为：");
	for (i = 0; i < plen; i++)
	{
		printf("%02X", rsa_en[i]);
	}
	printf("\n");
	/*写入文件*/
	printf("将加密信息写入文件cipher.txt中...\n");
	FILE *cipher;
	unsigned char out[128];
	if ((cipher = fopen("cipher.txt", "w")) == NULL)
	{
		fprintf(stderr, "Can not open cipher.txt file.\n");
		return 0;
	}
	for (i = 0;i<plen;i++)
	{
		out[i] = rsa_en[i];
	}
	fwrite(out, sizeof(unsigned char), plen, cipher);
	fclose(cipher);
	printf("\n");
	printf("进行文件传输...\n\n");
	printf("——————————————————当对方接受到文件后——————————————————\n");
	FILE *plainer;
	plainer = fopen("cipher.txt", "r");
	if (plainer == NULL)
	{
		printf("Can't open file\n");
		getchar();
		return 0;
	}
	while ((fread(buffer, 1, plen, plainer))>0)
	{
		printf("打开文件，接收到文件的信息为：");
		for (i = 0;i < plen; i++)
		{

			printf("%02X", buffer[i]);
		}
		printf("\n");
	}
	/*RSA解密*/
	ret = RSA_private_decrypt(128, rsa_en, rsa_dn, pRSAPublicKey, RSA_PKCS1_PADDING);
	printf("经过RSA私钥解密后的信息为：");
	for (i = 0; i < plen; i++)
	{
		printf("%02X", rsa_dn[i]);
	}
	printf("\n");
	RSA_free(pRSAPublicKey);
	/*AES解密*/
	AES_set_decrypt_key((unsigned char*)userkey, 128, &Akey);//设置解密密钥及其长度
	for (i = 0;i<(plen / 16);i++)
	{
		AES_decrypt(encrypt + i * 16, plain + i * 16, &Akey);
	}
	printf("经过AES解密后的信息为：");
	for (i = 0; i < plen; i++)
	{
		printf("%02X", plain[i]);
	}
	printf("\n");
	/*去掉为了AES加密而补充的部分*/
	unsigned int  pplen = plain[plen - 1];
	printf("去除填充后的信息为：");
	for (i = 0; i < pplen; i++)
	{
		printf("%02X", plain[i]);
	}
	printf("\n");
	//解压缩
	unsigned long exlen;
	uncompress(expand, &exlen, plain, pplen);
	printf("解压出的长度为:%ld\n", exlen);
	//printf("%s", expand);
	printf("解压出的信息为:");
	for (i = 0; i < exlen; i++)
	{
		printf("%02X", expand[i]);
	}
	printf("\n");
	int ylen;
	ylen = exlen - sig_len;
	unsigned char* output;//明文
	output = (unsigned char*)malloc(sizeof(unsigned char)*ylen);
	unsigned char* outmd1;//ECC加密后信息
	outmd1 = (unsigned char*)malloc(sizeof(unsigned char)*sig_len);
	for (i = 0;i < sig_len;i++)
	{
		outmd1[i] = expand[i];
	}
	printf("ECC椭圆曲线签名后的信息为:");
	for (i = 0; i < sig_len;i++)
	{
		printf("%02X", outmd1[i]);
	}
	printf("\n");
	printf("明文为:");
	for (i = 0; i < ylen; i++)
	{
		output[i] = expand[i + sig_len];
		printf("%c", output[i]);
	}
	printf("\n");
	/*完整性验证*/
	printf("进行完整性验证...\n");
	unsigned char digest[20];
	SHA1(output, ylen, digest);
	printf("验证得到明文摘要为：");
	for (i = 0; i < 20; i++)
	{
		printf("%02X", digest[i]);
	}
	printf("\n");
	/* 验证签名 */
	ret = ECDSA_verify(0, digest, 20, outmd1, sig_len, key);
	if (ret != 1)
	{
		ERR_print_errors(berr);
		printf("ECDSA_verify err!\n");
		getchar();
		return -1;
	}
	else
		printf("验证成功，文件完整！");
	BIO_free(berr);
	EC_KEY_free(key);
	free(signature);
	free(curves);
	getchar();
	return 0;
}


