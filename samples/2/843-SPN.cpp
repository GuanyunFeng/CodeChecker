#include<stdio.h>
int sbox(int a, int b, int c, int d);
int SBoxofinverse(int v);


int main()
{
	int K[32] = { 0,0,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,1,1 };
	int w[16] = { 0,0,1,0,0,1,1,0,1,0,1,1,0,1,1,1 }, u[16],v[16],V,U;
	int r = 0,i=0,j=0;
	for (i = 0; i<16; i = i + 4)
		printf("%d%d%d%d  ", w[i], w[i + 1], w[i + 2], w[i + 3]);
	printf("\n");
	for (r = 0; r < 3; r++)
	{
		for (i = 0; i < 16; i++)
			u[i] = w[i] ^ K[i + r * 4];//轮密钥异或
		for (i = 0; i<16; i = i + 4)
			printf("%d%d%d%d  ", u[i], u[i + 1], u[i + 2], u[i + 3]);
		printf("\n");
		for (i = 0; i < 4; i++)//s盒代换
		{
			V = sbox(u[4 * i + 0], u[4 * i + 1], u[4 * i + 2], u[4 * i + 3]);
			for (j = 3; j >= 0; j--)
			{
				v[j + 4 * i] = V % 2;
				V = V / 2;
			}
		}
		for (i = 0; i<16; i = i + 4)
			printf("%d%d%d%d  ", v[i], v[i + 1], v[i + 2], v[i + 3]);
		printf("\n");
		w[0] = v[0];//置换
		w[1] = v[4];
		w[2] = v[8];
		w[3] = v[12];
		w[4] = v[1];
		w[5] = v[5];
		w[6] = v[9];
		w[7] = v[13];
		w[8] = v[2];
		w[9] = v[6];
		w[10] = v[10];
		w[11] = v[14];
		w[12] = v[3];
		w[13] = v[7];
		w[14] = v[11];
		w[15] = v[15];
		for (i = 0; i<16; i = i + 4)
			printf("%d%d%d%d  ", w[i], w[i + 1], w[i + 2], w[i + 3]);
		printf("\n");
	}
	for (i = 0; i < 16; i++)
		u[i] = w[i] ^ K[i + 3 * 4];//轮密钥异或
	for (i = 0; i<16; i = i + 4)
		printf("%d%d%d%d  ", u[i], u[i + 1], u[i + 2], u[i + 3]);
	printf("\n");
	for (i = 0; i < 4; i++)//s盒代换
	{
		V = sbox(u[4 * i + 0], u[4 * i + 1], u[4 * i + 2], u[4 * i + 3]);
		for (j = 3; j >= 0; j--)
		{
			v[j + 4 * i] = V % 2;
			V = V / 2;
		}
	}
	for (i = 0; i<16; i = i + 4)
		printf("%d%d%d%d  ", v[i], v[i + 1], v[i + 2], v[i + 3]);
	printf("\n");
	for (i = 0; i < 16; i++)
		u[i] = v[i] ^ K[i + 4* 4];//轮密钥异或
	printf("加密结果为：");
	for (i = 0; i<16; i = i + 4)
		printf("%d%d%d%d  ", u[i], u[i + 1], u[i + 2], u[i + 3]);
	printf("\n");
	/*.....................解密...................*/
	for (i = 0; i < 16; i++)
		v[i] = u[i] ^ K[i + 4 * 4];//轮密钥异或
	for (i = 0; i<16; i = i + 4)
		printf("%d%d%d%d  ", v[i], v[i + 1], v[i + 2], v[i + 3]);
	printf("\n");
	for (i = 0; i < 4; i++)//s盒代换的逆
	{
		V = v[4 * i + 0] * 8 + v[4 * i + 1] * 4 + v[4 * i + 2] * 2 + v[4 * i + 3];
		U = SBoxofinverse(V);
		for (j = 3; j >= 0; j--)
		{
			u[j + 4 * i] = U % 2;
			U = U/ 2;
		}
	}
	for (i = 0; i<16; i = i + 4)
		printf("%d%d%d%d  ", u[i], u[i + 1], u[i + 2], u[i + 3]);
	printf("\n");
	for (i = 0; i < 16; i++)
		v[i] = u[i] ^ K[i + 3 * 4];//轮密钥异或
	for (i = 0; i<16; i = i + 4)
		printf("%d%d%d%d  ", w[i], w[i + 1], w[i + 2], w[i + 3]);
	printf("\n");
	for (r = 0; r < 3; r++)
	{
		v[0] = w[0];//置换
		v[1] = w[4];
		v[2] = w[8];
		v[3] = w[12];
		v[4] = w[1];
		v[5] = w[5];
		v[6] = w[9];
		v[7] = w[13];
		v[8] = w[2];
		v[9] = w[6];
		v[10] = w[10];
		v[11] = w[14];
		v[12] = w[3];
		v[13] = w[7];
		v[14] = w[11];
		v[15] = w[15];
		for (i = 0; i<16; i = i + 4)
			printf("%d%d%d%d  ", v[i], v[i + 1], v[i + 2], v[i + 3]);
		printf("\n");
		for (i = 0; i < 4; i++)//s盒代换的逆
		{
			V = v[4 * i + 0] * 8 + v[4 * i + 1] * 4 + v[4 * i + 2] * 2 + v[4 * i + 3];
			U = SBoxofinverse(V);
			for (j = 3; j >= 0; j--)
			{
				u[j + 4 * i] = U % 2;
				U = U / 2;
			}
		}
		for (i = 0; i<16; i = i + 4)
			printf("%d%d%d%d  ", u[i], u[i + 1], u[i + 2], u[i + 3]);
		printf("\n");
		for (i = 0; i < 16; i++)
			w[i] = u[i] ^ K[i + (2-r) * 4];//轮密钥异或
		for (i = 0; i<16; i = i + 4)
			printf("%d%d%d%d  ", w[i], w[i + 1], w[i + 2], w[i + 3]);
		printf("\n");
	}
	printf("解密结果为：");
	for (i = 0; i<16; i = i + 4)
		printf("%d%d%d%d  ", w[i], w[i + 1], w[i + 2], w[i + 3]);
	printf("\n");
	getchar();
}

int sbox(int a,int b,int c,int d)//s盒代换
{
	int u,v=0;
	u = a * 8 + b * 4 + c * 2 + d;
	switch (u)
	{
	case 0:v = 14; break;
	case 1:v = 4; break;
	case 2:v = 13; break;
	case 3:v = 1; break;
	case 4:v = 2; break;
	case 5:v = 15; break;
	case 6:v = 11; break;
	case 7:v = 8; break;
	case 8:v = 3; break;
	case 9:v = 10; break;
	case 10:v = 6; break;
	case 11:v = 12; break;
	case 12:v = 5; break;
	case 13:v = 9; break;
	case 14:v = 0; break;
	case 15:v = 7; break;
	}
	return v;
}
int SBoxofinverse(int v)//s盒的逆
{
	int u = 0;
	switch (v)
	{
	case 0:u = 14; break;
	case 1:u = 3; break;
	case 2:u = 4; break;
	case 3:u = 8; break;
	case 4:u = 1; break;
	case 5:u = 12; break;
	case 6:u = 10; break;
	case 7:u = 15; break;
	case 8:u = 7; break;
	case 9:u = 13; break;
	case 10:u = 9; break;
	case 11:u = 6; break;
	case 12:u = 11; break;
	case 13:u = 2; break;
	case 14:u = 0; break;
	case 15:u = 5; break;
	}
	return  u;
}