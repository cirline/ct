#include <iostream>

using namespace std;

enum class TrafficLight {
	red, green, yellow
};

class TrafficLightController {
	public:
	TrafficLightController() : light{TrafficLight:red} {}

	TrafficLight & operator++() {
		switch(light) {
			case TrafficLight::red:
				light = TrafficLight::yellow;
				break;
			case TrafficLight::yellow:
				light = TrafficLight::green;
				break;
			case TrafficLight::green:
				light = TrafficLight::red;
				break;
		}
		return light;
	}

	TrafficLight & status() {
		return light;
	}

	private:
	TrafficLight light;
}

int main(void)
{
	TrafficLightController tlc;

	cout << "now staus is: " << tlc.status() << "\n";
}


