/*
 * sizeof X:8
 * Y:8
 * Z:7
 * R:20
 * T:24
 * U:28
 * offset of m_width in R:4, m_name:12
 *
 * */
#include <stdio.h>
#include <stddef.h>

struct X
{
	char m_ch;
	char *m_pStr;
};

struct Y 
{
	char m_ch;
	int m_count;
};

struct Z
{
	bool m_ok;
	char m_name[6];
};

struct R
{
	char m_ch;
	double m_width;
	char m_name[6];
};

struct T
{
	int m_no;
	R m_r;
};

struct U
{
	bool m_ok;
	T m_t;
};

int main(void)
{
	printf("sizeof X:%d\nY:%d\nZ:%d\nR:%d\nT:%d\nU:%d\n",
		sizeof(struct X),
		sizeof(struct Y),
		sizeof(struct Z),
		sizeof(struct R),
		sizeof(struct T),
		sizeof(struct U)
		);
	printf("offset of m_width in R:%d, m_name:%d\n", 
		offsetof(R, m_width),
		offsetof(R, m_name)
		);
}
