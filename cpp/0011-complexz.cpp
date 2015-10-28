#include <iostream>
#include <string>
#include <sstream>

using namespace std;

namespace cpp_lesson {
    class complexz {
        public:
        complexz() : /* start */ zr{0}, zi{0} /* end */ { /* nothing */ }
        complexz(double r, double i) : /* start */ zr{r}, zi{i} /* end */ { /* nothing */ }

        string echo() {
            ostringstream t;
            t << zr << "+" << zi << "i";
            return t.str();
        }

        void set(double r, double i) {
            zr = r;
            zi = i;
        }

        double getr() const { return zr; }

        double geti() const { return zi; }

        complexz & operator+=(complexz a) {
            zr += a.getr();
            zi += a.geti();
            return *this;
        }

        private:
            double zr, zi;
    };

    complexz operator+(complexz a, complexz b) {
        return a += b;
    }

}

int main(void)
{
    cpp_lesson::complexz cz, cz1(2, 2), cz2;

    cz.set(1, 5);

    cz += cz1;
    cz2 = cz + cz1;

    cout << cz2.echo() << endl;

    return 0;
}

