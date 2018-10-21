//
//  main.c
//  PGP
//
//  Created by 林斌 on 2018/9/12.
//  Copyright © 2018年 林斌. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ec.h"
#include "sha.h"
#include "ec.h"
#include "ecdsa.h"
#include "rsa.h"
#include "aes.h"
#include "evp.h"
#include "bio.h"
#include "pem.h"

#define maxdatabuff 1024
#define FILEMAX 1024*1024*10 //10MB
#define true 1
#define false 0
#define T 10000

void digestsha1(const unsigned char *msg,int msglen,unsigned char*digest,unsigned int *digestlen){
    
    EVP_MD_CTX *ctx=EVP_MD_CTX_new();
    EVP_MD_CTX_init(ctx);
    EVP_DigestInit(ctx, EVP_sha1());
    EVP_DigestUpdate(ctx, msg, msglen);
    EVP_DigestFinal(ctx, digest, digestlen);
    EVP_MD_CTX_free(ctx);
    
    
}

void create_key_EC(const char *pubkeypath,const char *prvkeypath){
    EC_KEY *key=EC_KEY_new();
    EC_builtin_curve *curves;
    EC_GROUP *group;
    unsigned long cur_len;
    unsigned int nid;
    cur_len=EC_get_builtin_curves(NULL, 0);
    curves=(EC_builtin_curve *)malloc(sizeof(EC_builtin_curve)*cur_len);
    EC_get_builtin_curves(curves, cur_len);
    srand((unsigned)time(NULL));
    nid=curves[rand()%cur_len].nid;
    group=EC_GROUP_new_by_curve_name(nid);
    EC_KEY_set_group(key, group);
    EC_KEY_generate_key(key);
    BIO *pubfile,*prifile;
    pubfile=BIO_new_file(pubkeypath, "wb");
    prifile=BIO_new_file(prvkeypath, "wb");
    PEM_write_bio_EC_PUBKEY(pubfile,key );
    PEM_write_bio_ECPrivateKey(prifile, key, NULL, NULL, 0, NULL, NULL);
    BIO_flush(pubfile);
    BIO_flush(prifile);
    BIO_free(pubfile);
    BIO_free(prifile);
    EC_KEY_free(key);
    free(curves);
}

void ECC_signature(const char *prvkeypath,const unsigned char *digest,unsigned int dig_len,unsigned char *signature,unsigned int *sig_len){
    EC_KEY *key=EC_KEY_new();
    BIO *file=NULL;
    file=BIO_new_file(prvkeypath, "rb");
    key=PEM_read_bio_ECPrivateKey(file, NULL, NULL, NULL);
    ECDSA_sign(0, digest, dig_len, signature, sig_len, key);
    BIO_free(file);
    EC_KEY_free(key);
}

int Verify_ECC(const char *pubkeypath,const unsigned char *digest,unsigned int dig_len,unsigned char *signature,unsigned int sig_len){
    int ret;
    EC_KEY *key=NULL;
    BIO *file=NULL;
    file =BIO_new_file(pubkeypath, "rb");
    key=PEM_read_bio_EC_PUBKEY(file, NULL, NULL, NULL);
    ret=ECDSA_verify(0, digest, dig_len, signature, sig_len, key);
    BIO_free(file);
    EC_KEY_free(key);
    return ret;
}

void ase_encrypt(unsigned char *key,unsigned char *msg,int msg_len,unsigned char *ret,int *ret_len){
    EVP_CIPHER_CTX *ctx=EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    unsigned char out[16];
    EVP_EncryptInit(ctx, EVP_aes_256_ctr(), key, out);//使用ctr模式aes加密
    EVP_EncryptUpdate(ctx, ret, ret_len, msg, msg_len);
    EVP_EncryptFinal(ctx, ret, ret_len);
    EVP_CIPHER_CTX_free(ctx);
}

void create_key_rsa(const char *pubfile,const char *prifile){
    RSA *rsa=RSA_new();
    BIGNUM *e=BN_new();
    unsigned long w=65537;
    BN_set_word(e, w);
    RSA_generate_key_ex(rsa, 1024, e, NULL);
    BIO *f1,*f2;
    f1=BIO_new_file(pubfile, "wb");
    f2=BIO_new_file(prifile, "wb");
    PEM_write_bio_RSAPublicKey(f1, rsa);
    PEM_write_bio_RSAPrivateKey(f2, rsa, NULL, NULL, 0, NULL, NULL);
    BIO_flush(f1);
    BIO_flush(f2);
    BIO_free(f1);
    BIO_free(f2);
    RSA_free(rsa);
    BN_free(e);
    
}

void RSA_encrypt(const char *pubfile,unsigned char *msg,int msg_len,unsigned char *ret,unsigned ret_len){
    RSA *rsa=RSA_new();
    BIO *file=NULL;
    int rett;
    file=BIO_new_file(pubfile, "rb");
    rsa=PEM_read_bio_RSAPublicKey(file, NULL, NULL, NULL);
    EVP_PKEY *key=EVP_PKEY_new();
    rett=EVP_PKEY_set1_RSA(key, rsa);
    RSA_free(rsa);
    BIO_free(file);
    EVP_PKEY_CTX *ctx=EVP_PKEY_CTX_new(key, NULL);
    OpenSSL_add_all_ciphers();
    rett=EVP_PKEY_encrypt_init(ctx);
    //EVP_PKEY_encrypt(ctx, msg, msg_len, ret, ret_len);
    rett=EVP_PKEY_encrypt(ctx, ret, &ret_len, msg, msg_len);
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(key);
    
}

void Decrypt_RSA(const char * filepath_pri, unsigned char * msg, int msg_len, unsigned char * r, unsigned r_len)
{
    
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
    EVP_PKEY_decrypt(ctx, r, &r_len, msg, msg_len);
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(key);
    
}

