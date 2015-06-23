#include <iostream>
#include "2.4.1_vendor.h"

Ventor::Ventor(int n) : elem{new double[n]}, len{n} {}

double & Ventor::operator[](int i) {
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

    for(int i = 0; i < v.size(); i++) {
        std::cout << "v[" << i << "] = " << v[i] << ";\n";
    }
    return 0;
}


