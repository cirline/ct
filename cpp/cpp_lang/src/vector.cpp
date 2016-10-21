#include <stdexcept>
#include <sstream>
#include <iostream>

#include <cpplang/vector.h>

using namespace std;
using namespace cpplang;

vector::vector(int n) : elem{new double[n]}, sz{n} {
	/* nothing */
	descriptor = "vector descriptor";
}

vector::vector(initializer_list<double> list) :
	elem{new double[list.size()]},
	sz{(int)list.size()}
{
	copy(list.begin(), list.end(), elem);
	descriptor = "vector descriptor";
}

/**
 * copy object, by constructor
 * also use operator= to copy
 */
vector::vector(const vector &v) : elem{new double[v.size()]}, sz{v.size()}
{
	cout << "uninitial size = " << size() << endl;
	cout << "v size = " << v.size() << endl;
	//vector(v.size());
	cout << "initial size = " << size() << endl;
	for(int i = 0; i < v.size(); i++) {
		elem[i] = v.elem[i];
	}
}

/**
 * copy object, by assignment
 * also use constructor to copy
 */
vector &vector::operator=(vector &v)
{
	cout << "operator =" << endl;

	double *p = new double[v.size()];
	for(int i = 0; i < v.size(); i++)
		p[i] = v[i];
	delete elem;
	elem = p;
	sz = v.size();

	return *this;
}

vector::~vector()
{
	cout << "~vector - " << this << endl;
	delete elem;
}

double &vector::operator[](int n) {
	if(n < 0 || n >= size())
		throw std::out_of_range{"vector::operator[]"};

	return elem[n];
}

int vector::size() const
{
	return sz;
}

char *vector::to_string() const
{
	return object::to_string();
}

string vector::tostring() const
{
	string s = "";
	int i;

	for(int i = 0; i < sz; i++) {
		stringstream ss;
		ss << elem[i];
		s.append(ss.str());
		s.append(" ");
		ss.clear();
	}

	return s;
//	return object::tostring();
}

void vector::pure_vfunc(void)
{
}
