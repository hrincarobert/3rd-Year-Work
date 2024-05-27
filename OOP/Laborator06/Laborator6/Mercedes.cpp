#include "Car.h"
#include "Mercedes.h"
#include "Weather.h"
#include <string>
using namespace std;

Mercedes::Mercedes() {
	fuelCapacity = 120;
	fuelConsumption = 10;
	averageSpeed[Rain] = 140;
	averageSpeed[Sunny] = 170;
	averageSpeed[Snow] = 60;
	name = (char*)"Mercedes";
}

float Mercedes::GoCar(bool& b, int w, int circuitLength) {
	int speed = averageSpeed[w];

	float hours = fuelCapacity / fuelConsumption;
	float distance = hours * speed;

	b = distance > circuitLength;
	return (hours * circuitLength / distance);
}

char* Mercedes::getName() {
	return name;
}
