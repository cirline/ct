#include <stdexcept>

#include <cpplang/vector.h>

using namespace std;
using namespace cpplang;

vector::vector(int n) : elem{new double[n]}, sz{n} {
	/* nothing */
}

double &vector::operator[](int n) {
	if(n < 0 || n >= size())
		throw std::out_of_range{"vector::operator[]"};

	return elem[n];
}

int vector::size() {
	return sz;
}

