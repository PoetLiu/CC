#include <iostream>
// deleted function: default constructor can not initialize const and reference member
class X
{
public:
//	int &x; // error: reference member ‘X::x’ should be initialized
//	const int x1; // error: const member 'X::x1' should be initialized
	int x2;
	static int x3;
};

int X::x3;

using namespace std;

X x1;
int main()
{
	X x;

	cout << "x2:" << x.x2 << endl
		<< "x3:" << x.x3 << endl;

	cout << "x2:" << x1.x2 << endl
		<< "x3:" << x1.x3 << endl;
	return 0;
}

