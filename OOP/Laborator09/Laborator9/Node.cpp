#include <iostream>
#include "Node.h"
using namespace std;

template<class K, class V>
void Node<K, V>::operator=(V value) {
	this->value = value;
}

template<class K, class V>
Node<K, V>::~Node() {
	if (next != nullptr) {
		delete next;
	}
}
