#include <iostream>

class Ventor {
    public:
    Ventor(int n) : elem{new double[n]}, len{n} {}

    double & operator[](int i) {
        return elem[i];
    }

    int size() {
        return len;
    }

    private:
    double * elem;
    int len;
};

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


