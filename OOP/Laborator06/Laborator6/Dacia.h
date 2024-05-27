class Dacia : public Car {
public:
	Dacia();
	float GoCar(bool& b, int, int) override;
	char* getName() override;
};