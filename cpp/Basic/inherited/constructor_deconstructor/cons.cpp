#include <iostream>

using namespace std;

class base
{
public:
	base() {
		cout << "base constructor" << endl;
	}	

	~base() {
		cout << "base deconstructor" << endl;
	}
};

class derive : public base 
{
public:
	derive() {
		cout << "derive constructor" << endl;
	}

	~derive() {
		cout << "derive deconstructor" << endl;
	}
};

int main(void)
{
	derive d;
	return 0;
}
