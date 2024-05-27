#pragma once
template <class T>
class Vector {
	T **v;
	int length;
	int vectorSize;
public:
	Vector();
	void push(T&);
	T& pop();
	void remove(int);
	void insert(T&, int);
	void sort(bool (*fun)(T&, T&) = nullptr);
	T& get(int);
	void set(T&, int);
	int count();
	int firstIndexOf(T&, bool (*fun)(T&, T&) = nullptr);

	T** getArray();
	void deleteVector();
private:
	void resize();
};

