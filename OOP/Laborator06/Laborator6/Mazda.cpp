#include "Car.h"
#include "Mazda.h"
#include "Weather.h"
#include <string>
using namespace std;

Mazda::Mazda() {
	fuelCapacity = 100;
	fuelConsumption = 6;
	averageSpeed[Rain] = 55;
	averageSpeed[Sunny] = 100;
	averageSpeed[Snow] = 40;
	name = (char*)"Mazda";
}

float Mazda::GoCar(bool& b, int w, int circuitLength) {
	int speed = averageSpeed[w];

	float hours = fuelCapacity / fuelConsumption;
	float distance = hours * speed;

	b = distance > circuitLength;
	return (hours * circuitLength / distance);
}

char* Mazda::getName() {
	return name;
}
