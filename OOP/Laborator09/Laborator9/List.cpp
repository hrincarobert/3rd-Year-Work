#include "List.h"

template<class K, class V>
List<K, V>::List() {
	root = nullptr;
	head = nullptr;
	length = 0;
}

template<class K, class V>
void List<K, V>::add(Node<K, V>* n) {
	
	n->next = nullptr; // reset the node;
	if (root == nullptr) {
		root = n;
		head = n;
		length++;
		return;
	}

	head->next = n;
	head = head->next;
	length++;
}

template<class K, class V>
List<K, V>::~List() {
	delete root;
}
