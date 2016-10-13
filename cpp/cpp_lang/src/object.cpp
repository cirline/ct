#include "cpplang/object.h"

using namespace cpplang;

char *object::to_string()
{
	return class_info;
}
