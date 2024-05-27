#include <iostream>
#include <functional>
#include "Node.h"
#include "Map.h"
#include "MyIterator.h"
using namespace std;

template<class K, class V>
Map<K, V>::Map() {
	capacity = 10;
	length = 0;
	container = new List<K, V>[capacity];
	allElements = new Node<K, V>*[capacity];
}

template<class K, class V>
void Map<K, V>::Set(K key, V value) {
	if (length + 1 >= capacity) {
		Resize();
	}
	unsigned long rawKey = getHash(key);
	int keyIndex = (int)(rawKey % capacity);

	Node<K, V>* newNode = new Node<K, V>(rawKey, value, key);
	container[keyIndex].add(newNode);
	length++;
}

template<class K, class V>
bool Map<K, V>::Get(const K& key, V value) {
	
	int rawKey = getHash(key);
	int keyIndex = rawKey % capacity;
	
	Node<K, V>* temp = container[keyIndex].root;
	
	while (temp != nullptr) {
		if (temp->rawKey == rawKey) {
			temp->value = value;
			return true;
		}
		temp = temp->next;
	}
	
	if (length + 1 >= capacity) {
		Resize();
	}

	Node<K, V>* n = new Node<K, V>(rawKey, value, key);
	container[keyIndex].add(n);
	length++;

	return false;
}

template<class K, class V>
void Map<K, V>::printAll() {
	for (int i = 0; i < length; i++) {
		cout << allElements[i]->value << " "<< allElements[i]->key <<"\n";
	}

}

template<class K, class V>
void Map<K, V>::Resize() {
	List<K, V>* backup = container;
	cout << "Resize\n";

	capacity = capacity * 2;
	
	container = new List<K, V>[capacity];

	for (int i = 0; i < capacity / 2; i++) {
		Node<K, V>* temp = backup[i].root;

		while (temp != nullptr) {
			int indexKey = temp->rawKey % capacity;
			
			Node<K, V>* last = temp;
			temp = temp->next; // because the add method destroys the chain, keep a reference for the last temp
			container[indexKey].add(last);
		}
	}
	delete[] backup;
}

template<class K, class V>
int Map<K, V>::Count() {
	return length;
}

template<class K, class V>
void Map<K, V>::Clear() {
	delete[] container;
	capacity = 10;
	container = new List<K, V>[capacity];
	allElements = new Node<K, V> * [capacity];
	
	length = 0;
}

template<class K, class V>
void Map<K, V>::Delete(const K key) {
	int rawKey = getHash(key);
	int indexKey = rawKey % capacity;

	Node<K, V>* temp = container[indexKey].root;

	while (temp != nullptr) {
		if (temp->next != nullptr && temp->next->rawKey == rawKey) {
			temp->next = temp->next->next;
			temp->next->next = nullptr;
			delete temp->next;
			length--;
			return;
		}
		temp = temp->next;
	}
}

template<class K, class V>
bool Map<K, V>::Includes(const Map<K, V>& m) {
	int nr = 0;
	for (auto [indexA, keyA, valueA] : m) {
		for (auto [indexB, keyB, valueB] : *this) {
			if (keyA == keyB && valueA == valueB) {
				nr++;
			}
		}
	}
	return nr == m.Count();
}

template<class K, class V>
Node<K, V>& Map<K, V>::operator[](K key) {

	unsigned long rawKey = getHash(key);
	int indexKey = (int)(rawKey % capacity);

	// check for duplicate key
	Node<K, V>* temp = container[indexKey].root;
	while (temp != nullptr) {
		if (temp->rawKey == rawKey) {
			return *temp;
		}
		temp = temp->next;
	}

	if (length + 1 >= capacity) {
		Resize();
	}

	Node<K, V>* n = new Node<K, V>();
	n->rawKey = rawKey;
	n->key = key;

	container[indexKey].add(n);
	length++;

	return *n;
}

template<class K, class V>
MyIterator<K, V> Map<K, V>::begin() {
	allElements = new Node<K, V>*[capacity];

	int l = 0;
	for (int i = 0; i < capacity; i++) {
		Node<K, V>* temp = container[i].root;
		while (temp != nullptr) {
			allElements[l++] = temp;
			temp = temp->next;
		}
	}

	MyIterator<K, V> it(capacity, length ,allElements);
	return it;
}

template<class K, class V>
MyIterator<K, V> Map<K, V>::end() {
	MyIterator it(capacity, length, allElements);
	return it;
}

