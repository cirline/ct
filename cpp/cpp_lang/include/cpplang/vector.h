#ifndef __CPPLANG_VECTOR_H__
#define __CPPLANG_VECTOR_H__

#include <string>
#include "cpplang/object.h"

using namespace std;

/* 2.4 Modularity
 *
 * there are declarations
 * definitions will be written elsewhere
 *
 * definition like:
 *
 * classname::function {
 *	// function body
 * }
 */
namespace cpplang {
	class vector;
}

class cpplang::vector : public cpplang::object {
	public:
		vector(int n);
		vector(initializer_list<double> list);
		/*
		 * copy constructor
		 */
		vector(const vector &v);

		~vector();

		int size() const;
		/* object[n] be referred to elem[n] */
		double &operator[](int n);
		/* copy container operator, also need copy constructor */
		vector &operator=(const vector &v);

		char *to_string() const;
		string tostring() const;
		void pure_vfunc(void);
	private:
		double *elem;
		int sz;
};

#endif
