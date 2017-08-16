#include <iostream>

void fun(int i)
{
	static  int value = i++;
	std::cout << value;
}

int main(void)
{
	fun(0);
	fun(1);
	fun(2);

	return 0;

}
