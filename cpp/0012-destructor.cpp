#include <iostream>
#include <string>
#include <sstream>

using namespace std;

namespace cpp_lesson {
    class complexz {
        public:
        complexz() : /* start */ zr{0}, zi{0} /* end */ { /* nothing */ }
        complexz(double r, double i) : /* start */ zr{r}, zi{i} /* end */ { /* nothing */ }
        ~complexz() {
            cout << "hi, doing destructor, complex = " << echo() << endl;
        }

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

    cout << "start({1)" << endl;
    {
        cout << "started({1)" << endl;
        cpp_lesson:: complexz c1;
        c1.set(1, 2);
        cout << "start({2)" << endl;
        {
            cout << "started({2)" << endl;
            cpp_lesson:: complexz c2;
            c2.set(3, 4);
            cout << "end(1})" << endl;
        }
        cout << "ended(1})" << endl;
    }
    cout << "ended(2})" << endl;

    return 0;
}

