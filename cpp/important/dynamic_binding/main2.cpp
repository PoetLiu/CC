#include <iostream>

using namespace std;

class base {
public:
	virtual void print() const { cout << "base::f()" << endl;}
};

class derived : public base {
public:
	virtual void print() const { cout << "derived::f()" << endl;}
};

void print(const base &obj)
{
	obj.print();
}

int main(void)
{
	base *pb1 = new derived();
	print(*pb1);

	base *pb2 = new base(*pb1);
	print(*pb2);

	base b = base(derived());
	print(b);

	delete pb1;
	delete pb2;

	return 0;
}
