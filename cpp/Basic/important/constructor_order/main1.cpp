#include <iostream>

using namespace std;
class A{
public:
	A() {cout<<"A::A()"<<endl;}
	~A() {cout<<"A::~A()"<<endl;}  
};  

class B{
public:
	B(){cout<<"B::B()"<<endl;}
	~B(){cout<<"B::~B()"<<endl;}  
};  

class C:public B{
public:
	C():a(A()),b(B()){}
	~C(){}
private:
	B b;
	A a;  
};

int main()
{
	C c;
} 
