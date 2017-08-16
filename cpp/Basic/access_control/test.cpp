#include <iostream>

class base
{
public:
	// friends class can access protected private public members
	friend void test0(int x, base& b) { b.p = x; b.p1 = x; }
protected:
	int p;
private:
	int p1;
};

class dervived : public base
{

public:
	// 友元和派生类只能通过派生类去访问基类中的protected成员
	// ok 通过dervived 访问p	
	friend void test(int x, dervived &d) { d.p = x; } 
	friend int get_p(dervived &d) { return d.p; }


	// error 通过base访问p, test不是base的友元	
	// friend void test(int x, base &b) { b.p = x; } 


	// ok 隐式地访问this所指向的dervived类的p， 通过dervived 访问p
	void test1(int x) { p = x; }
	int get_p1() {return p;}

	// error 通过base访问p, 没有权限
	// void test1(int x, base &) { base.p = x; }
};

// error, dervived继承了base::p, 为protected成员， 不能被用户访问
// 只能在友元中或者dervived内部通过dervived对象访问
/*
void test2(int x, dervived & d)
{
	d.p = x;
}
*/

using namespace std;

int main(void)
{
	dervived d;
	base b;
	test0(0, b);
	d.test1(10);
	cout << d.get_p1() << endl;
	return 0;
}

