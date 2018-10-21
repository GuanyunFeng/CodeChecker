#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int SubBytes(int w);
int XSubBytes(int w);
void XPbox(unsigned int *x, unsigned  int *y);
void Pbox(unsigned int *x, unsigned int *y);
unsigned int *SPN(unsigned int *x, unsigned int *key);

int main()
{
	unsigned int x[4] = { 12345,54321,0,0 },y[4] = { 0,0,0,0 }, u[16], v[16], w[16], k[16], key[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };
    int i = 0, j = 0;
	unsigned int Rcon[10] = { 0x01000000,0x02000000,0x04000000,0x08000000,0x10000000
		,0x20000000,0x40000000,0x80000000,0x1B000000,0x36000000 };
	unsigned int W[44];
	unsigned int temp, B[4];
	//KeyExpansion
	for (i = 0; i < 4; i++)
	{
		W[i] = key[4 * i] * 16777216 + key[4 * i + 1] * 65536 + key[4 * i + 2] * 256 + key[4 * i + 3];
	}
	for (i = 4; i < 44; i++)
	{
		temp = W[i - 1];
		if (i % 4 == 0)
		{
			temp = (temp << 8 | temp >> 24);//循环左移一个字节,RotWord
			B[3] = temp % 256;//SubWord
			temp = temp / 256;
			B[2] = temp % 256;
			temp = temp / 256;
			B[1] = temp % 256;
			B[0] = temp / 256;
			temp = ((SubBytes(B[0]) * 256 + SubBytes(B[1])) * 256 + SubBytes(B[2])) * 256 + SubBytes(B[3]);
			temp = temp ^ Rcon[i / 4 - 1];
		}
		W[i] = W[i - 1] ^ temp;
	}
		for (i = 0; i < 4; i++)//state初始化为w
		{
			temp = x[i];
			w[i * 4 + 3] = temp % 256;
			temp = temp / 256;
			w[i * 4 + 2] = temp % 256;
			temp = temp / 256;
			w[i * 4 + 1] = temp % 256;
			w[i * 4] = temp / 256;
		}
	for (i = 0; i<16; i = i + 4)
	printf("%u %u %u %u ", w[i], w[i + 1], w[i + 2], w[i + 3]);
	printf("\n");
	//spn算法
	for (j = 0; j < 9; j++)
	{
		for (i = 0; i < 4; i++)//生成密钥k
		{
			temp = W[i + 4 * j];
			k[i * 4 + 3] =  temp % 256;
			temp = temp / 256;
			k[i * 4 + 2] = temp % 256;
			temp = temp / 256;
			k[i * 4 + 1]= temp % 256;
			k[i * 4] = temp / 256;
		}
		for (i = 0; i<16; i = i + 4)
			printf("%u %u %u %u ", k[i], k[i + 1], k[i + 2], k[i + 3]);
		printf("\n");
		for (i = 0; i < 16; i++)//Addroundkey
			u[i] = w[i] ^ k[i]; 
		for (i = 0; i<16; i = i + 4)
			printf("%u %u %u %u ", u[i], u[i + 1], u[i + 2], u[i + 3]);
		printf("\n");
		for (i = 0; i < 16; i++)//SubBytes
		{
			v[i] = SubBytes(u[i]);
		}
		for (i = 0; i<16; i = i + 4)
			printf("%u %u %u %u ", v[i], v[i + 1], v[i + 2], v[i + 3]);
		printf("\n");
 	//	for (i = 0; i < 4; i++)//ShiftRows
		//{
		//	w[i] = v[(i + 4 * i) % 16];
		//	w[i + 4] = v[(i + 4 + 4 * i) % 16];
		//	w[i + 8] = v[(i + 8 + 4 * i) % 16];
		//	w[i + 12] = v[(i + 12 + 4 * i) % 16];
		//}
		Pbox(w, v);
		for (i = 0; i<16; i = i + 4)
			printf("%u %u %u %u ", w[i], w[i + 1], w[i + 2], w[i + 3]);
		printf("\n");
	}

	for (i = 0; i < 4; i++)//生成密钥k
	{
		temp = W[i + 4 * 9];
		k[i * 4 + 3] = temp % 256;;
		temp = temp / 256;
		k[i * 4 + 2] = temp % 256;;
		temp = temp / 256;;
		k[i * 4 + 1] = temp % 256;;
		k[i * 4] = temp / 256;;
	}
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", k[i], k[i + 1], k[i + 2], k[i + 3]);
	printf("\n");
	for (i = 0; i < 16; i++)//Addroundkey
		u[i] = w[i] ^ k[i];
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", u[i], u[i + 1], u[i + 2], u[i + 3]);
	printf("\n");
	for (i = 0; i < 16; i++)//SubBytes
	{
		v[i] = SubBytes(u[i]);
	}
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", v[i], v[i + 1], v[i + 2], v[i + 3]);
	printf("\n");
	for (i = 0; i < 4; i++)//生成密钥k
	{
		temp = W[i + 4 * 10];
		k[i * 4 + 3] = temp % 256;;
		temp = temp / 256;;
		k[i * 4 + 2] = temp % 256;;
		temp = temp / 256;;
		k[i * 4 + 1] = temp % 256;;
		k[i * 4] = temp / 256;;
	}
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", k[i], k[i + 1], k[i + 2], k[i + 3]);
	printf("\n");
	
	for (i = 0; i < 16; i++)//Addroundkey
	{
		u[i] = v[i] ^ k[i];
	}
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", u[i], u[i + 1], u[i + 2], u[i + 3]);
	printf("\n");
	for (i = 0; i < 4; i++)
	{
		y[i] = 0;//初始化
		y[i] = ((u[4*i] * 256 + u[4*i + 1]) * 256 + u[4*i + 2]) * 256 + u[4*i + 3];
	}
	
	printf("加密密文为：%u %u %u %u \n", y[0],y[1],y[2],y[3]);
	//y1 = SPN(x, key);
	//printf("加密密文为：%u %u %u %u \n", y1[0], y1[1], y1[2], y1[3]);
//.................解密算法.......................
	for (i = 0; i < 4; i++)
	{
		temp = y[i];
		u[i * 4 + 3] = temp % 256;
		temp = temp / 256;
		u[i * 4 + 2] = temp % 256;
		temp = temp / 256;
		u[i * 4 + 1] = temp % 256;
		u[i * 4] = temp / 256;
	}
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", u[i], u[i + 1], u[i + 2], u[i + 3]);
	printf("\n");
	for (i = 0; i < 4; i++)//生成密钥k
	{
		temp = W[i + 4 * 10];
		k[i * 4 + 3] = temp % 256;;
		temp = temp / 256;;
		k[i * 4 + 2] = temp % 256;;
		temp = temp / 256;;
		k[i * 4 + 1] = temp % 256;;
		k[i * 4] = temp / 256;;
	}
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", k[i], k[i + 1], k[i + 2], k[i + 3]);
	printf("\n");
	for (i = 0; i < 16; i++)//Addroundkey
	{
		v[i] = u[i] ^ k[i];
	}
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", v[i], v[i + 1], v[i + 2], v[i + 3]);
	printf("\n");
	for (i = 0; i < 16; i++)//XSubBytes
	{
		u[i] = XSubBytes(v[i]);
	}
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", u[i], u[i + 1], u[i + 2], u[i + 3]);
	printf("\n");
	for (i = 0; i < 4; i++)//生成密钥k
	{
		temp = W[i + 4 * 9];
		k[i * 4 + 3] = temp % 256;;
		temp = temp / 256;
		k[i * 4 + 2] = temp % 256;;
		temp = temp / 256;;
		k[i * 4 + 1] = temp % 256;;
		k[i * 4] = temp / 256;;
	}
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", k[i], k[i + 1], k[i + 2], k[i + 3]);
	printf("\n");
	for (i = 0; i < 16; i++)//Addroundkey
		w[i] = u[i] ^ k[i];
	for (i = 0; i<16; i = i + 4)
		printf("%u %u %u %u ", w[i], w[i + 1], w[i + 2], w[i + 3]);
	printf("\n");
	for (j = 8; j >=0; j--)
	{
		//for (i = 0; i < 4; i++)//XShiftRows
		//{
		//	v[i] = w[(i - 4 * i+16) % 16];
		//	v[i + 4] = w[(i + 4 -4 * i+16) % 16];
		//	v[i + 8] = w[(i + 8 - 4 * i+16) % 16];
		//	v[i + 12] = w[(i + 12 - 4 * i + 16) % 16];
		//}
		XPbox(v, w);
		for (i = 0; i<16; i = i + 4)
			printf("%u %u %u %u ", v[i], v[i + 1], v[i + 2], v[i + 3]);
		printf("\n");
		for (i = 0; i < 16; i++)//XSubBytes
		{
			u[i] = XSubBytes(v[i]);
		}
		for (i = 0; i<16; i = i + 4)
			printf("%u %u %u %u ", u[i], u[i + 1], u[i + 2], u[i + 3]);
		printf("\n");
		for (i = 0; i < 4; i++)//生成密钥k
		{
			temp = W[i + 4 * j];
			k[i * 4 + 3] = temp % 256;
			temp = temp / 256;
			k[i * 4 + 2] = temp % 256;
			temp = temp / 256;
			k[i * 4 + 1] = temp % 256;
			k[i * 4] = temp / 256;
		}
		for (i = 0; i<16; i = i + 4)
			printf("%u %u %u %u ", k[i], k[i + 1], k[i + 2], k[i + 3]);
		printf("\n");
		for (i = 0; i < 16; i++)//Addroundkey
			w[i] = u[i] ^ k[i];	
		for (i = 0; i<16; i = i + 4)
			printf("%u %u %u %u ", w[i], w[i + 1], w[i + 2], w[i + 3]);
		printf("\n");
	}
	
	for (i = 0; i < 4; i++)
	{
		x[i] = 0;//初始化
		x[i] = ((w[4*i] * 256 + w[4*i + 1]) * 256 + w[4*i + 2]) * 256 + w[4*i + 3];
	}
	printf("解密出的明文为：%u %u %u %u ", x[0],x[1],x[2],x[3]);
	getchar();
}



