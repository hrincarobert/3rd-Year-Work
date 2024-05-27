class Ford : public Car {
public:
	Ford();
	float GoCar(bool& b, int, int) override;
	char* getName() override;
};