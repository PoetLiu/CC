/*
 * Funcion	: a demo of aes_cbc_128 pkcs5padding encrypt.
 * Author	: NULL
 * arguments	:
 * return	:
**/
#include <stdio.h>
#include <openssl/aes.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG(fmt, args...)	do {printf("[%s-%s-%d]:"fmt,__FILE__,__FUNCTION__,__LINE__,##args);} while(0)
#define SAFE_FREE(p)		{if (p){free(p);p=NULL;}}
#define AES_IV_BUF 		"360luyou@install"

static unsigned char num_2_hex_char(unsigned char inch)
{
        unsigned char hex = 0;
        if(inch >= 0 && inch <= 9) {
                hex = inch + '0';
        } else {
                switch(inch) {
                        case 10: 
                                hex = 'a';
                                break;
                        case 11: 
                                hex = 'b';
                                break;
                        case 12: 
                                hex = 'c';
                                break;
                        case 13: 
                                hex = 'd';
                                break;
                        case 14: 
                                hex = 'e';
                                break;
                        case 15: 
                                hex = 'f';
                                break;
                        default:
                                return -1; 

                }   
        }
        return hex;
}

static int num_2_hex_str(unsigned char *in_str, unsigned char *out_str, int len)
{
        unsigned char cha = 0,hex1 = 0,hex2 = 0;
        int i = 0,j = 0;
        while(i<len*2) {
                cha	= in_str[j++];
                hex1	= (cha & 0xf0) >> 4;
                hex2	= cha & 0x0f;
                out_str[i]	= num_2_hex_char(hex1);
                out_str[i+1]	= num_2_hex_char(hex2);
                if(out_str[i] == (unsigned char)-1 || out_str[i+1] == (unsigned char)-1) {
                        return -1;
                }
                i	= i+2;
        }
        return j;
}

static unsigned char hex_char_2_num(char inch)
{
	unsigned char hex = 0;
	if(inch >= '0' && inch <= '9') {
		hex = inch - '0';
	} else {
		switch(inch) {
			case 'a':
			case 'A':
				hex = 0x0a;
				break;
			case 'b':
			case 'B':
				hex = 0x0b;
				break;
			case 'c':
			case 'C':
				hex = 0x0c;
				break;
			case 'd':
			case 'D':
				hex = 0x0d;
				break;
			case 'e':
			case 'E':
				hex = 0x0e;
				break;
			case 'f':
			case 'F':
				hex = 0x0f;
				break;
			default:
				return -1;
		}
	}
	return hex;
}

static int hex_str_2_num(char *in_str, char *out_str, int len)
{
	unsigned char hex1 = 0;
	unsigned char hex2 = 0;
	int i = 0,j = 0;
	if (len%2) {
		return -1;
	}
	while (i < len) {
		hex1	= hex_char_2_num(in_str[i]);
		hex2	= hex_char_2_num(in_str[i+1]);
		if (hex1 == (unsigned char)-1 || hex2 == (unsigned char)-1) {
			return -1;
		}
		i	= i+2;
		out_str[j++]	= (hex1 << 4) | hex2;
	}
	return j;
}

static int aes_cbc_128_encrypt(const char *in, unsigned char *ou, const unsigned char *secret_key, unsigned char *p_iv)
{
	int i_padding = 0, in_len = 0;
	unsigned char iv[16] = {0};
	AES_KEY  aes_key;
	char * pad_data = NULL;
	char tmp_key[64] = {0};

	if (!in || !ou || !secret_key || !p_iv)
		return -1;

	memset(&aes_key, 0, sizeof(aes_key));

	//Pkcs5Padding
	in_len		= strlen(in);
	i_padding	= AES_BLOCK_SIZE-in_len%AES_BLOCK_SIZE;
	pad_data	= (char *)malloc(in_len + i_padding + 1);
	if (!pad_data)
		return -2;
	memset(pad_data, 0, in_len+i_padding+1);
	strcpy(pad_data, in);
	memset(pad_data+in_len, i_padding, i_padding);//Padding

	memcpy(tmp_key, secret_key, 16);
	memset(&aes_key, 0, sizeof(aes_key));
	memset(iv, 0, sizeof(iv));
	memcpy(iv, p_iv, sizeof(iv));
	AES_set_encrypt_key((const unsigned char *)tmp_key, 128, &aes_key);
	AES_cbc_encrypt((const unsigned char *)pad_data, ou, in_len+i_padding, &aes_key, iv, AES_ENCRYPT);

	SAFE_FREE(pad_data);
	return (in_len+i_padding);
}

/*
 * 使用：
 * demo名	  加密数据	      密钥
 * ./encrypt_test 12345678 a36a1b37f64873b4541d36e59c77142b
 * 输出：
 * [encrypt_test.c-main-183]:3e49173aba6ecab35cf2524e1e529ab8
**/
int main(int argc,  char *argv[])
{
	unsigned char encrypt[256] = {0};
	unsigned char out[512] = {0};
	int encrypt_len = 0;
	unsigned char iv[17] = {0};
	unsigned char secret_key[256] = {0};

	if (argc < 3) {
		DEBUG("usage:encrypt_test in secret_key\n");	
		return -1;
	}
	strcpy((char *)iv, AES_IV_BUF);
	
	hex_str_2_num(argv[2], (char *)secret_key, sizeof(secret_key));
	encrypt_len = aes_cbc_128_encrypt(argv[1], encrypt, secret_key, iv);
	num_2_hex_str(encrypt, out, encrypt_len);
	
	printf("%s", out);

	return 0;
}
