#include <iostream>

using namespace std;
const char *hello() { return "Hello world"; }

int main(void)
{
	const char *pc = hello();
	cout << pc;

	return 0;
}
