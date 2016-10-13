#include <iostream>

#include "cpplang/object.h"

using namespace std;
using namespace cpplang;

char *object::to_string()
{
	cout << "object " << __func__ << endl;
	return class_info;
}