//int main()
//{
//	unsigned int x[4] = { 0,0, 0,0 }, *y,key[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };
//	int i = 0, j = 0;
//	unsigned int Rcon[10] = { 0x01000000,0x02000000,0x04000000,0x08000000,0x10000000
//		,0x20000000,0x40000000,0x80000000,0x1B000000,0x36000000 }; 
//	unsigned int temp;
//	FILE *fp = NULL;//CBC模式
//	//y = SPN(x,key);
//	//fp = fopen("F:\data.txt", "a +");
//	//if (fp == NULL) return 0;
//	//for (i = 0; i < 655360; i++)
//	//{
//	//	fwrite(y, sizeof(char), 16, fp);
//	//	y = SPN(y,key);
//	//}
//	//fclose(fp);
//	//CFB模式
//	y = SPN(x, key);
//	fp = fopen("F:\data2.txt", "a +");
//	if (fp == NULL) return 0;
//	for (i = 0; i < 655360; i++)
//	{
//		fwrite(y, sizeof(char), 16, fp);
//		for (int j= 0; j < 4; j++)//生成密钥key
//		{
//			temp = y[j ];
//			key[j* 4 + 3] = temp % 256;;
//			temp = temp / 256;;
//			key[j* 4 + 2] = temp % 256;;
//			temp = temp / 256;;
//			key[j * 4 + 1] = temp % 256;;
//			key[j* 4] = temp / 256;;
//		}
//		y = SPN(x,key );
//	}
//	fclose(fp);
//}

