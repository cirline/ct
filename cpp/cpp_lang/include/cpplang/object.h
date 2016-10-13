#ifndef __CPPLANG_OBJECT_H__
#define __CPPLANG_OBJECT_H__

namespace cpplang {
	class object;
};

class cpplang::object {
	public:
		virtual void pure_vfunc(void) = 0;
		virtual char *to_string();
	private:
		char *class_info;

};

#endif
