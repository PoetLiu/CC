#include <stdio.h>
#include <stdlib.h>

static int version = 1;

int isBadVersion(int ver)
{
	return ver >= version;
}

int binary_chop(int start, int end)
{
    int tmp = (end-start)/2 + start;
    if (tmp == start) 
        return isBadVersion(start) ? start : end;
    
    if (isBadVersion(tmp)) {
        if (isBadVersion(tmp - 1))
            return binary_chop(start, tmp - 1);
        else
            return tmp;
    } else{
        return binary_chop(tmp + 1, end);
    }
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("usage: %s ver ver_num\n", argv[0]);
		return -1;
	}
	version	= atoi(argv[1]);
	return binary_chop(1, atoi(argv[2]));
}

