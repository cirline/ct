#include <iostream>

#include "cpplang/cl_vector.h"

using namespace std;

/* reference argument */
void ref_init(int &r, int n)
{
	r = n;
}

/* 2.3.2 class
 */
class Vector {
	public:
		/* init elem and sz by initializer list
		 * a initializer list is
		 *	: assignment-expr, assignment-expr, ...
		 * */
		Vector(int size) : elem{new double[size]}, sz{size} {
			/* nothing */
		}

		~Vector() {
			//delete elem;
		}

		/* overide operator [] */
		double &operator[](int i) {
			return elem[i];
		}

		int size() {
			return sz;
		}
	private:
		double *elem;
		int sz;
};

/* 2.3.3 enumerations
 */
enum class Color {red, green, blue};

int main()
{
	cout << "hello world\n";

	/* 2.2.2 types */
	cout << "sizeof(bool) = " << sizeof(bool) << "\n";
	cout << "sizeof(char) = " << sizeof(char) << "\n";
	cout << "sizeof(int) = " << sizeof(int) << "\n";
	cout << "sizeof(float) = " << sizeof(float) << "\n";
	cout << "sizeof(double) = " << sizeof(double) << "\n";

	/* 2.2.3 constants
	 * const: value cannot be changed
	 * constexpr: expr will be evaluated at compile time
	 */
	const int cint = 1;
	cout << "cint = " << cint << "\n";
	constexpr int cexpr = 8 + cint;
	cout << "cexpr = " << cexpr << "\n";

	/* 2.2.5 pointer, arrays and loops
	 */
	int v[] = {1, 2, 3, 4, 5};
	for(auto i : v) {
		cout << "v = " << i <<  "\n";
	}
	/* declarator operators
	 * T a[n]; a is T type n size array
	 * T *p; p is a T pointer
	 * T &r; r is a T reference
	 * T f(A); f is a function which taking an A argument and return T type
	 *
	 * pointer and reference
	 * 1. reference don't need * prefix * to access the value referred
	 * 2. reference can't be made to refer to a different object after its initialization
	 * 3. reference use . to use member
	 */
	for(auto &j : v) {
		cout << "j = " << j << "\n";
	}

	int a = 3;
	cout << "before refer, a = " << a << endl;
	ref_init(a, 8);
	cout << "after refer, a = " << a << endl;

	/* test class */
	Vector my_vector(7);
	cout << "vector size = " << my_vector.size() << endl;

	/* test enumeration */
	Color color = Color::blue;
	switch(color) {
		case Color::red:
			cout << "color = red" << endl;
			break;
		case Color::green:
			cout << "color = green" << endl;
			break;
		case Color::blue:
			cout << "color = blue" << endl;
			break;
	}

	/*
	 * modularity
	 */
	cl_vector clv(9);
	cout << "clv.size = " << clv.size() << endl;

	/* 2.2.4 test and loops
	 */
	cout << "input a character: ";
	char in;
	cin >> in;

	if(in >= 0x30 && in <= 0x39)
		cout << "in is a number = " << in << "\n";
	else
		cout << "is is a char = " << in << "\n";

	return 0;
}