void allEncrypt(char * password, const char* filepath_pub, const char* filepath_pri, const char* filepath_in, const char* filepath_out)
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
    unsigned dig_len, sig_len;
    
    //hash
    digestsha1(msg, msg_len, dig, &dig_len);
    
    //签名
    ECC_signature(filepath_pri, dig, dig_len, sig, &sig_len);
    
    //将签名与文件信息拼接在一起
    int allLen = sig_len + msg_len + 8;
    memcpy(buf1, &sig_len, sizeof(unsigned int));
    memcpy(buf1 + 4, sig, sig_len * sizeof(unsigned char));
    memcpy(buf1 + sig_len + 4, &msg_len, sizeof(unsigned));
    memcpy(buf1 + sig_len + 8, msg, msg_len * sizeof(char));
    
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
    while(1){
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
    
    //RSA非对称加密密钥
    memcpy(buf1, key_s, EVP_MAX_KEY_LENGTH * sizeof(unsigned char));
    memcpy(buf1 + EVP_MAX_KEY_LENGTH, iv, EVP_MAX_IV_LENGTH * sizeof(unsigned char));
    unsigned keyLen;
    RSA* rsa = RSA_new();
    BIO *file = BIO_new_file(filepath_pub, "rb");
    rsa = PEM_read_bio_RSAPublicKey(file, NULL, NULL, NULL);
    EVP_PKEY * rsaKey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(rsaKey, rsa);
    RSA_free(rsa);
    BIO_free(file);
    EVP_PKEY_CTX * ctx2 = EVP_PKEY_CTX_new(rsaKey, NULL);
    OpenSSL_add_all_ciphers();
    EVP_PKEY_encrypt_init(ctx2);
    EVP_PKEY_encrypt(ctx2, buf3, &keyLen, buf1, EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH);
    EVP_PKEY_CTX_free(ctx2);
    EVP_PKEY_free(rsaKey);
    
    //将加密后的密钥与之前的结果拼接在一起
    memcpy(output, &keyLen, sizeof(unsigned));
    memcpy(output + 4, buf3, keyLen * sizeof(unsigned char));
    memcpy(output + 4 + keyLen, &cipLen, sizeof(int));
    memcpy(output + 4 + keyLen + 4, buf2, cipLen * sizeof(unsigned char));
    
    //写文件
    fwrite(output, 1, keyLen+cipLen+8, fp2);
    
    EVP_CIPHER_CTX_free(ctx);
    free(msg);
    free(output);
    free(buf1);
    free(buf2);
    fclose(fp1);
    fclose(fp2);
}


bool allDecrypt(const char * filepath_pub, const char * filepath_pri, const char * filepath_in, const char * filepath_out){
    unsigned char dig[1024], sig[1024];
    unsigned len1, len2;
    unsigned sig_len, msg_len, dig_len;
    unsigned char *msg = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char)),
    *buf1 = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char)),
    *buf2 = (unsigned char*)malloc(FILEMAX * sizeof(unsigned char));
    
    FILE *fp1 = fopen(filepath_in, "rb");
    FILE *fp2 = fopen(filepath_out, "wb");
    
    //读取aes密钥密文长度
    char size[4];
    fread(size, 1, 4, fp1);
    memcpy(&len1, size, sizeof(unsigned));
    fread(buf1, 1, len1, fp1);
    
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
    
    //读取密文长度
    fread(size, 1, 4, fp1);
    memcpy(&len1, size, sizeof(unsigned));
    fread(buf1, 1, len1, fp1);
    //对称密钥解密
    EVP_CIPHER_CTX* ctx2 = EVP_CIPHER_CTX_new();
    OpenSSL_add_all_ciphers();
    EVP_DecryptInit_ex(ctx2, EVP_aes_256_cfb(), NULL, key_s, iv);
    int plainLen = 0, tmpLen;
    unsigned char *pBuf1 = buf1, *pBuf2 = buf2;
    while(1){
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
    
    //hash
    digestsha1(msg, msg_len, dig, &dig_len);
    //验证签名
    int n=Verify_ECC(filepath_pub, dig, dig_len, sig, sig_len);
    free(msg);
    free(buf1);
    free(buf2);
    
    return n;
}


int main() {
    
    const char *writin="/Users/bin/Desktop/SPNnetwork/PGP/PGP/x.txt";
    const char *writout="/Users/bin/Desktop/SPNnetwork/PGP/PGP/y.txt";
    const char *writcheck="/Users/bin/Desktop/SPNnetwork/PGP/PGP/z.txt";
    unsigned char *password="1234567";
    unsigned char *ret;
    unsigned ret_len;
    unsigned char *msg;
    unsigned msg_len;
    bool correct;
    const char *pubfileEC="/Users/bin/Desktop/SPNnetwork/PGP/PGP/pubfileEC.pem";
    const char *prifileEC="/Users/bin/Desktop/SPNnetwork/PGP/PGP/prifileEC.pem";
    const char *pubfileRSA="/Users/bin/Desktop/SPNnetwork/PGP/PGP/pubfileRSA.pem";
    const char *prifileRSA="/Users/bin/Desktop/SPNnetwork/PGP/PGP/prifileRSA.pem";
    //create_key_EC(pubfileEC, prifileEC);
    //create_key_rsa(pubfileRSA, prifileRSA);
    allEncrypt(password, pubfileRSA, prifileEC, writin, writout);
    correct=allDecrypt(pubfileEC, prifileRSA, writout,writcheck );
    //RSA_encrypt(pubfileRSA, password, 7, ret, ret_len);
    //Decrypt_RSA(prifileRSA, msg, msg_len, ret, ret_len);
    
    return 0;
}

