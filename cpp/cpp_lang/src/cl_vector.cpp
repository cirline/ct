#include <cpplang/cl_vector.h>

cl_vector::cl_vector(int n) : elem{new double[n]}, sz{n} {
	/* nothing */
}

int cl_vector::size() {
	return sz;
}

