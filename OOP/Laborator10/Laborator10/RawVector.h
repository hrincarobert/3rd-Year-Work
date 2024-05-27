#pragma once

class RawVector {
	int* v;
	int size;
	int maxCapacity;
public:
	RawVector(int);

	bool Add(int);
	bool Delete(int);
	void PrintAll();

	void Iterate(void (*fun)(int&));
	void Filter(bool (*fun)(int));

	~RawVector();
};