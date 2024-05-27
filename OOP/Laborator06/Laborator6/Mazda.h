#include "Car.h"
class Mazda : public Car {
public:
	Mazda();
	float GoCar(bool& b, int, int) override;
	char* getName() override;
};