unsigned int *SPN(unsigned int *x, unsigned int *key)
{
	unsigned int *y = (unsigned int*)malloc(sizeof(unsigned int) * 4), u[16], v[16], w[16], k[16];
	int i = 0, j = 0;
	unsigned int Rcon[10] = { 0x01000000,0x02000000,0x04000000,0x08000000,0x10000000
		,0x20000000,0x40000000,0x80000000,0x1B000000,0x36000000 };
	unsigned int W[44];
	unsigned int temp, B[4];

	/*for (i = 0; i<16; i = i + 4)
	printf("%u %u %u %u ", w[i], w[i + 1], w[i + 2], w[i + 3]);
	printf("\n");*/
	//KeyExpansion
	for (i = 0; i < 4; i++)
	{
		W[i] = key[4 * i] * 16777216 + key[4 * i + 1] * 65536 + key[4 * i + 2] * 256 + key[4 * i + 3];
	}
	for (i = 4; i < 44; i++)
	{
		temp = W[i - 1];
		if (i % 4 == 0)
		{
			temp = (temp << 8 | temp >> 24);//循环左移一个字节,RotWord
			B[3] = temp % 256;//SubWord
			temp = temp / 256;
			B[2] = temp % 256;
			temp = temp / 256;
			B[1] = temp % 256;
			B[0] = temp / 256;
			temp = ((SubBytes(B[0]) * 256 + SubBytes(B[1])) * 256 + SubBytes(B[2])) * 256 + SubBytes(B[3]);
			temp = temp ^ Rcon[i / 4 - 1];
		}
		W[i] = W[i - 1] ^ temp;
	}

	for (i = 0; i < 4; i++)//state初始化为w
	{
		temp = x[i];
		w[i * 4 + 3] = temp % 256;
		temp = temp / 256;
		w[i * 4 + 2] = temp % 256;
		temp = temp / 256;
		w[i * 4 + 1] = temp % 256;
		w[i * 4] = temp / 256;
	}
	//spn算法
	for (j = 0; j < 9; j++)
	{
		for (i = 0; i < 4; i++)//生成密钥k
		{
			temp = W[i + 4 * j];
			k[i * 4 + 3] = temp % 256;
			temp = temp / 256;
			k[i * 4 + 2] = temp % 256;
			temp = temp / 256;
			k[i * 4 + 1] = temp % 256;
			k[i * 4] = temp / 256;
		}
		for (i = 0; i < 16; i++)//Addroundkey
			u[i] = w[i] ^ k[i];
		for (i = 0; i < 16; i++)//SubBytes
		{
			v[i] = SubBytes(u[i]);
		}
		Pbox(w, v);
	}

	for (i = 0; i < 4; i++)//生成密钥k
	{
		temp = W[i + 4 * 9];
		k[i * 4 + 3] = temp % 256;;
		temp = temp / 256;
		k[i * 4 + 2] = temp % 256;;
		temp = temp / 256;;
		k[i * 4 + 1] = temp % 256;;
		k[i * 4] = temp / 256;;
	}
	for (i = 0; i < 16; i++)//Addroundkey
		u[i] = w[i] ^ k[i];
	for (i = 0; i < 16; i++)//SubBytes
	{
		v[i] = SubBytes(u[i]);
	}
	for (i = 0; i < 4; i++)//生成密钥k
	{
		temp = W[i + 4 * 10];
		k[i * 4 + 3] = temp % 256;;
		temp = temp / 256;;
		k[i * 4 + 2] = temp % 256;;
		temp = temp / 256;;
		k[i * 4 + 1] = temp % 256;;
		k[i * 4] = temp / 256;;
	}
	for (i = 0; i < 16; i++)//Addroundkey
	{
		u[i] = v[i] ^ k[i];
	}
	for (i = 0; i < 4; i++)
	{
		y[i] = 0;//初始化
		y[i] = ((u[4 * i] * 256 + u[4 * i + 1]) * 256 + u[4 * i + 2]) * 256 + u[4 * i + 3];
	}
	return y;
}

