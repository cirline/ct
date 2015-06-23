#include <iostream>
#include <stdexcept>
#include "2.4.1_vendor.h"

using namespace std;

Ventor::Ventor(int n) : elem{new double[n]}, len{n} {}

double & Ventor::operator[](int i) {
    // 2.4.3, error handing
    if(i < 0 || i >= size()) {
        throw out_of_range{"Ventor::operator[]"};
    }
    return elem[i];
}

int Ventor::size() {
    return len;
}

int main(void) {
    Ventor v(3);

    for(int i = 0; i < v.size(); i++) {
        v[i] = i;
    }

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


