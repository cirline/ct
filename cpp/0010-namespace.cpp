#include <iostream>

using namespace std;

namespace name_test {
    class ns_class {
        public:
            ns_class() : i{1} {}
            int seti(int n) {
                i = n;
                return i;
            }

            int geti() {
                return i;
            }
        private:
            int i;
    };

    int main() {
        ns_class nc;

        cout << "ns_class i = " << nc.geti() << "\n";

        nc.seti(3);

        return nc.geti();


    }
}


int main()
{
    int x = name_test::main();

    cout << "test main return is " << x << "\n";

    return 0;
}
