#include <iostream>

using namespace std;

class base
{
public:
	virtual void test() {
		cout << "I am in base" << endl;
	}
	void run() {
		test();
	}
protected:
	static base * pbase;
};

base * base::pbase = 0;

class derive : public base
{
public:
	derive() {
		// 动态绑定
		pbase = this;
	}

	static base * getInstance() {
		return pbase;
	}

	virtual void  test() {
		cout << "I am in derive" << endl;
	}	
};

class derive1 : public base
{
public:
	derive1() {
		// 动态绑定
		pbase = this;
	}

	static base * getInstance() {
		return pbase;
	}

	virtual void  test() {
		cout << "I am in derive1" << endl;
	}	
};

int main(void)
{
	derive d;
	derive::getInstance()->run();

	derive1 d1;
	derive1::getInstance()->run();

	return 0;
}
