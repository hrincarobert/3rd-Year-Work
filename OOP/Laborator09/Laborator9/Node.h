#pragma once

template<class K, class V>
class Node {
public:
	V value;
	K key;
	unsigned long rawKey = 0;

	Node<K, V>* next = nullptr;

	Node(unsigned long rawKey) : rawKey(rawKey) {};
	Node(unsigned long rawKey, V value, K key): rawKey(rawKey), value(value), key(key)  {};
	Node() {};
	void operator =(V value);
	~Node();
};
