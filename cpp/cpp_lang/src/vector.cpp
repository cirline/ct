#include <stdexcept>

#include <cpplang/vector.h>

using namespace std;
using namespace cpplang;

vector::vector(int n) : elem{new double[n]}, sz{n} {
	/* nothing */
	char *p = "vector info";
	class_info = p;
}

vector::vector(initializer_list<double> list) :
	elem{new double[list.size()]},
	sz{(int)list.size()}
{
	copy(list.begin(), list.end(), elem);
	char *p = "vector info";
	class_info = p;
}

double &vector::operator[](int n) {
	if(n < 0 || n >= size())
		throw std::out_of_range{"vector::operator[]"};

	return elem[n];
}

int vector::size()
{
	return sz;
}

char *vector::to_string()
{
	return class_info;
}

