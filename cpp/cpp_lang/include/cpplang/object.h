#ifndef __CPPLANG_OBJECT_H__
#define __CPPLANG_OBJECT_H__

#include <string>

using namespace std;

namespace cpplang {
	class object;
};

class cpplang::object {
	public:
		virtual void pure_vfunc(void) = 0;

		/*
		 * const indicate this function don't modify the object
		 */
		virtual char *to_string() const;
		virtual string tostring() const;
	protected:
		char *class_info;
		string descriptor;
};

#endif
