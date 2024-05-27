#pragma once
#include "Map.h"
#include "List.h"
#include "MyIterator.h"
#include <functional>
using namespace std;

template<class K, class V>
class Map {
	int length;
	int capacity;

	List<K, V>* container;
	Node<K, V>** allElements;
	hash<K> getHash;
public:
	Map();
	void Set(K, V);
	bool Get(const K&, V);
	void printAll();
	void Resize();
	int Count();
	void Clear();
	void Delete(const K);
	bool Includes(const Map<K, V>&);
	Node<K, V>& operator[] (K);

	MyIterator<K, V> begin();
	MyIterator<K, V> end();

};

