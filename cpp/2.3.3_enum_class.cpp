#include <iostream>

using namespace std;

enum class Tcolor {red, green, yellow};

class Tctrl {
    public:
    Tctrl() : light{Tcolor::red} {}

    Tcolor operator++() {
        switch(light) {
            case Tcolor::red:
                light = Tcolor::yellow;
                break;
            case Tcolor::yellow:
                light = Tcolor::green;
                break;
            case Tcolor::green:
                light = Tcolor::red;
                break;
        }
        return light;
    }

    Tcolor getLight() {
        return light;
    }

    private:
    Tcolor light;
};

int main(void)
{
    int i;

    Tctrl tc;

    for(i = 0; i < 7; i++) {
    switch(tc.getLight()) {
        case Tcolor::red:
            cout << "light is red\n";
            break;
        case Tcolor::yellow:
            cout << "light is yellow\n";
            break;
        case Tcolor::green:
            cout << "light is green\n";
            break;
        default:
            cout << "unknow light\n";
            break;
    }
    ++tc;
    }

    return 0;


}

