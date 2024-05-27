#pragma once
#include "Node.h"

template<class K, class V>
class List {
public:
	Node<K, V>* root;
	Node<K, V>* head;
	int length;

	List();
	void add(Node<K, V>*);

	~List();

};
