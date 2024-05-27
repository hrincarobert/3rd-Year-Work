#include "Sort.h";
#include <cstdlib>
#include <stdarg.h>
#include <iostream>

Sort::Sort(int min, int max, int n) {
	int dif = max - min;
	v = new int[n];
	this->n = n;

	for (int i = 0; i < n; i++) {
		v[i] = min + rand() % (dif + 1);
	}
}

Sort::Sort(int* v, int n) {
	this->n = n;
	this->v = v;
}

Sort::Sort(int n, ...) {
	va_list ap;
	va_start(ap, n);

	v = new int[n];
	this->n = n;

	for (int i = 1; i <= n; i++) {
		int a = va_arg(ap, int);
		v[i - 1] = a;
	}
	va_end(ap);
}

Sort::Sort(const char* s) {
	n = 0;
	for (int i = 0; s[i] != '\0'; i++) {
		if(s[i] == ',')
			n++;
	}
	n++;
	v = new int[n];

	int nr = 0;
	int i = 0;
	for (int l = 0; s[l] != '\0'; l++) {
		if (s[l] == ',') {
			v[i++] = nr;
			nr = 0;
			continue;
		}

		nr = nr * 10 + (s[l] - '0');

	}

	v[i++] = nr;

}

void Sort::InsertSort(bool reverse) {
	
	for (int i = 1; i < this->n; i++) {
		int c = v[i];
		int j = i - 1;
		while (j >= 0 && v[j] > c) {
			v[j + 1] = v[j];
			j--;
		}
		v[j + 1] = c;
	}

	if (reverse) {
		reverseArray();
	}

}

void Sort::QuickSort(bool reverse) {
	this->quickSort(0, n - 1);

	if (reverse) {
		reverseArray();
	}
}
void Sort::quickSort(int low, int high) {
	if (low < high) {
		int pi = partition(low, high);

		quickSort(low, pi - 1);
		quickSort(pi + 1, high);
	}
}
int Sort::partition(int low, int high) {
	
	int pivot = v[high];
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++) {
		if (v[j] < pivot) {
			i++;
			swap(v[i], v[j]);
		}
	}
	swap(v[i + 1], v[high]);
	return (i + 1);
}

void Sort::BubbleSort(bool reverse) {

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (v[j] > v[j + 1]) {
				swap(v[j], v[j + 1]);
			}
		}
	}

	if (reverse) {
		reverseArray();
	}
}

void Sort::Print() {
	for (int i = 0; i < n; i++) {
		std::cout << GetElementFromIndex(i)<<" ";
	}
	std::cout << "\n";
}

int Sort::GetElementsCount() {
	return n;
}

int Sort::GetElementFromIndex(int index) {
	return v[index];
}

void Sort::swap(int& a, int& b) {
	int aux = a;
	a = b;
	b = aux;
}

void Sort::reverseArray() {
	for (int i = 0; i < n / 2; i++) {
		this->swap(v[i], v[n - i - 1]);
	}
}
