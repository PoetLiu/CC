#include <iostream>
using namespace std;

#define MAX_ROW		500
#define MAX_COLUMN	10000

typedef unsigned long DWORD;

unsigned long GetTickCount()
{
	struct timespec ts;
     	clock_gettime(CLOCK_MONOTONIC, &ts);
     	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

int main(void)
{
	int (*a)[MAX_COLUMN]	= new int [MAX_ROW][MAX_COLUMN];
	std::cerr << "int array(MAX_ROW = " << MAX_ROW <<	\
		", MAX_COLUMN = " << MAX_COLUMN << ")\n";

	DWORD start = GetTickCount();
	for (int i = 0; i < MAX_ROW; i++)
		for (int j = 0; j < MAX_COLUMN; j++)
			a[i][j]	= 1;
	DWORD end = GetTickCount();
	std::cerr << "Stepthrough by row-first-column-last takes "	\
		<< (end - start) << "msec\n";

	start = GetTickCount();
	for (int j = 0; j < MAX_COLUMN; j++)
		for (int i = 0; i < MAX_ROW; i++)
			a[i][j]	= 1;
	end = GetTickCount();
	std::cerr << "Stepthrough by column-first-row-last takes "	\
		<< (end - start) << "msec\n";

	delete []a;
	return 0;
}
