#include "Car.h"
#include "Dacia.h"
#include "Weather.h"
#include <string>
using namespace std;

Dacia::Dacia() {
	fuelCapacity = 54;
	fuelConsumption = 20;
	averageSpeed[Rain] = 30;
	averageSpeed[Sunny] = 50;
	averageSpeed[Snow] = 20;
	name = (char*)"Dacia";
}

float Dacia::GoCar(bool& b, int w, int circuitLength) {
	int speed = averageSpeed[w];

	float hours = fuelCapacity / fuelConsumption;
	float distance = hours * speed;

	b = distance > circuitLength;
	return (hours * circuitLength / distance);
}

char* Dacia::getName(){
	return name;
}
