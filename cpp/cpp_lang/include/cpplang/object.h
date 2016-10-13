#ifndef __CPPLANG_OBJECT_H__
#define __CPPLANG_OBJECT_H__

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
	protected:
		char *class_info;

};

#endif
