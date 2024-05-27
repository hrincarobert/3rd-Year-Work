#include "Vector.h"
#include <iostream>

template<class T>
Vector<T>::Vector() {
	v = new T*[1];
	length = 0;
	vectorSize = 1;
}

template<class T>
void Vector<T>::push(T& t) {
	if (length + 1 >= vectorSize) {
		resize();
	}

	v[length++] = &t;

}

template<class T>
T& Vector<T>::pop() {
	if (length - 1 >= 0) {
		return get(length - 1);
	}
}

template<class T>
void Vector<T>::remove(int i) {
	if (i >= 0 && i < length) {
		delete v[i];
		
		while (v[i] != nullptr) {
			v[i] = v[i + 1];
			i++;
		}
		length--;
	}
}

template<class T>
void Vector<T>::insert(T& t, int i) {
	if (i < 0 || i >= length) {
		return;
	}
	
	if (length + 1 >= vectorSize) {
		resize();
	}

	int j = length;

	while (j > i) {
		v[j] = v[j - 1];
		j--;
	}

	v[i] = &t;
	length++;
}

template<class T>
void Vector<T>::sort(bool (*fun)(T&, T&)) {

	for (int i = 0; i < length - 1; i++) {
		for (int j = 0; j < length - i - 1; j++) {
			if (fun == nullptr) {
				if (*v[j] < *v[j + 1] == false) {
					T* aux = v[j];
					v[j] = v[j + 1];
					v[j + 1] = aux;
				}
			}
			else {
				bool r = fun(*v[j], *v[j + 1]);
				if (!r) {
					T* aux = v[j];
					v[j] = v[j + 1];
					v[j + 1] = aux;
				}

			}
		}
	}
}

template<class T>
T& Vector<T>::get(int i) {
	if (i >= 0 && i < length) {
		return *v[i];
	}
}

template<class T>
void Vector<T>::set(T& t, int i) {
	if (i >= 0 && i < length) {
		v[i] = &t;
	}
}

template<class T>
int Vector<T>::count() {
	return length;
}

template<class T>
int Vector<T>::firstIndexOf(T& t, bool (*fun)(T&, T&) ) {
	for (int i = 0; i < length; i++) {
		if (fun == nullptr) {
			if (t == *v[i]) {
				return i;
			}
		}
		else {
			bool r = fun(*v[i], t);

			if (r) {
				return i;
			}
		}
	}
	return -1;
}

template<class T>
T** Vector<T>::getArray() {
	return v;
}

template<class T>
void Vector<T>::deleteVector() {
	delete[] v;
}

template<class T>
void Vector<T>::resize() {
	T** newV = new T * [vectorSize * 2];
	vectorSize *= 2;
	for (int i = 0; i < length; i++) {
		newV[i] = v[i];
		v[i] = nullptr;
	}

	delete[]v;

	v = newV;
	// std::cout << "Resize()" << vectorSize<<"\n";
}



