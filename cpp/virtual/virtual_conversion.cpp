#include <iostream>

using namespace std;

class Base
{
	public:Base(){}
	public:
	       virtual void print();
	       //virtual void print(){cout << "Base" << endl;}
};

class Derived : public Base
{
	public:Derived(){}
	public:
	       void print(){cout << "Derived" << endl;}
};

int main(void)
{
	Base *p = new Derived();
	p->print();
	delete p;

	Base *p1 = new Base();
	p1->print();
	delete p1;

	Derived *p2 = new Derived();
	p2->print();
	delete p2;
	return 0;
}
