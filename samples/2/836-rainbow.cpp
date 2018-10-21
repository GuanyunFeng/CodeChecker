#include <stdio.h>
#include "time.h"
#include <stdlib.h>
#include "md5.h"
#include "evp.h"
#include "string.h"
typedef struct node1{
    unsigned char s[6];
    struct node1 *next;
}node;

char text[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

#define RCount 36
#define inputlen 6
#define mdlen 16
#define chainlength 100


int cmp(unsigned char *a,unsigned char *b){
    for (int i=0; i<6; i++) {
        if (a[i]!=b[i]) {
            return -1;
        }
    }
    return 0;
}

void create_randtext(char s[6]){
    
    char c;
    for (int i=0; i<6; i++) {
        c=text[rand()%36];
        *(s+i)=c;
    }
}

void hash_md5(const char *s,unsigned char md5[16]){
    unsigned long len;
    len=6;
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, s, len);
    MD5_Final(md5, &ctx);

}

void reductionfunction(unsigned char *encrypt, unsigned char *decrypt, int k, int length)
{
    int i;
    unsigned char x, y;
    k = k%RCount;
    for (i = 0; i<length; i++) {
        x = decrypt[i] ^ decrypt[(i + 1) & 0xf] ^ decrypt[(i + 2) & 0xf] ^ decrypt[(i + 3) & 0xf];
        y = decrypt[k & 0xf] + decrypt[(k - 1) & 0xf] + decrypt[(k - 2) & 0xf] + decrypt[(k - 3) & 0xf];
        encrypt[i] = text[(x + y) % 36];
        k = k + k + k + 1;
    }
    //encrypt[length] = '\0';
}


int  lookup(unsigned char hash[16],unsigned char result[6])
{
    FILE *fp;
    fp=fopen("/Users/bin/Desktop/SPNnetwork/rainbow/rainbow/rainbow.txt", "r");
    
    int i,k;
    int ret;
    char c;
    //node *p=(node *)malloc(10000*sizeof(node));
    node p[100000];
    int fk=0,fi=0;
    unsigned char temp[chainlength][6];
    unsigned char outtemp[6];
    unsigned char ha[16];
    
    fseek(fp, SEEK_SET, 0);
    
    for (i=0; i<100000; i++) {
        
        fscanf(fp, "%6c",p[i].s);
        //p[i].s[6]='\0';
        //fread(p[i].s, 6, 1, fp);
        node *q;
        q=(node *)malloc(sizeof(node));
        c=fgetc(fp);
        fscanf(fp, "%6c",q->s);
        //q.s[6]='\0';
        c=getc(fp);
        q->next=NULL;
        p[i].next=q;
    }
    
    for (k=0; k<chainlength; k++) {
        if (k==0)
        {
            reductionfunction(temp[k], hash, 0, 6);
        }
        else {
            for (int z=0; z<6; z++) {
                temp[k][z]=temp[k-1][z];
            }
            hash_md5(temp[k], ha);
            reductionfunction(temp[k], ha, 0, 6);
        }
    }
    
    for (k=0; k<chainlength; k++) {
        for(i=0;i<100000;i++){
            
            //reductionfunction(temp[k],hash , 0, 6);
            ret=cmp(temp[k], (p[i].next)->s);
            //ret=strcmp(temp, *(p[i].next)->s);
            if (ret==0){
                fi=i;
                fk=k;
                goto LOOP;
            }
        }
    }
    printf("can't find the hash\n");
    return 0;
    
LOOP:if(fk==chainlength-1){
    for (int i=0; i<6; i++) {
        outtemp[i]=p[fi].s[i];
    }
}else
    for(k=0;k<chainlength-fk-1;k++){
        if (k==0) {
            for (int i=0; i<6; i++)
            {
                outtemp[i]=p[fi].s[i];
            }
        }
    hash_md5(outtemp, ha);
    reductionfunction(outtemp, ha, 0, 6);
    
}
    printf("%s",outtemp);
    return 0;
}

int main(int argc, const char * argv[]) {
    FILE *fpx;
    fpx=fopen("/Users/bin/Desktop/SPNnetwork/rainbow/rainbow/rainbow.txt", "r");
    
    srand((unsigned int)time(NULL));
    
    char s[inputlen];
    char ss[inputlen];
    unsigned char md[mdlen];
    unsigned char reducemd[inputlen];
    
    scanf("%s",s);
    for (int i=0; i<6; i++) {
        ss[i]=s[i];
    }
    for (int i=0; i<10; i++) {
        hash_md5(s, md);
        reductionfunction(s,md,0,6);
    }
    
    hash_md5(s,md);
    printf("%s\n",s);
    printf("%s\n",md);
    lookup(md, reducemd);

    fclose(fpx);
    return 0;
}
