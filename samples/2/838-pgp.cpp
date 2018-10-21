// ConsoleApplication4.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"openssl/bn.h"
#include "string.h"
#include "openssl/sha.h"
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/objects.h>
#include <openssl/err.h>
#include <zlib.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>

int Get_ECC_key(EC_KEY * &key);

int main()
{
	unsigned char out[20];
	unsigned char in[1000];
	int n;
	unsigned char password[1000] = { 0 };
	unsigned char chuanshu[1000];
	printf("要加密的字符串:\n");
	scanf("%s", password);
	int i;
	for (i = 0; i < 1000; i++)
	{
		in[i] = password[i];
	}
	n = strlen((const char*)password);
	SHA1(in, n, out);
	printf("\nSHA1:");
	for (int i = 0; i<20; i++)
		printf("%x", out[i]);
	EC_KEY *key=EC_KEY_new();
	Get_ECC_key(key);
	int size;
	size = ECDSA_size(key);
	//printf("size %d \n", size);
	unsigned char *signature;
	signature = (unsigned char *)malloc(size);
	int ret;
	unsigned int  sig_len;
	ret = ECDSA_sign(0, out, 20, signature, &sig_len, key);
	int target;
	for (target = 0; target < n; target++)
	{
		chuanshu[target] = in[target];
	}
	for (target = n; target < sig_len + n; target++)
	{
		chuanshu[target] = signature[target - n];
	}
	printf("\nECC加密；");
	for (target = 0; target < n + sig_len; target++)
	{
		printf("%x",chuanshu[target]);
	}
	int n1;
	n1 = n + sig_len;
	unsigned char signature1[1000];
	unsigned int  sig_len1;
	compress(signature1, (uLongf *)&sig_len1, chuanshu, (uLongf)n1);
	printf("\n压缩后：");
	for (target = 0; target < sig_len1; target++)
	{
		printf("%x", signature1[target]);
	}
	for (target = 0; target < 1000 ; target++)
	{
		if (target < sig_len1)
			chuanshu[target] = signature1[target];
		else chuanshu[target] = 0;
	}
	int kuozhanshu;
	for (kuozhanshu = 0;; kuozhanshu++)
	{
		if ((kuozhanshu + sig_len1) % 16 == 0)
			break;
	}
	for (target = 0; target < kuozhanshu; target++)
	{
		chuanshu[target + sig_len1] = 0x0;
	}
	int n2;
	n2 = kuozhanshu + sig_len1;
	//  
	unsigned char userKey[16] = {1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf ,0};
	AES_KEY key2;
	AES_set_encrypt_key(userKey, 128 , &key2);
	unsigned char chuanshu1[1000];
	unsigned char ivec[16] = { 0 };
	AES_cbc_encrypt(chuanshu, chuanshu1, n2, &key2, ivec, AES_ENCRYPT);
	printf("\nAES加密： ");
	for (target = 0; target < n2; target++)
	{
		printf("%x", chuanshu1[target]);
	}
	RSA *r;
	int bits = 512;
	unsigned long e = RSA_3;
	BIGNUM *bne;
	r = RSA_generate_key(bits, e, NULL, NULL);
	bne = BN_new();
	ret = BN_set_word(bne, e);
	r = RSA_new();
	ret = RSA_generate_key_ex(r, bits, bne, NULL);
	int flen;
	flen = RSA_size(r);
	unsigned char to[1000];
	RSA_public_encrypt(flen, userKey, to, r, RSA_NO_PADDING);
	for (target = 0; target < flen; target++)
	{
		chuanshu1[target + n2] = to[target];
	}
	int n3;
	n3 = n2 + flen;
	printf("\nRSA加密： ");
	for (target = 0; target < n3; target++)
	{
		printf("%x", chuanshu1[target]);
	}
	//解密
	unsigned char chuanshu2[1000];
	for (target = 0; target < flen; target++)
	{
		 chuanshu2[target] = chuanshu1[target + n2];
		 chuanshu1[target + n2] = 0x0;

	}
	printf("\n\n\n\nrsa加密的密钥 ：");
	for (target = 0; target < flen; target++)
	{
		printf("%x", chuanshu2[target]);
	}
	RSA_private_decrypt(flen, chuanshu2, userKey, r, RSA_NO_PADDING);
	printf("\nAES解密：");
	AES_set_encrypt_key(userKey, 128, &key2);
	unsigned char chuanshu3[1000];
	AES_cbc_encrypt(chuanshu1, chuanshu3, n2, &key2, ivec, AES_DECRYPT);
	for (target = 0; target < sig_len1; target++)
	{
		 chuanshu3[target] = signature1[target];
	}
	for (target = 0; target < sig_len1; target++)
	{
		printf("%x", chuanshu3[target]);
	}
	printf("\n解压缩 "); 
	unsigned char signature2[1000];
	uncompress(signature2, (uLongf *)&sig_len1, chuanshu3, (uLongf)n1);
	for (target = 0; target < n + sig_len; target++)
	{
		printf("%x", signature2[target]);
	}
	for (target = 0; target < kuozhanshu; target++)
	{
		signature2[target + sig_len1] = 0;
	}
	printf("\nECC解密：  ");
	int yanqian;
	unsigned char chuanshu4[1000] = { 0 };
	for (target = 0; target < n; target++)
	{
		chuanshu4[target] = signature2[target];
		printf("%x", chuanshu4[target]);
	}
	unsigned char out1[20];
	SHA1(chuanshu4, n, out1);
	printf("\nSHA1：");
	for (int i = 0; i<20; i++)
		printf("%x", out1[i]);
	unsigned char signature3[1000];
	for (target = 0; target <  sig_len; target++)
	{
		signature3[target] = signature2[target + n];
	}
	yanqian = ECDSA_verify(0, out1, 20, signature, sig_len, key);
	if (yanqian = 1)
	printf("\nresult： ");
	printf("%s", chuanshu4);


    return 0;
}
int Get_ECC_key(EC_KEY * &key)
{
	EC_GROUP *group;
	int ret, nid, crv_len;
	EC_builtin_curve *curves;
	if (key)
		EC_KEY_free(key);
	key = EC_KEY_new();
	if (key == NULL)
	{
		printf("EC_KEY_new err!\n");
		return -1;
	}
	crv_len = EC_get_builtin_curves(NULL, 0);
	curves = (EC_builtin_curve *)malloc(sizeof(EC_builtin_curve) * crv_len);
	EC_get_builtin_curves(curves, crv_len);
	nid = curves[25].nid;
	group = EC_GROUP_new_by_curve_name(nid);
	if (group == NULL)
	{
		printf("EC_GROUP_new_by_curve_name err!\n");
		return -1;
	}
	ret = EC_KEY_set_group(key, group);
	if (ret != 1)
	{
		printf("EC_KEY_set_group err.\n");
		return -1;
	}
	ret = EC_KEY_generate_key(key);
	if (ret != 1)
	{
		printf("EC_KEY_generate_key err.\n");
		return -1;
	}
	ret = EC_KEY_check_key(key);
	if (ret != 1)
	{
		printf("check key err.\n");
		return -1;
	}
	free(curves);
	return 1;
}