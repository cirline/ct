#include <iostream>

#include "cpplang/object.h"

using namespace std;
using namespace cpplang;

char *object::to_string() const
{
	cout << "object to_string" << endl;
	return class_info;
}

string object::tostring() const
{
	return descriptor;
}
