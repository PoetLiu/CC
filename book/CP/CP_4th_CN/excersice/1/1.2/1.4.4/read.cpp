#include <iostream>

int main()
{
	int	sum = 0, value;
	// read till end-of-file, calculating a running total of all values read
	while (std::cin >> value) 
		sum += value;
	std::cout << "The sum of all values is " << sum << std::endl;
}
