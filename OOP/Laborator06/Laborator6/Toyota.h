class Toyota : public Car {
public:
	Toyota();
	float GoCar(bool& b, int, int) override;
	char* getName() override;
};