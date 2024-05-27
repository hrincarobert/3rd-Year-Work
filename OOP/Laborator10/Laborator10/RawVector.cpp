#include "RawVector.h"
#include <iostream>
using namespace std;

RawVector::RawVector(int n){
	v = new int[n];
	maxCapacity = n;
	size = 0;
}

bool RawVector::Add(int x) {
	if (size + 1 == maxCapacity) {
		return false;
	}
	v[size++] = x;
	return true;
}

bool RawVector::Delete(int n) {
	while (n < size - 1) {
		v[n] = v[n + 1];
		n++;
	}
	size--;
	return true;
}

void RawVector::PrintAll() {
	for (int i = 0; i < size; i++) {
		cout << v[i] <<" ";
	}
	cout << "\n";
}

void RawVector::Iterate(void(*fun)(int&)) {
	for (int i = 0; i < size; i++) {
		fun(v[i]);
	}
}

void RawVector::Filter(bool(*fun)(int)) {
	for (int i = 0; i < size; i++) {
		if (fun(v[i]) == true) {
			Delete(i);
			i--;
		}
	}
}

RawVector::~RawVector() {
	delete[] v;
}
