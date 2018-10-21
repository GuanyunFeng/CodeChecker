#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
unsigned short sboxplace(unsigned short y)
{
	unsigned char sbox[16] = { '\xe','\x4','\xd','\x1','\x2','\xf','\xb','\x8','\x3','\xa','\x6','\xc','\x5','\x9','\x0','\x7' };
	//unsigned char sbox1[16] = {'\x1','\x2','\x3','\x4','\x5','\x6','\x7','\x8','\x9','\xa','\xb','\xc','\xd','\xe','\x0','\x7'};
	int i = 0;
	i = (y >> 12) & 0xf;
	unsigned short result1 = (unsigned short)sbox[i];
	i = ((y << 4) >> 12) & 0xf;
	unsigned short result2 = (unsigned short)sbox[i];
	i = ((y << 8) >> 12) & 0xf;
	unsigned short result3 = (unsigned short)sbox[i];
	i = ((y << 12) >> 12) & 0xf;
	unsigned short result4 = (unsigned short)sbox[i];
	unsigned short result = 0;
	result = result4 ^ (result3 << 4) ^ (result2 << 8) ^ (result1 << 12);
	return result;
}
unsigned short nisboxplace(unsigned short y)
{
	//unsigned char sbox[16] = {'\xe','\x4','\xd','\x1','\x2','\xf','\xb','\x8','\x3','\xa','\x6','\xc','\x5','\x9','\x0','\x7'};
	unsigned char sbox[16] = { '\xe','\x3','\x4','\x8','\x1','\xc','\xa','\xf','\x7','\xd','\x9','\x6','\xb','\x2','\x0','\x5' };
	int i = 0;
	i = (y >> 12) & 0xf;
	unsigned short result1 = (unsigned short)sbox[i];
	i = ((y << 4) >> 12) & 0xf;
	unsigned short result2 = (unsigned short)sbox[i];
	i = ((y << 8) >> 12) & 0xf;
	unsigned short result3 = (unsigned short)sbox[i];
	i = ((y << 12) >> 12) & 0xf;
	unsigned short result4 = (unsigned short)sbox[i];
	unsigned short result = 0;
	result = result4 ^ (result3 << 4) ^ (result2 << 8) ^ (result1 << 12);
	return result;
}
unsigned short zhihuan(unsigned short y)
{
	unsigned short result = 0;
	int target;
	for (target = 0; target < 4; target++)
	{
		result = result ^ ((y << target) >> 15 & 0x1) << (15 - 4 * target);
		result = result ^ ((y << target + 4) >> 15 & 0x1) << (14 - 4 * target);
		result = result ^ ((y << target + 8) >> 15 & 0x1) << (13 - 4 * target);
		result = result ^ ((y << target + 12) >> 15 & 0x1) << (12 - 4 * target);
	}
	return result;
}
int main() {
	unsigned short password;
	//printf("input your password(16)\n");
	//scanf_s("%x", &password);
	unsigned short random[8000][2];
	unsigned short random2[8000][4];
	unsigned short miyao[256];
	unsigned int k = 0x3a94d63f;
	unsigned short key[5] = { 0 };
	int target;
	int target1, target2;
	unsigned char sbox2[16] = { '\xe','\x3','\x4','\x8','\x1','\xc','\xa','\xf','\x7','\xd','\x9','\x6','\xb','\x2','\x0','\x5' };
	for (target = 0; target < 5; target++)
	{
		key[target] = (k << (4 * target)) >> 16;
	}
	password = password ^ key[0];
	password = sboxplace(password);
	password = zhihuan(password);
	password = password ^ key[1];
	password = sboxplace(password);
	password = zhihuan(password);
	password = password ^ key[2];
	password = sboxplace(password);
	password = zhihuan(password);
	password = password ^ key[3];
	password = sboxplace(password);
	password = password ^ key[4];
	printf("%x", password);
	//解密
	password = password ^ key[4];
	password = nisboxplace(password);
	password = password ^ key[3];
	password = zhihuan(password);
	password = nisboxplace(password);
	password = password ^ key[2];
	password = zhihuan(password);
	password = nisboxplace(password);
	password = password ^ key[1];
	password = zhihuan(password);
	password = nisboxplace(password);
	password = password ^ key[0];
	printf("\n%x", password); 
	srand(time(0));
	for (target = 0; target < 8000; target++)
	{
		random[target][0] = rand() % 65536;
		//printf("\n%x", random[target][0]);
		password = random[target][0];
		password = password ^ key[0];
		password = sboxplace(password);
		password = zhihuan(password);
		password = password ^ key[1];
		password = sboxplace(password);
		password = zhihuan(password);
		password = password ^ key[2];
		password = sboxplace(password);
		password = zhihuan(password);
		password = password ^ key[3];
		password = sboxplace(password);
		password = password ^ key[4];
		random[target][1] = password;
		//printf(" %x", random[target][1]);

	}
		srand(time(0));
		for (target = 0; target < 256; target++)
		{
			miyao[target] = target;
		}
		unsigned char ky2, ky4,result;
		 int count[256] = { 0 };
		int max = 0;  
		for (target1 = 0; target1 < 8000; target1++)
		{
			for (target2 = 0; target2 < 256; target2++)
			{
				ky2 = (random[target1][1] >> 8) & 0xf;
				ky2 = ky2 ^ ((miyao[target2] >> 4) & 0xf);
				ky2 = sbox2[ky2];
				result = (ky2 & 0x1) ^ ((ky2 >> 2) & 0x1);
				ky4 = (random[target1][1]) & 0xf;
				ky4 = ky4 ^ ((miyao[target2]) & 0xf);
				ky4 = sbox2[ky4];
				result = (ky4 & 0x1) ^ ((ky4 >> 2) & 0x1) ^ result;
				result = result ^ ((random[target1][0] >> 11) & 0x1) ^ ((random[target1][0] >> 9) & 0x1) ^ ((random[target1][0] >> 8) & 0x1);
				if (result == 0)
					count[target2] = count[target2] + 1;
			}
		}
		if ((count[0] - 4000) < 0)
			count[0] = 4000 - count[0];
		else count[0] = count[0] - 4000;
		for (target2 = 1; target2 < 256; target2++)
		{
			if ((count[target2] - 4000) < 0)
			{
				count[target2] = 4000 - count[target2];
				if (count[0] <= count[target2])
				{
					count[0] = count[target2];
					max = target2;
				}
			}
			else
			{
				count[target2] = count[target2] - 4000;
				if (count[0] <= count[target2])
				{
					count[0] = count[target2];
					max = target2;
				}
			}
		}
		unsigned int a, b;
		a = (miyao[max] >> 4) & 0xf;
		b = (miyao[max]) & 0xf;
		printf("\n   a/b");
		for (target1 = 0; target1 < 16; target1++)
		{
			printf("%6x", target1);
		}
		for(target1 = 0; target1 < 16; target1++)
		{
			printf("\n%6x", target1);
			for (target2 = 0; target2 < 16; target2++)
			{
				if (target2 <= 15)
					printf("%6d", count[target1 * 16 + target2]);
			}
		}
		printf("\n%d", count[max]);
		printf("\n%x", miyao[max]);
		unsigned int a1,b1,c,d;
		c = 0x6;
		d = 0xf;
		int e = 0;
		unsigned int keyright[2000] = { 0 };
		for (a1 = 0; a1 < 0x100000; a1++)
		{
			for (b1 = 0; b1 < 16; b1++)
			{
				k = (a1 << 12) ^ (c << 8) ^ (b1 << 4) ^ d;
				for (target = 0; target < 5; target++)
				{
					key[target] = (k << (4 * target)) >> 16;
				}
				password = 0x26b7;
				password = password ^ key[0];
				password = sboxplace(password);
				password = zhihuan(password);
				password = password ^ key[1];
				password = sboxplace(password);
				password = zhihuan(password);
				password = password ^ key[2];
				password = sboxplace(password);
				password = zhihuan(password);
				password = password ^ key[3];
				password = sboxplace(password);
				password = password ^ key[4];
				if (password == 0xbcd6)
				{
					//printf("\n%x", k);
					keyright[e] = k;
					e++;
				}	
			}
		}
		for (target = 0; target < e; target++)
		{
			for (target1 = 0; target1 < 5; target1++)
			{
				key[target1] = (keyright[target] << (4 * target1)) >> 16;
			}
			password = random[1000][0];
			password = password ^ key[0];
			password = sboxplace(password);
			password = zhihuan(password);
			password = password ^ key[1];
			password = sboxplace(password);
			password = zhihuan(password);
			password = password ^ key[2];
			password = sboxplace(password);
			password = zhihuan(password);
			password = password ^ key[3];
			password = sboxplace(password);
			password = password ^ key[4];
			if (password == random[1000][1])
			{
				printf("\n\nthe right key is %x", keyright[target]); 
			}
		}
		//差分攻击
		k = 0x3a94d63f;
		for (target = 0; target < 5; target++)
		{
			key[target] = (k << (4 * target)) >> 16;
		}
		unsigned short chafen = 0x0b00;
		srand(time(0));
		for (target = 0; target < 8000; target++)
		{
			random2[target][0] = rand() % 65536;
			//printf("%x", random2[target][0]);
			password = random2[target][0];
			random2[target][2] = random2[target][0] ^ chafen;
			password = password ^ key[0];
			password = sboxplace(password);
			password = zhihuan(password);
			password = password ^ key[1];
			password = sboxplace(password);
			password = zhihuan(password);
			password = password ^ key[2];
			password = sboxplace(password);
			password = zhihuan(password);
			password = password ^ key[3];
			password = sboxplace(password);
			password = password ^ key[4];
			random2[target][1] = password;
			//printf(" %x", random2[target][1]);
			password = random2[target][2];
			//printf("\n%x", random2[target][2]);
			password = password ^ key[0];
			password = sboxplace(password);
			password = zhihuan(password);
			password = password ^ key[1];
			password = sboxplace(password);
			password = zhihuan(password);
			password = password ^ key[2];
			password = sboxplace(password);
			password = zhihuan(password);
			password = password ^ key[3];
			password = sboxplace(password);
			password = password ^ key[4];
			random2[target][3] = password;
			//printf(" %x\n", random2[target][3]);
		}
			for (target = 0; target < 256; target++)
			{
				miyao[target] = target;
			}
			unsigned char k52, k54, result2,result3;
			int count2[256] = { 0 };
			int max2 = 0;
			for (target = 0; target < 8000; target++)
			{
				if ((random2[target][1] >> 12 & 0xf) == (random2[target][3] >> 12 & 0xf) && (random2[target][1] >> 4 & 0xf) == (random2[target][3] >> 4 & 0xf))
				for (target2 = 0; target2 < 256; target2++)
				{
					k52 = (random2[target][1] >> 8) & 0xf;
					k52 = k52 ^ (miyao[target2] >> 4 & 0xf);
					k52 = sbox2[k52];
					k54 = (random2[target][3] >> 8) & 0xf;
					k54 = k54 ^ (miyao[target2] >> 4 & 0xf);
					k54 = sbox2[k54];
					result2 = k52 ^ k54;
					k54 = (random2[target][1]) & 0xf;
					k54 = k54 ^ (miyao[target2] & 0xf);
					k54 = sbox2[k54];
					k52 = (random2[target][3]) & 0xf;
					k52 = k52 ^ (miyao[target2] & 0xf);
					k52 = sbox2[k52];
					result3 = k52 ^ k54;
					if((result2 == 0x6) && (result3 == 0x6))
						count2[target2] = count2[target2] + 1;
				}
			}
			for (target = 1; target < 256; target++)
			{
				if (count2[0] < count2[target])
				{ 
					count2[0] = count2[target];
					max2 = target;
				}
			}
			unsigned char a2, b2;
			a2 = (miyao[max2] >> 4) & 0xf;
			b2 = (miyao[max2]) & 0xf;
			printf("\n   a/b");
			for (target1 = 0; target1 < 16; target1++)
			{
				printf("%6x", target1);
			}
			for (target1 = 0; target1 < 16; target1++)
			{
				printf("\n%6x", target1);
				for (target2 = 0; target2 < 16; target2++)
				{
					if (target2 <= 15)
						printf("%6d", count2[target1 * 16 + target2]);
				}
			}
			printf("\n%d", count2[max2]);
			printf("\n%x", miyao[max2]);
			unsigned int a3, b3, c2, d2;
			c2 = 0x6;
			d2 = 0xf;
			int e2 = 0;
			unsigned int keyright2[2000] = { 0 };
			for (a3 = 0; a3 < 0x100000; a3++)
			{
				for (b3 = 0; b3 < 16; b3++)
				{
					k = (a3 << 12) ^ (c2 << 8) ^ (b3 << 4) ^ d2;
					for (target = 0; target < 5; target++)
					{
						key[target] = (k << (4 * target)) >> 16;
					}
					password = 0x26b7;
					password = password ^ key[0];
					password = sboxplace(password);
					password = zhihuan(password);
					password = password ^ key[1];
					password = sboxplace(password);
					password = zhihuan(password);
					password = password ^ key[2];
					password = sboxplace(password);
					password = zhihuan(password);
					password = password ^ key[3];
					password = sboxplace(password);
					password = password ^ key[4];
					if (password == 0xbcd6)
					{
						//printf("\n%x", k);
						keyright2[e2] = k;
						e2++;
					}
				}
			}
			for (target = 0; target < e2; target++)
			{ 
				for (target1 = 0; target1 < 5; target1++)
				{
					key[target1] = (keyright2[target] << (4 * target1)) >> 16;
				}
				password = random2[1000][0];
				password = password ^ key[0];
				password = sboxplace(password);
				password = zhihuan(password);
				password = password ^ key[1];
				password = sboxplace(password);
				password = zhihuan(password);
				password = password ^ key[2];
				password = sboxplace(password);
				password = zhihuan(password);
				password = password ^ key[3];
				password = sboxplace(password);
				password = password ^ key[4];
				if (password == random2[1000][1])
				{
					printf("\n\nthe right key is %x", keyright2[target]);
				}
			}

	return 0; 
}
