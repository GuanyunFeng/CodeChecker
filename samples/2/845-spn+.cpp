#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
//spnpp
unsigned char password[4][4] = { 0 };
unsigned char password1[4][4] = { 0 };
unsigned char password2[4][4] = { 0 };
unsigned int key1[4] = { 0 };
unsigned char key2[11][4][4] = { 0 };
unsigned short  mx = 0x011b;
int seekat7(unsigned char temp)
{
	if (temp & 0x80)  return 1;
	else   return 0;
}
int seekat6(unsigned char temp)
{
	if (temp & 0x40)  return 1;
	else   return 0;
}
int seekat5(unsigned char temp)
{
	if (temp & 0x20)  return 1;
	else   return 0;
}
int seekat4(unsigned char temp)
{
	if (temp & 0x10)  return 1;
	else   return 0;
}
int seekat3(unsigned char temp)
{
	if (temp & 0x08)  return 1;
	else   return 0;
}
int seekat2(unsigned char temp)
{
	if (temp & 0x04)  return 1;
	else   return 0;
}
int seekat1(unsigned char temp)
{
	if (temp & 0x02)  return 1;
	else   return 0;
}
int seekat0(unsigned char temp)
{
	if (temp & 0x01)  return 1;
	else   return 0;
}
int length(unsigned short  temp)
{

	int i = 0;
	while (1)
	{

		if (temp == 0)  break;
		temp = temp >> 1;
		i++;
	}

	return i;

}
unsigned char  multwo(unsigned char temp1, unsigned char temp2)
{
	unsigned short  out = 0;
	if (seekat7(temp1)*seekat7(temp2)) out += 0x4000;
	if ((seekat7(temp1)*seekat6(temp2)) ^ (seekat6(temp1)*seekat7(temp2))) out += 0x2000;
	if ((seekat7(temp1)*seekat5(temp2)) ^ (seekat6(temp1)*seekat6(temp2)) ^ (seekat5(temp1)*seekat7(temp2))) out += 0x1000;
	if ((seekat7(temp1)*seekat4(temp2)) ^ (seekat6(temp1)*seekat5(temp2)) ^ (seekat5(temp1)*seekat6(temp2)) ^ (seekat4(temp1)*seekat7(temp2))) out += 0x0800;
	if ((seekat7(temp1)*seekat3(temp2)) ^ (seekat6(temp1)*seekat4(temp2)) ^ (seekat5(temp1)*seekat5(temp2)) ^ (seekat4(temp1)*seekat6(temp2)) ^ (seekat3(temp1)*seekat7(temp2))) out += 0x0400;
	if ((seekat7(temp1)*seekat2(temp2)) ^ (seekat6(temp1)*seekat3(temp2)) ^ (seekat5(temp1)*seekat4(temp2)) ^ (seekat4(temp1)*seekat5(temp2)) ^ (seekat3(temp1)*seekat6(temp2)) ^ (seekat2(temp1)*seekat7(temp2))) out += 0x0200;

	if ((seekat7(temp1)*seekat1(temp2)) ^ (seekat6(temp1)*seekat2(temp2)) ^ (seekat5(temp1)*seekat3(temp2)) ^ (seekat4(temp1)*seekat4(temp2)) ^ (seekat3(temp1)*seekat5(temp2)) ^ (seekat2(temp1)*seekat6(temp2)) ^ (seekat1(temp1)*seekat7(temp2)))  out += 0x0100;

	if ((seekat7(temp1)*seekat0(temp2)) ^ (seekat6(temp1)*seekat1(temp2)) ^ (seekat5(temp1)*seekat2(temp2)) ^ (seekat4(temp1)*seekat3(temp2)) ^ (seekat3(temp1)*seekat4(temp2)) ^ (seekat2(temp1)*seekat5(temp2)) ^ (seekat1(temp1)*seekat6(temp2)) ^ (seekat0(temp1)*seekat7(temp2))) out += 0x0080;

	if ((seekat6(temp1)*seekat0(temp2)) ^ (seekat5(temp1)*seekat1(temp2)) ^ (seekat4(temp1)*seekat2(temp2)) ^ (seekat3(temp1)*seekat3(temp2)) ^ (seekat2(temp1)*seekat4(temp2)) ^ (seekat1(temp1)*seekat5(temp2)) ^ (seekat0(temp1)*seekat6(temp2)))  out += 0x0040;

	if ((seekat5(temp1)*seekat0(temp2)) ^ (seekat4(temp1)*seekat1(temp2)) ^ (seekat3(temp1)*seekat2(temp2)) ^ (seekat2(temp1)*seekat3(temp2)) ^ (seekat1(temp1)*seekat4(temp2)) ^ (seekat0(temp1)*seekat5(temp2)))  out += 0x0020;

	if ((seekat4(temp1)*seekat0(temp2)) ^ (seekat3(temp1)*seekat1(temp2)) ^ (seekat2(temp1)*seekat2(temp2)) ^ (seekat1(temp1)*seekat3(temp2)) ^ (seekat0(temp1)*seekat4(temp2)))  out += 0x0010;

	if ((seekat3(temp1)*seekat0(temp2)) ^ (seekat2(temp1)*seekat1(temp2)) ^ (seekat1(temp1)*seekat2(temp2)) ^ (seekat0(temp1)*seekat3(temp2))) out += 0x0008;

	if ((seekat2(temp1)*seekat0(temp2)) ^ (seekat1(temp1)*seekat1(temp2)) ^ (seekat0(temp1)*seekat2(temp2))) out += 0x0004;

	if ((seekat1(temp1)*seekat0(temp2)) ^ (seekat0(temp1)*seekat1(temp2)))  out += 0x0002;
	if (seekat0(temp1)*seekat0(temp2))  out += 0x0001;
	unsigned short  temp;
	temp = out;
	unsigned short bemod;
	while (temp>256 || temp == 256)
	{
		bemod = mx;
		bemod = bemod << (length(temp) - 9);
		temp = temp ^ bemod;
	}

	return  (unsigned char)temp;
}
unsigned char sboxplace(unsigned char y)
{
	unsigned char sbox[256] = { '\x63','\x7c','\x77','\x7b','\xf2','\x6b','\x6f','\xc5','\x30','\x01','\x67','\x2b','\xfe','\xd7','\xab','\x76',\
		'\xca','\x82','\xc9','\x7d','\xfa','\x59','\x47','\xf0','\xad','\xd4','\xa2','\xaf','\x9c','\xa4','\x72','\xc0',\
		'\xb7','\xfd','\x93','\x26','\x36','\x3f','\xf7','\xcc','\x34','\xa5','\xe5','\xf1','\x71','\xd8','\x31','\x15',\
		'\x04','\xc7','\x23','\xc3','\x18','\x96','\x05','\x9a','\x07','\x12','\x80','\xe2','\xeb','\x27','\xb2','\x75',\
		'\x09','\x83','\x2c','\x1a','\x1b','\x6e','\x5a','\xa0','\x52','\x3b','\xd6','\xb3','\x29','\xe3','\x2f','\x84',\
		'\x53','\xd1','\x00','\xed','\x20','\xfc','\xb1','\x5b','\x6a','\xcb','\xbe','\x39','\x4a','\x4c','\x58','\xcf',\
		'\xd0','\xef','\xaa','\xfb','\x43','\x4d','\x33','\x85','\x45','\xf9','\x02','\x7f','\x50','\x3c','\x9f','\xa8',\
		'\x51','\xa3','\x40','\x8f','\x92','\x9d','\x38','\xf5','\xbc','\xb6','\xda','\x21','\x10','\xff','\xf3','\xd2',\
		'\xcd','\x0c','\x13','\xec','\x5f','\x97','\x44','\x17','\xc4','\xa7','\x7e','\x3d','\x64','\x5d','\x19','\x73',\
		'\x60','\x81','\x4f','\xdc','\x22','\x2a','\x90','\x88','\x46','\xee','\xb8','\x14','\xde','\x5e','\x0b','\xdb',\
		'\xe0','\x32','\x3a','\x0a','\x49','\x06','\x24','\x5c','\xc2','\xd3','\xac','\x62','\x91','\x95','\xe4','\x79',\
		'\xe7','\xc8','\x37','\x6d','\x8d','\xd5','\x4e','\xa9','\x6c','\x56','\xf4','\xea','\x65','\x7a','\xae','\x08',\
		'\xba','\x78','\x25','\x2e','\x1c','\xa6','\xb4','\xc6','\xe8','\xdd','\x74','\x1f','\x4b','\xbd','\x8b','\x8a',\
		'\x70','\x3e','\xb5','\x66','\x48','\x03','\xf6','\x0e','\x61','\x35','\x57','\xb9','\x86','\xc1','\x1d','\x9e',\
		'\xe1','\xf8','\x98','\x11','\x69','\xd9','\x8e','\x94','\x9b','\x1e','\x87','\xe9','\xce','\x55','\x28','\xdf',\
		'\x8c','\xa1','\x89','\x0d','\xbf','\xe6','\x42','\x68','\x41','\x99','\x2d','\x0f','\xb0','\x54','\xbb','\x16' };
	unsigned char result = 0;
	result = sbox[y];
	return result;
}
unsigned char nisboxplace(unsigned char y)
{
	unsigned char sbox[256] = { '\x52','\x09','\x6a','\xd5','\x30','\x36','\xa5','\x38','\xbf','\x40','\xa3','\x9e','\x81','\xf3','\xd7','\xfb',\
		'\x7c','\xe3','\x39','\x82','\x9b','\x2f','\xff','\x87','\x34','\x8e','\x43','\x44','\xc4','\xde','\xe9','\xcb',\
		'\x54','\x7b','\x94','\x32','\xa6','\xc2','\x23','\x3d','\xee','\x4c','\x95','\x0b','\x42','\xfa','\xc3','\x4e',\
		'\x08','\x2e','\xa1','\x66','\x28','\xd9','\x24','\xb2','\x76','\x5b','\xa2','\x49','\x6d','\x8b','\xd1','\x25',\
		'\x72','\xf8','\xf6','\x64','\x86','\x68','\x98','\x16','\xd4','\xa4','\x5c','\xcc','\x5d','\x65','\xb6','\x92',\
		'\x6c','\x70','\x48','\x50','\xfd','\xed','\xb9','\xda','\x5e','\x15','\x46','\x57','\xa7','\x8d','\x9d','\x84',\
		'\x90','\xd8','\xab','\x00','\x8c','\xbc','\xd3','\x0a','\xf7','\xe4','\x58','\x05','\xb8','\xb3','\x45','\x06',\
		'\xd0','\x2c','\x1e','\x8f','\xca','\x3f','\x0f','\x02','\xc1','\xaf','\xbd','\x03','\x01','\x13','\x8a','\x6b',\
		'\x3a','\x91','\x11','\x41','\x4f','\x67','\xdc','\xea','\x97','\xf2','\xcf','\xce','\xf0','\xb4','\xe6','\x73',\
		'\x96','\xac','\x74','\x22','\xe7','\xad','\x35','\x85','\xe2','\xf9','\x37','\xe8','\x1c','\x75','\xdf','\x6e',\
		'\x47','\xf1','\x1a','\x71','\x1d','\x29','\xc5','\x89','\x6f','\xb7','\x62','\x0e','\xaa','\x18','\xbe','\x1b',\
		'\xfc','\x56','\x3e','\x4b','\xc6','\xd2','\x79','\x20','\x9a','\xdb','\xc0','\xfe','\x78','\xcd','\x5a','\xf4',\
		'\x1f','\xdd','\xa8','\x33','\x88','\x07','\xc7','\x31','\xb1','\x12','\x10','\x59','\x27','\x80','\xec','\x5f',\
		'\x60','\x51','\x7f','\xa9','\x19','\xb5','\x4a','\x0d','\x2d','\xe5','\x7a','\x9f','\x93','\xc9','\x9c','\xef',\
		'\xa0','\xe0','\x3b','\x4d','\xae','\x2a','\xf5','\xb0','\xc8','\xeb','\xbb','\x3c','\x83','\x53','\x99','\x61',\
		'\x17','\x2b','\x04','\x7e','\xba','\x77','\xd6','\x26','\xe1','\x69','\x14','\x63','\x55','\x21','\x0c','\x7d' };
	unsigned char result = 0;
	result = sbox[y];
	return result;
}
void miyao(void)
{
	int target, target2, target1;
	key1[0] = 0x00000000;
	key1[1] = 0x00000000;
	key1[2] = 0x00000000;
	key1[3] = 0x00000000;
	for (target = 1; target < 11; target++)
	{
		key1[3] = ((key1[3] >> 24) & 0xff) ^ ((key1[3] << 8) & 0xffffff00);
		key1[3] = (sboxplace((key1[3] >> 24) & 0xff) << 24) ^ (sboxplace((key1[3] >> 16) & 0xff) << 16) ^ (sboxplace((key1[3] >> 8) & 0xff) << 8) ^ sboxplace((key1[3]) & 0xff);
		key1[0] = key1[0] ^ key1[3];
		key1[1] = key1[1] ^ key1[0];
		key1[2] = key1[2] ^ key1[1];
		key1[3] = key1[3] ^ key1[2];
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				key2[target][target1][target2] = (key1[target1] >> 8 * target2) & 0xff;
				printf("%x", key2[target][target1][target2]);
			}
			printf("\n");
		}
		printf("\n");
	}
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
void hangbianhuan(int a)
{
	int target;
	unsigned char key3[4] = { 0 };
	key3[0] = password[1][0];
	key3[1] = password[1][1];
	key3[2] = password[1][2];
	key3[3] = password[1][3];
	password[1][3] = key3[0];
	password[1][0] = key3[1];
	password[1][1] = key3[2];
	password[1][2] = key3[3];
	key3[0] = password[2][0];
	key3[1] = password[2][1];
	key3[2] = password[2][2];
	key3[3] = password[2][3];
	password[2][2] = key3[0];
	password[2][3] = key3[1];
	password[2][0] = key3[2];
	password[2][1] = key3[3];
	key3[0] = password[3][0];
	key3[1] = password[3][1];
	key3[2] = password[3][2];
	key3[3] = password[3][3];
	password[3][1] = key3[0];
	password[3][2] = key3[1];
	password[3][3] = key3[2];
	password[3][0] = key3[3];
}
void nihangbianhuan(int a)
{
	int target;
	unsigned char key3[4] = { 0 };


	key3[0] = password[1][0];
	key3[1] = password[1][1];
	key3[2] = password[1][2];
	key3[3] = password[1][3];
	password[1][1] = key3[0];
	password[1][2] = key3[1];
	password[1][3] = key3[2];
	password[1][0] = key3[3];

	key3[0] = password[2][0];
	key3[1] = password[2][1];
	key3[2] = password[2][2];
	key3[3] = password[2][3];
	password[2][2] = key3[0];
	password[2][3] = key3[1];
	password[2][0] = key3[2];
	password[2][1] = key3[3];

	key3[0] = password[3][0];
	key3[1] = password[3][1];
	key3[2] = password[3][2];
	key3[3] = password[3][3];
	password[3][3] = key3[0];
	password[3][0] = key3[1];
	password[3][1] = key3[2];
	password[3][2] = key3[3];
}
void liebianhuan(int a)
{
	unsigned char t[4][4];
	int target, target1, target2;
	unsigned char x = 0x2;
	for (target = 0; target < 4; target++)
	{
		t[0][target] = multwo(password[0][target], x) ^ multwo(password[1][target], x + 1) ^ password[2][target] ^ password[3][target];
		t[1][target] = multwo(password[1][target], x) ^ multwo(password[2][target], x + 1) ^ password[0][target] ^ password[3][target];
		t[2][target] = multwo(password[2][target], x) ^ multwo(password[3][target], x + 1) ^ password[0][target] ^ password[1][target];
		t[3][target] = multwo(password[3][target], x) ^ multwo(password[0][target], x + 1) ^ password[2][target] ^ password[1][target];
	}
	for (target1 = 0; target1 < 4; target1++)
	{
		for (target2 = 0; target2 < 4; target2++)
		{
			password[target1][target2] = t[target1][target2];
			//printf("%02x ", password[target1][target2]);
		}
	}
}
void niliebianhuan(int a)
{
	unsigned char t[4][4];
	int target, target1, target2;
	unsigned char x1, x2, x3 , x4;
	for (target = 0; target < 4; target++)
	{
		x1 = 0x0e;
		x2 = 0x0b;
		x3 = 0x0d;
		x4 = 0x09;
		t[0][target] = multwo(password[0][target], x1) ^ multwo(password[1][target], x2) ^ multwo(password[2][target], x3) ^ multwo(password[3][target], x4);
		x1 = 0x09;
		x2 = 0x0e;
		x3 = 0x0b;
		x4 = 0x0d;
		t[1][target] = multwo(password[0][target], x1) ^ multwo(password[1][target], x2) ^ multwo(password[2][target], x3) ^ multwo(password[3][target], x4);
		x1 = 0x0d;
		x2 = 0x09;
		x3 = 0x0e;
		x4 = 0x0b;
		t[2][target] = multwo(password[0][target], x1) ^ multwo(password[1][target], x2) ^ multwo(password[2][target], x3) ^ multwo(password[3][target], x4);
		x1 = 0x0b;
		x2 = 0x0d;
		x3 = 0x09;
		x4 = 0x0e;
		t[3][target] = multwo(password[0][target], x1) ^ multwo(password[1][target], x2) ^ multwo(password[2][target], x3) ^ multwo(password[3][target], x4);
	}
	for (target1 = 0; target1 < 4; target1++)
	{
		for (target2 = 0; target2 < 4; target2++)
		{
			password[target1][target2] = t[target1][target2];
			//printf("%02x ", password[target1][target2]);
		}
	}
}
int main() {
	int target, target2, target1;
	unsigned int rcon[10] = { 0x1000000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000,0x80000000,0x1b000000,0x36000000 };
	key1[0] = 0x00000000;
	key1[1] = 0x00000000;
	key1[2] = 0x00000000;
	key1[3] = 0x00000000;
	unsigned int key4;
	for (target = 1; target < 11; target++)
	{
		key4 = key1[3];
		key1[3] = ((key1[3] >> 24) & 0xff) ^ ((key1[3] << 8) & 0xffffff00);
		key1[3] = (sboxplace((key1[3] >> 24) & 0xff) << 24) ^ (sboxplace((key1[3] >> 16) & 0xff) << 16) ^ (sboxplace((key1[3] >> 8) & 0xff) << 8) ^ sboxplace((key1[3]) & 0xff);
		key1[3] = key1[3] ^ rcon[target - 1];
		key1[0] = key1[0] ^ key1[3];
		key1[1] = key1[1] ^ key1[0];
		key1[2] = key1[2] ^ key1[1];
		key1[3] = key4 ^ key1[2];
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				key2[target][target2][target1] = (key1[target1] >> (24 - 8 * target2)) & 0xff;
			}
		}
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				//printf("%x ", key2[target][target1][target2]);
			}
		}
		//printf("\n");
	}
	for (target1 = 0; target1 < 4; target1++)
	{
		for (target2 = 0; target2 < 4; target2++)
		{
			printf("%x ", password[target1][target2]);
			//printf("%x ", key2[target][target1][target2]);
		}
		printf("\n");
	}
	int i;
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				password[target1][target2] = password[target1][target2] ^ key2[0][target1][target2];
			}
		}
		for (target = 0; target < 9; target++)
		{
			for (target1 = 0; target1 < 4; target1++)
			{
				for (target2 = 0; target2 < 4; target2++)
				{
					password[target1][target2] = sboxplace(password[target1][target2]);
				}
			}
			hangbianhuan(target);
			liebianhuan(target);
			for (target1 = 0; target1 < 4; target1++)
			{
				for (target2 = 0; target2 < 4; target2++)
				{
					password[target1][target2] = password[target1][target2] ^ key2[target + 1][target1][target2];
				}
			}
		}
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				password[target1][target2] = sboxplace(password[target1][target2]);
			}
		}
		hangbianhuan(target);
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				password[target1][target2] = password[target1][target2] ^ key2[10][target1][target2];
			}
		}
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
					printf("%x  ", password[target1][target2]);
			}
			printf("\n");
		}
//解密
		printf("解密\n");
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				password[target1][target2] = password[target1][target2] ^ key2[10][target1][target2];
			}
		}
		nihangbianhuan(target);
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				password[target1][target2] = nisboxplace(password[target1][target2]);
			}
		}
		for (target = 0; target < 9; target++)
		{
			for (target1 = 0; target1 < 4; target1++)
			{
				for (target2 = 0; target2 < 4; target2++)
				{
					password[target1][target2] = password[target1][target2] ^ key2[9 - target][target1][target2];
				}
			}
			niliebianhuan(target);
			nihangbianhuan(target);
			for (target1 = 0; target1 < 4; target1++)
			{
				for (target2 = 0; target2 < 4; target2++)
				{
					password[target1][target2] = nisboxplace(password[target1][target2]);
				}
			}
		}
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				password[target1][target2] = password[target1][target2] ^ key2[0][target1][target2];
			}
		}
		for (target1 = 0; target1 < 4; target1++)
		{
			for (target2 = 0; target2 < 4; target2++)
			{
				printf("%x  ", password[target1][target2]);
			}
			printf("\n");
		}
	return 0;
}
