#include <stdio.h>
#include <iostream>

using namespace std;

class TestArrage {
	public:
		long m_lng;
		char m_ch1;
		TestArrage() {
			m_lng	= 0;
			m_ch1	= 'a';
			m_int	= 0;
			m_ch2	= 'a';
		}

		const int * GetIntAddr() {return &m_int;}
		const char * GetChar2Addr() {return &m_ch2;}

	private:
		int m_int;
		char m_ch2;
};

int main(void)
{
	TestArrage test;
	cout << "Address of test object:" << &test << endl;
	cout << endl;
	cout << "Address of m_lng:" << &(test.m_lng) << endl;
	printf("Address of m_ch1:%p\n", &(test.m_ch1));
	cout << "Address of m_int:" << test.GetIntAddr() << endl;
	cout << "Address of m_ch2:" << (void *)test.GetChar2Addr() << endl;

	return 0;
}
