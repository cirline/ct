#ifndef __0014_ABSTRACT_CLASS_H__
#define __0014_ABSTRACT_CLASS_H__

#include <iostream>

using namespace std;

class cpp_container {
    public:
    virtual int size() const =0;
    virtual ~cpp_container() {
        cout << "cpp_container destructor running ..." << endl;
    }
};

#endif
