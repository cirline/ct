#ifndef __CPP_VECTOR__
#define __CPP_VECTOR__

#define Ventor cpp_vector

using namespace std;

class cpp_vector {
    public:
    cpp_vector(unsigned long n);

    /* container initializing */
    cpp_vector(initializer_list<double> list);

    double & operator[](int i);

    unsigned long size();
    string tostring(void);

    private:
    double * elem;
    unsigned long len;
};

#endif

