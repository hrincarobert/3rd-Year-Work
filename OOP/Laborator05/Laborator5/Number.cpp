#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#include "Number.h"
#include <string.h>
#include <iostream>
using namespace std;

Number::Number(const char* value, int base) {
	this->value = new char[strlen(value)];
	memcpy(this->value, value, strlen(value) + 1);
	this->base = base;
	isNegative = value[0] == '-';
}

Number::~Number() {
}

void Number::SwitchBase(int newBase) {
	
	long long n = convertToBaseTen(value, base);
	char* newValue = convertToBase(n, newBase);
	
	value = newValue;
	base = newBase;
}

void Number::Print() {
	for (int i = 0; value[i] != '\0'; i++) {
		cout << value[i];
	}
	cout << "\n";
}

int Number::GetDigitsCount() {
	return strlen(value) - (isNegative ? 1 : 0);
}

int Number::GetBase() {
	return base;
}

long long Number::getBaseTen() {
	long long n = convertToBaseTen(value, base);
	return isNegative ? -n : n;
}


long long Number::convertToBaseTen(char* s, int base) {
	
	long long n = 0, p = 1;

	for (int i = strlen(value) - 1; i >= (int)isNegative; i--) {
		n += normalizeDigit(s[i]) * p;
		p *= base;
	}
	
	return n;
}

char* Number::convertToBase(long long n, int base) {
	
	char* c = new char[50];
	int i = 0;

	while (n != 0) {
		int d = n % base;
		c[i++] = formalizeDigit(d);

		n /= base;
	}
	if (isNegative) {
		c[i++] = '-';
	}
	c[i] = '\0';

	for (int l = 0; l < i / 2; l++) {
		char aux = c[l];
		c[l] = c[i - l - 1];
		c[i - l - 1] = aux;
	}

	return c;
}

int Number::normalizeDigit(char c) {
	return c >= 'A' ? c - 'A' + 10 : c - '0';
}

char Number::formalizeDigit(int n) {
	return n >= 10 ? 'A' + (n - 10) : '0' + n;
}


Number::Number(const Number &n) {
	base = n.base;
	//strcpy(value, n.value);
	value = n.value;
	this->isNegative = n.isNegative;
	// delete n
}

Number::Number(const Number&& n) {
	this->base = n.base;
	//copyString(value, n.value);
	value = n.value;
	isNegative = n.isNegative;
}

void copyString(char* a, char* b) {
	b = new char[strlen(a)];
	for (int i = 0; a[i] != '\0'; i++) {
		b[i] = a[i];
	}
}


Number::Number(const int n) {
	base = 10;
	value = toString(n);
	bool b = n < 0;
	isNegative = b;
}
void Number::operator=(int n) {
	Number nr(toString(n), 10);
	nr.SwitchBase(base);
	*this = nr;
}


char Number::operator[](int i) {
	return value[i];
}

Number operator+(const Number& x, const Number& y) {
	Number a = x;
	Number b = y;
	int biggestBase = a.GetBase() > b.GetBase() ? a.GetBase() : b.GetBase();

	long long nr = a.getBaseTen();
	nr += b.getBaseTen();

	char* c = toString(nr);

	Number newNumber(c, 10);
	newNumber.SwitchBase(biggestBase);

	return newNumber;
}


Number operator-(const Number& x, const Number& y) {
	Number a = x;
	Number b = y;
	int biggestBase = a.GetBase() > b.GetBase() ? a.GetBase() : b.GetBase();

	long long nr = a.getBaseTen();
	nr -= b.getBaseTen();

	char* c = toString(nr);

	Number newNumber(c, 10);
	
	newNumber.SwitchBase(biggestBase);

	return newNumber;
}

bool Number::operator>(Number& n) {
	long long n1 = getBaseTen();
	long long n2 = n.getBaseTen();

	return n1 > n2;
}

void Number::operator=(const char* s) {
	Number nr(s, 10);
	*this = nr;
}

void Number::operator=(const Number &n) {
	base = n.base;
	value = n.value;
	isNegative = n.isNegative;
}

void Number::operator+=(Number& n) {
	long long n1 = getBaseTen();
	n1 += n.getBaseTen();
	bool b = n1 < 0;
	
	toString(n1);

	value = toString(n1);

	isNegative = b;

	SwitchBase(base);
}

void Number::operator--() {
	value++;
}

void Number::operator--(int) {
	value[strlen(value) - 1] = '\0';
}

char* toString(long long n) {
	char* s = new char[50];
	int i = 0;

	bool b = false;
	if (n < 0) {
		n = -n;
		b = true;
	}

	while (n != 0) {
		int d = n % 10;

		s[i++] = '0' + d;

		n /= 10;
	}

	if (b) {
		s[i++] = '-';
	}

	s[i] = '\0';

	for (int l = 0; l < i / 2; l++) {
		char aux = s[l];
		s[l] = s[i - l - 1];
		s[i - l - 1] = aux;
	}
	
	return s;
}


