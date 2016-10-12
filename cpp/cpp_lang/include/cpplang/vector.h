#ifndef __CPPLANG_VECTOR_H__
#define __CPPLANG_VECTOR_H__

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

class cpplang::vector {
	public:
		vector(int n);
		int size();
		/* object[n] be referred to elem[n] */
		double &operator[](int n);
	private:
		double *elem;
		int sz;
};

#endif