int SubBytes(int w)
{
	int x, y, z=0;
	int sbox[16][16] = 
	   {
		{ 0x63 ,0x7c ,0x77 ,0x7b,0xf2 ,0x6b ,0x6f ,0xc5 ,0x30  ,0x1,0x67 ,0x2b ,0xfe ,0xd7 ,0xab ,0x76}
		,{0xca ,0x82 ,0xc9 ,0x7d ,0xfa ,0x59 ,0x47 ,0xf0 ,0xad ,0xd4 ,0xa2 ,0xaf ,0x9c ,0xa4 ,0x72 ,0xc0}
		,{0xb7 ,0xfd ,0x93 ,0x26 ,0x36 ,0x3f ,0xf7 ,0xcc ,0x34 ,0xa5 ,0xe5 ,0xf1 ,0x71 ,0xd8 ,0x31 ,0x15}
		,{0x4 ,0xc7 ,0x23 ,0xc3 ,0x18 ,0x96  ,0x5 ,0x9a  ,0x7 ,0x12 ,0x80 ,0xe2 ,0xeb ,0x27 ,0xb2 ,0x75}
		,{0x9 ,0x83 ,0x2c ,0x1a ,0x1b ,0x6e ,0x5a ,0xa0 ,0x52 ,0x3b ,0xd6 ,0xb3 ,0x29 ,0xe3 ,0x2f ,0x84}
		,{0x53 ,0xd1 ,0x0 ,0xed ,0x20 ,0xfc ,0xb1 ,0x5b ,0x6a ,0xcb ,0xbe ,0x39 ,0x4a ,0x4c ,0x58 ,0xcf }
		,{ 0xd0 ,0xef ,0xaa ,0xfb ,0x43 ,0x4d ,0x33 ,0x85 ,0x45 ,0xf9  ,0x2 ,0x7f ,0x50 ,0x3c ,0x9f ,0xa8 }
		,{ 0x51 ,0xa3 ,0x40 ,0x8f ,0x92 ,0x9d ,0x38 ,0xf5 ,0xbc ,0xb6 ,0xda ,0x21 ,0x10 ,0xff ,0xf3 ,0xd2 }
		,{ 0xcd  ,0xc ,0x13 ,0xec ,0x5f ,0x97 ,0x44 ,0x17 ,0xc4 ,0xa7 ,0x7e ,0x3d ,0x64 ,0x5d ,0x19 ,0x73 }
		,{ 0x60 ,0x81 ,0x4f ,0xdc ,0x22 ,0x2a ,0x90 ,0x88 ,0x46 ,0xee ,0xb8 ,0x14 ,0xde ,0x5e  ,0xb ,0xdb }
		,{ 0xe0 ,0x32 ,0x3a  ,0xa ,0x49  ,0x6 ,0x24 ,0x5c ,0xc2 ,0xd3 ,0xac ,0x62 ,0x91 ,0x95 ,0xe4 ,0x79 }
		,{ 0xe7 ,0xc8 ,0x37 ,0x6d ,0x8d ,0xd5 ,0x4e ,0xa9 ,0x6c ,0x56 ,0xf4 ,0xea ,0x65 ,0x7a ,0xae  ,0x8 }
		,{ 0xba ,0x78 ,0x25 ,0x2e ,0x1c ,0xa6 ,0xb4 ,0xc6 ,0xe8 ,0xdd ,0x74 ,0x1f ,0x4b ,0xbd ,0x8b ,0x8a }
		,{ 0x70 ,0x3e ,0xb5 ,0x66 ,0x48 ,0x3 ,0xf6 ,0xe ,0x61 ,0x35 ,0x57 ,0xb9 ,0x86 ,0xc1 ,0x1d ,0x9e }
		,{ 0xe1 ,0xf8 ,0x98 ,0x11 ,0x69 ,0xd9 ,0x8e ,0x94 ,0x9b ,0x1e ,0x87 ,0xe9 ,0xce ,0x55 ,0x28 ,0xdf }
		,{ 0x8c ,0xa1 ,0x89 ,0xd ,0xbf ,0xe6 ,0x42 ,0x68 ,0x41 ,0x99 ,0x2d  ,0xf ,0xb0 ,0x54 ,0xbb ,0x16 }};
	x = w / 16;
	y = w % 16;
	if(x<16)  z = sbox[x][y];
	else printf("输入格式错误！");
	return z;
}

