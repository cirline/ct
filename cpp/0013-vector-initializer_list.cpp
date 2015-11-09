#include <iostream>
#include <stdexcept>
#include <sstream>
#include "cpp_vector.h"

using namespace std;

Ventor::Ventor(unsigned long n) : elem{new double[n]}, len{n} {}

double & Ventor::operator[](int i) {
    // 2.4.3, error handing
    if(i < 0 || i >= size()) {
        throw out_of_range{"Ventor::operator[]"};
    }
    return elem[i];
}

/* container initializing */
cpp_vector::cpp_vector(initializer_list<double> list)
    :len{list.size()},
    elem{new double[list.size()]}
{
    copy(list.begin(), list.end(), elem);
}

unsigned long cpp_vector::size() {
    return len;
}

string cpp_vector::tostring(void) {
    ostringstream ts;
    ts << "vector: { ";
    for(int i = 0; i < size(); i++)
        ts << elem[i] << ", ";
    ts << "}";
    return ts.str();
}

int main(void) {
    Ventor v(3);

    for(int i = 0; i < v.size(); i++) {
        v[i] = i;
    }

    /* container initializing */
    cpp_vector v1 = {5, 8, 3, 9};
    cout << v1.tostring() << endl;

    // 2.4.3, error handing
    try {
        for(int i = 0; i <= v.size(); i++) {
            std::cout << "v[" << i << "] = " << v[i] << ";\n";
        }
    } catch(out_of_range) {
        std::cout << "vendor out of range!\n";
    }
    return 0;
}


