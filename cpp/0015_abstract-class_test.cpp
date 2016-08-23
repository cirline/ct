#include <iostream>

#include "0014_abstract-class.h"

using namespace std;

class cpp_container_test: public cpp_container {
    public:
        int size() const {
            return 1;
        }
};



int main(void)
{
    {
        cpp_container_test cct;
        cout << "size = " << cct.size() << endl;
    }

    cout << "main end" << endl;

    return 0;

}

