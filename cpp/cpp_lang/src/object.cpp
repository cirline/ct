#include <iostream>

#include "cpplang/object.h"

using namespace std;
using namespace cpplang;

char *object::to_string() const
{
	static char ch = 'c';

	cout << "this is object " << __func__ << endl;
	//return class_info;
	return &ch;
}

string object::tostring() const
{
	return descriptor;
}