int XSubBytes(int w)
{
	int x, y, z;
	int sbox[16][16] =
	{
		{ 0x63 ,0x7c ,0x77 ,0x7b,0xf2 ,0x6b ,0x6f ,0xc5 ,0x30  ,0x1,0x67 ,0x2b ,0xfe ,0xd7 ,0xab ,0x76 }
		,{ 0xca ,0x82 ,0xc9 ,0x7d ,0xfa ,0x59 ,0x47 ,0xf0 ,0xad ,0xd4 ,0xa2 ,0xaf ,0x9c ,0xa4 ,0x72 ,0xc0 }
		,{ 0xb7 ,0xfd ,0x93 ,0x26 ,0x36 ,0x3f ,0xf7 ,0xcc ,0x34 ,0xa5 ,0xe5 ,0xf1 ,0x71 ,0xd8 ,0x31 ,0x15 }
		,{ 0x4 ,0xc7 ,0x23 ,0xc3 ,0x18 ,0x96  ,0x5 ,0x9a  ,0x7 ,0x12 ,0x80 ,0xe2 ,0xeb ,0x27 ,0xb2 ,0x75 }
		,{ 0x9 ,0x83 ,0x2c ,0x1a ,0x1b ,0x6e ,0x5a ,0xa0 ,0x52 ,0x3b ,0xd6 ,0xb3 ,0x29 ,0xe3 ,0x2f ,0x84 }
		,{ 0x53 ,0xd1 ,0x0 ,0xed ,0x20 ,0xfc ,0xb1 ,0x5b ,0x6a ,0xcb ,0xbe ,0x39 ,0x4a ,0x4c ,0x58 ,0xcf }
		,{ 0xd0 ,0xef ,0xaa ,0xfb ,0x43 ,0x4d ,0x33 ,0x85 ,0x45 ,0xf9  ,0x2 ,0x7f ,0x50 ,0x3c ,0x9f ,0xa8 }
		,{ 0x51 ,0xa3 ,0x40 ,0x8f ,0x92 ,0x9d ,0x38 ,0xf5 ,0xbc ,0xb6 ,0xda ,0x21 ,0x10 ,0xff ,0xf3 ,0xd2 }
		,{ 0xcd  ,0xc ,0x13 ,0xec ,0x5f ,0x97 ,0x44 ,0x17 ,0xc4 ,0xa7 ,0x7e ,0x3d ,0x64 ,0x5d ,0x19 ,0x73 }
		,{ 0x60 ,0x81 ,0x4f ,0xdc ,0x22 ,0x2a ,0x90 ,0x88 ,0x46 ,0xee ,0xb8 ,0x14 ,0xde ,0x5e  ,0xb ,0xdb }
		,{ 0xe0 ,0x32 ,0x3a  ,0xa ,0x49  ,0x6 ,0x24 ,0x5c ,0xc2 ,0xd3 ,0xac ,0x62 ,0x91 ,0x95 ,0xe4 ,0x79 }
		,{ 0xe7 ,0xc8 ,0x37 ,0x6d ,0x8d ,0xd5 ,0x4e ,0xa9 ,0x6c ,0x56 ,0xf4 ,0xea ,0x65 ,0x7a ,0xae  ,0x8 }
		,{ 0xba ,0x78 ,0x25 ,0x2e ,0x1c ,0xa6 ,0xb4 ,0xc6 ,0xe8 ,0xdd ,0x74 ,0x1f ,0x4b ,0xbd ,0x8b ,0x8a }
		,{ 0x70 ,0x3e ,0xb5 ,0x66 ,0x48 ,0x3 ,0xf6 ,0xe ,0x61 ,0x35 ,0x57 ,0xb9 ,0x86 ,0xc1 ,0x1d ,0x9e }
		,{ 0xe1 ,0xf8 ,0x98 ,0x11 ,0x69 ,0xd9 ,0x8e ,0x94 ,0x9b ,0x1e ,0x87 ,0xe9 ,0xce ,0x55 ,0x28 ,0xdf }
	,{ 0x8c ,0xa1 ,0x89 ,0xd ,0xbf ,0xe6 ,0x42 ,0x68 ,0x41 ,0x99 ,0x2d  ,0xf ,0xb0 ,0x54 ,0xbb ,0x16 } };
	for (x = 0; x < 16; x++)
		for (y = 0; y < 16; y++)
			if (sbox[x][y] == w)  goto A;
	A: z = x * 16 + y;
	return z;
}

