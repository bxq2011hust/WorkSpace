#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include "sm3.h"

using namespace std;
// https://zhuanlan.zhihu.com/p/43865231

void BiToW(unsigned int Bi[], unsigned int W[])
{
	int i;
	unsigned int tmp;
	for (i = 0; i <= 15; i++)
	{
		W[i] = Bi[i];
	}
	for (i = 16; i <= 67; i++)
	{
		tmp = W[i - 16] ^ W[i - 9] ^ SM3_rotl32(W[i - 3], 15);
		W[i] = SM3_p1(tmp) ^ (SM3_rotl32(W[i - 13], 7)) ^ W[i - 6];
	}
}

void WToW1(unsigned int W[], unsigned int W1[])
{
	int i;
	for (i = 0; i <= 63; i++)
	{
		W1[i] = W[i] ^ W[i + 4];
	}
}

void CF(unsigned int W[], unsigned int W1[], unsigned int V[])
{
	unsigned int SS1;
	unsigned int SS2;
	unsigned int TT1;
	unsigned int TT2;
	unsigned int A, B, C, D, E, F, G, H;
	unsigned int T = SM3_T1;
	unsigned int FF;
	unsigned int GG;
	int j;
	//reg init,set ABCDEFGH=V0
	A = V[0];
	B = V[1];
	C = V[2];
	D = V[3];
	E = V[4];
	F = V[5];
	G = V[6];
	H = V[7];
	for (j = 0; j <= 63; j++)
	{
		//SS1
		if (j == 0)
		{
			T = SM3_T1;
		}
		else if (j == 16)
		{
			T = SM3_rotl32(SM3_T2, 16);
		}
		else
		{
			T = SM3_rotl32(T, 1);
		}
		SS1 = SM3_rotl32((SM3_rotl32(A, 12) + E + T), 7);
		//SS2
		SS2 = SS1 ^ SM3_rotl32(A, 12);
		//TT1
		if (j <= 15)
		{
			FF = SM3_ff0(A, B, C);
		}
		else
		{
			FF = SM3_ff1(A, B, C);
		}
		TT1 = FF + D + SS2 + *W1;
		W1++;
		//TT2
		if (j <= 15)
		{
			GG = SM3_gg0(E, F, G);
		}
		else
		{
			GG = SM3_gg1(E, F, G);
		}
		TT2 = GG + H + SS1 + *W;
		W++;
		//D
		D = C;
		//C
		C = SM3_rotl32(B, 9);
		//B
		B = A;
		//A
		A = TT1;
		//H
		H = G; //G
		G = SM3_rotl32(F, 19);
		//F
		F = E;
		//E
		E = SM3_p0(TT2);
	}
	//update V
	V[0] = A ^ V[0];
	V[1] = B ^ V[1];
	V[2] = C ^ V[2];
	V[3] = D ^ V[3];
	V[4] = E ^ V[4];
	V[5] = F ^ V[5];
	V[6] = G ^ V[6];
	V[7] = H ^ V[7];
}

void BigEndian(unsigned char src[], unsigned int bytelen, unsigned char des[])
{
	unsigned char tmp = 0;
	unsigned int i = 0;
	for (i = 0; i < bytelen / 4; i++)
	{
		tmp = des[4 * i];
		des[4 * i] = src[4 * i + 3];
		src[4 * i + 3] = tmp;
		tmp = des[4 * i + 1];
		des[4 * i + 1] = src[4 * i + 2];
		des[4 * i + 2] = tmp;
	}
}

void SM3_init(SM3_STATE *md)
{
	md->curlen = md->length = 0;
	md->state[0] = SM3_IVA;
	md->state[1] = SM3_IVB;
	md->state[2] = SM3_IVC;
	md->state[3] = SM3_IVD;
	md->state[4] = SM3_IVE;
	md->state[5] = SM3_IVF;
	md->state[6] = SM3_IVG;
	md->state[7] = SM3_IVH;
}

void SM3_compress(SM3_STATE *md)
{
	unsigned int W[68];
	unsigned int W1[64];
	//if CPU uses little-endian, BigEndian function is a necessary call
	BigEndian(md->buf, 64, md->buf);
	BiToW((unsigned int *)md->buf, W);
	WToW1(W, W1);
	CF(W, W1, md->state);
}

void SM3_process(SM3_STATE *md, unsigned char *buf, int len)
{
	while (len--)
	{
		/* copy byte */
		md->buf[md->curlen] = *buf++;
		md->curlen++;
		/* is 64 bytes full? */
		if (md->curlen == 64)
		{
			SM3_compress(md);
			md->length += 512;
			md->curlen = 0;
		}
	}
}

void SM3_done(SM3_STATE *md, unsigned char hash[])
{
	int i;
	unsigned char tmp = 0;
	/* increase the bit length of the message */
	md->length += md->curlen << 3;
	/* append the '1' bit */
	md->buf[md->curlen] = 0x80;
	md->curlen++;
	/* if the length is currently above 56 bytes, appends zeros till
	   it reaches 64 bytes, compress the current block, creat a new
	   block by appending zeros and length,and then compress it
	   */
	if (md->curlen > 56)
	{
		for (; md->curlen < 64;)
		{
			md->buf[md->curlen] = 0;
			md->curlen++;
		}
		SM3_compress(md);
		md->curlen = 0;
	}
	/* if the length is less than 56 bytes, pad upto 56 bytes of zeroes */
	for (; md->curlen < 56;)
	{
		md->buf[md->curlen] = 0;
		md->curlen++;
	}
	/* since all messages are under 2^32 bits we mark the top bits zero */
	for (i = 56; i < 60; i++)
	{
		md->buf[i] = 0;
	}
	/* append length */
	md->buf[63] = md->length & 0xff;
	md->buf[62] = (md->length >> 8) & 0xff;
	md->buf[61] = (md->length >> 16) & 0xff;
	md->buf[60] = (md->length >> 24) & 0xff;
	SM3_compress(md);
	/* copy output */
	memcpy(hash, md->state, SM3_len / 8);
	BigEndian(hash, SM3_len / 8, hash); //if CPU uses little-endian, BigEndian function is a necessary call
}

void SM3_256(unsigned char buf[], int len, unsigned char hash[])
{
	SM3_STATE md;
	SM3_init(&md);
	SM3_process(&md, buf, len);
	SM3_done(&md, hash);
}

int main(int argc, char **argv)
{
	unsigned char hash[32];
	SM3_256((unsigned char *)argv[0], 128, hash);
	int i;
	for (int i = 12; i < 32; i++)
	{
		printf("%02x", hash[i]);
	}
	return 0;
}
