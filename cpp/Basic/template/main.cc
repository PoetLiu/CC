#include <iostream>

using namespace std;

template <typename T1, typename T2> 
class handle
{
public:
	typedef T1(*dataFunc)(T1, T2);
	T1 t;
	static T1 data;

	static dataFunc char_output(char op) {
		switch(op) {
		case '+':
			return pluse;
			break;
		case '-':
			return minus;
			break;
		default:
			return NULL;
			break;
		}	
	}

protected:
	explicit handle();
	
private:
	static T1 pluse(T1 lh, T2 rh) {
		return (T1)(lh + rh);	
	}

	static T1 minus(T1 lh, T2 rh) {
		return (T1)(lh - rh);	
	}
};

template <> int handle<int, int>::data = 2;

int main(void)
{

	handle<int, int>::dataFunc pDataFunc = handle<int,int>::char_output('+');
	cout << pDataFunc(1, 2) << endl << handle<int, int>::data << endl;
	return 0;
}
