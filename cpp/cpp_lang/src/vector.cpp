#include <stdexcept>
#include <sstream>

#include <cpplang/vector.h>

using namespace std;
using namespace cpplang;

vector::vector(int n) : elem{new double[n]}, sz{n} {
	/* nothing */
	char *p = "vector info";
	class_info = p;
	descriptor = "vector descriptor";
}

vector::vector(initializer_list<double> list) :
	elem{new double[list.size()]},
	sz{(int)list.size()}
{
	copy(list.begin(), list.end(), elem);
	char *p = "vector info";
	class_info = p;
	descriptor = "vector descriptor";
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
