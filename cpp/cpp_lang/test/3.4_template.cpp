/*
 * templates is similar macro, so the templates codes
 * cannot be prebuild. In other words, templates codes
 * cannot be build as shared libraries
 *
 * it this example, class definition and implementation
 * are in one file
 */

#include <iostream>

#include "cpplang/vector.h"

using namespace std;

namespace tplns {
	template<typename T>
	class tplcls;
}

namespace tplns {
	class tplbase;
}

class tplns::tplbase {
	public:
		int a;
};

/*
 * to use 2 or more templates, type as:
 * template<typename T1, typename T2, ...>
 */
template<typename T>
class tplns::tplcls : public tplns::tplbase {
	public:
		tplcls(int n);

		void set_value(T v);
		T get_value();

	private:
		T x;
};

namespace cpplang {
	template<typename T>
	class lt_functor;
};

template<typename T>
class cpplang::lt_functor {
	private:
		const T val;
	public:
		lt_functor(const T &v) : val{v} {}

		/*
		 * function object also called functor
		 * typename operator()(const vector &v), that object() is a functor
		 *
		 * while a function return bool, it can be said a predicate
		 * predicate: something that we can invoke to return bool
		 */
		bool operator()(const T &v) const {
			return v < val;
		}

};

using namespace tplns;

template<typename T>
tplcls<T>::tplcls(int n)
{
}

template<typename T>
void tplcls<T>::set_value(T v)
{
	x = v;
	cout << "set x = " << x << endl;
}

template<typename T>
T tplcls<T>::get_value()
{
	cout << "get x = " << x << endl;
	return x;
}

/*
 * a functor can be used as argument
 */
template<typename object, typename predicate>
int count(object &obj, predicate pred) {
	int count = 0;
	for(auto &x : obj)
		if(pred(x))
			count++;

	return count;
}

int main(void)
{
	cout << "hello world" << endl;

	tplcls<int> t(10);

	t.set_value(9);
	t.get_value();

	tplcls<float> t2(10);
	t2.set_value(9.9);
	t2.get_value();

	cpplang::vector v = {1, 2, 3, 4, 5};

	/* transfer a less then 4 functor to count */
	cout << "count(v < 4) = " << count(v, cpplang::lt_functor<double>(4)) << endl;

	return 0;
}