void Pbox(unsigned int *x, unsigned int *y)
{
	int P[128] = { 90, 41, 32, 110, 66, 54, 5, 34, 21, 39, 40, 84, 99, 118, 97, 25,
		91, 31, 80, 95, 52, 81, 111, 42, 51, 100, 71, 7, 18, 33, 48, 50,
		16, 114, 105, 126, 60, 61, 83, 56, 15, 88, 12, 23, 67, 11, 92, 125,
		69, 104, 68, 36, 3, 75, 124, 30, 113, 96, 20, 43, 85, 98, 119, 59,
		82, 6, 14, 29, 2, 86, 55, 49, 65, 76, 53, 117, 112, 74, 123, 9,
		44, 35, 38, 115, 108, 73, 57, 19, 22, 37, 17, 45, 78, 58, 107, 28,
		122, 47, 70, 13, 116, 63, 77, 27, 102, 120, 106, 127, 0, 10, 87, 64,
		1, 89, 79, 72, 103, 8, 109, 4, 121, 46, 94, 26, 24, 62, 101, 93 };
	bool tmpBool[128], tmpState[128];
	int count, i,k;
	for ( i = 0; i < 16; i++) {
		for ( k = 0; k < 8; k++)
		{
			tmpState[i * 8+ k] = (y[i] >> k) & 0x01;//取最后一位
		}
	}
	for ( i = 0; i < 128; i++) {
		tmpBool[i] = tmpState[P[i]];//存放置换后的结果
	}
	for ( i = 0; i < 16; i++)
	{
		x[i] = 0;
		for ( k = 0, count = 1; k < 8; k++, count *= 2) 
		{
			x[i] += tmpBool[i * 8 + k] * count;
		}
	}
}

	void XPbox(unsigned int *x, unsigned int *y)
	{
		int P_1[128] = { 108, 112, 68, 52, 119, 6, 65, 27, 117, 79, 109, 45, 42, 99, 66, 40,
			32, 90, 28, 87, 58, 8, 88, 43, 124, 15, 123, 103, 95, 67, 55, 17,
			2, 29, 7, 81, 51, 89, 82, 9, 10, 1, 23, 59, 80, 91, 121, 97,
			30, 71, 31, 24, 20, 74, 5, 70, 39, 86, 93, 63, 36, 37, 125, 101,
			111, 72, 4, 44, 50, 48, 98, 26, 115, 85, 77, 53, 73, 102, 92, 114,
			18, 21, 64, 38, 11, 60, 69, 110, 41, 113, 0, 16, 46, 127, 122, 19,
			57, 14, 61, 12, 25, 126, 104, 116, 49, 34, 106, 94, 84, 118, 3, 22,
			76, 56, 33, 83, 100, 75, 13, 62, 105, 120, 96, 78, 54, 47, 35, 107 };
		bool tmpBool[128], tmpState[128];
		int count,i,k;
		for ( i = 0; i < 16; i++) {
				for (int k = 0; k < 8; k++) {
					tmpState[i * 8  + k] = (y[i] >> k) & 0x01;
				}
		}
		for (i = 0; i < 128; i++) {
			tmpBool[i] = tmpState[P_1[i]];
		}
		for (i = 0; i < 16; i++) {
				x[i] = 0;
				for (k = 0, count = 1; k < 8; k++, count *= 2) {
					x[i] += tmpBool[i * 8 + k] * count;
				}
		}
	}


