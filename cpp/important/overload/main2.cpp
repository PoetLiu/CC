#include <iostream>

void fun(int) { std::cout << "int\n";}
void fun(int*) { std::cout << "int\n*";}

int main(void)
{
	fun(0);
	return 0;
}
