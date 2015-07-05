#include <iostream>

using namespace std;


char get_char()
{
    char ch;

    cin >> ch;

    return ch == '#' ? 0 : ch;
}

int main(void)
{
    char ch;
    while((ch = get_char()) != 0) {
        cout << ch;
    }

    return 0;
}


