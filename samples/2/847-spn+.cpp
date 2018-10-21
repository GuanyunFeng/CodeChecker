#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <ctime>
#define nturn 10//轮数
#define len 16//单位长度
#define zu 8//组数
using namespace std;

int key[208];
int s[len*len];
int sn[len*len];

int k[nturn+2][len*zu];
int w[nturn+2][len*zu];
int u[nturn+1][len*zu];
int v[nturn+1][len*zu];

int bin2hex(int *a, int n)
{
	int b = a[n]*128+a[n+1]*64+a[n+2]*32+a[n+3]*16+a[n+4]*8+a[n+5]*4+a[n+6]*2+a[n+7];
	return b;
}

void hex2bin(int *a, int n, int m)
{
	int t = m;
	int i;
	for (i=zu-1;i>0;i--)
	{
	    a[n+i] = t%2;
	    t/=2;
	}
	a[n] = t%2;
}


void sre(int *a, int *b)
{
	memset(b, 0, sizeof(b));
	for (int i = 0; i < len; i++)
		hex2bin(b, zu*i, s[bin2hex(a, i*zu)]);
}

void pre(int *a, int *b)
{
    int i,j;
	memset(b, 0, sizeof(b));
	for (i = 0; i < 16; i++)
		for (j = 0; j < 8; j++)
			b[j*16+i] = a[8*i+j];
}

void scopy(int *a, int *b)
{
	memset(b,0,sizeof(b));
	for (int i = 0; i< len*zu; i++)
		b[i] = a[i];
}

//XOR
void sxor(int *a, int *b, int *c)
{
	for (int i = 0; i< len*zu; i++)
		c[i] = a[i]^b[i];
}

int main()
{
    int x[len*zu];
	memset(x, 0, sizeof(x));
	int y[len*zu];
	memset(y, 0, sizeof(y));//初始化xy


	freopen("D:\\sbox.txt", "r", stdin);//读取S盒
	for (int i = 0; i < len*len; i++)
		scanf("%d", &s[i]);
	for (int i = 0; i < len*len; i++)
		scanf("%d", &sn[i]);



	char c;//设置明文
	for (int i = 0; i < 128; i++)
	{
		x[i] = 0;
	}

	FILE *fp=fopen("d:\\897.txt","wb");

	srand((unsigned)(time(NULL)));
	for (int i = 0; i < 208; i++)//随机设置密钥
	{
		key[i] = rand()%2;
	}


	for (int i = 1; i <= nturn+1; i++)//得到轮密钥
		for (int j = 0; j < zu*len; j++)
			k[i][j] = key[zu*i-zu+j];
	scopy(x, w[0]);

	for (int i=1; i<= 1000000;i++)//循环，输出足够多的数据
    {
        for (int i = 1; i <= nturn; i++)
        {
            sxor(w[i-1], k[i], u[i]);
            sre(u[i], v[i]);
            pre(v[i], w[i]);
        }
        sxor(k[nturn+1], v[nturn], y);

        for (int i = 0; i < 16; i+=2)
        {
            short aaa=(short) bin2hex(y,i*8);
            aaa=(aaa<<8)|((short) bin2hex(y,i*8+8));
            fwrite(&aaa,sizeof(short),1,fp);
        }
        sxor(y, x, w[0]);
    }
    fclose(fp);
	return 0;
}
