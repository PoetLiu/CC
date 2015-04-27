#include <iostream>
#include <exception>
#include <string>

using namespace std;

void ex()
{
	string *s;
	//throw exception("exception");
	throw s;
}

void fun()
{
	try {
		ex();
	} catch (exception *) {
		cout << "exception*" << endl;
	} catch (exception &) {
		cout << "exception&" << endl;
	} catch (...) {
		cout << "..." << endl;
	}
}

int main(void)
{
	fun();
	return 0;
}
