#include <iostream>
using namespace std;

class Integer {
public:
	Integer() : m_lng(100) {}
	long m_lng;
};

void test(Integer *l)
{
	cout << l->m_lng << endl;
	return;
}

int main(void)
{
	const long lng = 10;
	long *pl = (long *)&lng;
	*pl	= 1000;

	cout << *pl << endl;
	cout << lng << endl;

	const Integer int_l;
	Integer *pInt = (Integer *)&int_l;
	pInt->m_lng	= 1000;
	cout << pInt->m_lng << endl;
	cout << int_l.m_lng << endl;
	test(pInt);

	return 0;
}

