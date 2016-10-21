#ifndef __CRYPTO_H__
#define __CRYPTO_H__
uchar *MD5_encode(uchar *in, int inlen, uchar *out, int olen);
char *MD5_encode_str(char *in, int inlen, char *out, int olen, int upper_case);
#endif
