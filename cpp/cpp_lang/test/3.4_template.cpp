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

void va_tpl() {
	cout << "va_tpl end" << endl;
}

template<typename T, typename... Ttail>
void va_tpl(T head, Ttail... tail) {
	cout << head << endl;
	va_tpl(tail...);
}

template<typename Tkey, typename Tvalue>
class kv_map {
};

template<typename Tvalue>
using string_map = kv_map<string, Tvalue>

/* so
 * string_map<int> m; m is a kv_map<string, int>
 */

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

	/* lambda expression, it generates a function object, (functor)
	 * [&]: all local names used by reference
	 * [=]: all local names used value/object copy
	 * []: captrue nothing
	 * [&x]: captrue x as reference
	 * [=x]: captrue x as value/object copy
	 *
	 * [&](const int &a){ return a < x; }
	 * this expression same as lt_functor<int>(x), same as invoke object(a)
	 */
	cout << "count(v < 3) = " << count(v, [](double &a){ return a < 3; }) << endl;	// this lambda captrue nothing

	double xx = 2;
	cout << "count(v < 2) = " << count(v, [&](double &a){ return a < xx; }) << endl;	// xx is a reference

	/* variadic templates
	 */
	va_tpl(1.2, 'k', 4, 5, "hello world");

	return 0;
}

