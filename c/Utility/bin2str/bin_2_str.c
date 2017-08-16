#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bin_2_str.h"

char * bin_2_str(uchar *bin, int len)
{
        char *str;
        int     i, slen;

        slen    = len * 2 + 1;
        str     = (char *)malloc(sizeof(char) * slen);

        memset(str, 0, 2*len + 1);

        for (i = 0; i < len; i++)
                snprintf(&str[2*i], slen - 2*i, "%X", bin[i]);
        return str;
}

