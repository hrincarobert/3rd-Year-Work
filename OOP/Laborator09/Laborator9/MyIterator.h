#include "Node.h"
#pragma once

template<class K, class V>
struct RetData {
	K key;
	V value;
	int index;
};

template<class K, class V>
class MyIterator {
public:
	Node<K, V>** elements;
	int capacity;
	int i = 0;
	int end;
	
	MyIterator(int capacity, int end, Node<K, V>** el) : capacity(capacity),end(end) , elements(el) {}
	
	MyIterator& operator++() {
		i++;
		return *this; 
	}

	RetData<K, V> operator* () {
		RetData<K, V> rd = {
			elements[i]->key,
			elements[i]->value,
			elements[i]->rawKey % capacity
		};
		return rd;
	}

	bool operator != (const MyIterator it) { return i < end; }
	
};