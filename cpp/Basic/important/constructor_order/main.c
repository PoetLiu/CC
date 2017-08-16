#include <iostream>

class A 
{
public:
	A() { std::cout << "A::A()\n"; }
	~A() { std::cout << "A::~A()\n"; }
};

class B
{
public:
	B() { std::cout << "B::B()\n"; }
	~B() { std::cout << "B::~B()\n"; }
};

class C : public B
{
	A a;
public:
	C() { std::cout << "C::C()\n"; }
	~C() { std::cout << "C::~C()\n"; }
};

A a;

int main(void)
{
	C c;
}
