/*
 * templates is similar macro, so the templates codes
 * cannot be prebuild. In other words, templates codes
 * cannot be build as shared libraries
 *
 * it this example, class definition and implementation
 * are in one file
 */

#include <iostream>

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

template<typename T>
class tplns::tplcls : public tplns::tplbase {
	public:
		tplcls(int n);

		void set_value(T v);
		T get_value();

	private:
		T x;
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

int main(void)
{
	cout << "hello world" << endl;

	tplcls<int> t(10);

	t.set_value(9);
	t.get_value();

	tplcls<float> t2(10);
	t2.set_value(9.9);
	t2.get_value();

	return 0;
}

