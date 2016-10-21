#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include "mylib.h"

uchar *MD5_encode(uchar *in, int inlen, uchar *out, int olen)
{
        MD5_CTX ctx;
	P_VALID_RET(in, NULL);
	P_VALID_RET(out, NULL);
        MD5_Init(&ctx);
        MD5_Update(&ctx, in, inlen);
        MD5_Final(out, &ctx);
	return out;
}

char *MD5_encode_str(char *in, int inlen, char *out, int olen, int upper_case)
{
	uchar out_bin[16] = {0};
	int i = 0;

	P_VALID_RET(in, NULL);
	P_VALID_RET(out, NULL);
	if (olen < sizeof(out_bin)*2+1) {
		DEBUG("obuf too low, len:%d, at least:%ld\n", olen, sizeof(out_bin)*2+1);
		return NULL;
	}
	MD5_encode((uchar *)in, inlen, out_bin, sizeof(out_bin));
	for (i = 0; i < sizeof(out_bin); i++) {
		sprintf(out + 2*i, upper_case ? "%02X" : "%02x", (uchar)out_bin[i]);
	}
	DEBUG("in:%s inlen:%d out:%s olen:%d\n", in, inlen, out, olen);
	return out;
}

int aes_cbc_128_encrypt(const char *in, unsigned char *ou, const unsigned char *secret_key, unsigned char *p_iv)
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
	AES_set_encrypt_key(tmp_key, 128, &aes_key);
	AES_cbc_encrypt(pad_data, ou, in_len+i_padding, &aes_key, iv, AES_ENCRYPT);

	SAFE_FREE(pad_data);
	return (in_len+i_padding);
}

