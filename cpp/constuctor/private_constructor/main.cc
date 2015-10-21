#include <iostream>

class A {
public:
	//A ();
	const char *GetType();
private:
	A& operator = (const A &a);
	A (const A &a);
};

A::A()
{
}

A::A (const A &a)
{
}

void A(void)
{
}

int main(void)
{
	class A a;
	class A b(a);
	return 0;
}
