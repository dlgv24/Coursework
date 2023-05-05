#pragma once

#include <initializer_list>

template <typename T>
class List {
private:
	struct Node {
		T data;
		Node* next;
		Node(T data, Node* next) {
			this->data = data;
			this->next = next;
		}
	};
	Node* _head;
	size_t _size;
public:
	List() : _head(nullptr), _size(0) {}

	List(std::initializer_list<T> list) : List() {
		for (auto i : list) {
			push_back(i);
		}
	}

	List(const List<T>& list) : List() {
		Node* current = list._head;
		while (current != nullptr) {
			push_back(current->data);
			current = current->next;
		}
	}

	~List() { clear(); }

	void push_back(T data) {
		Node* node = new Node(data, nullptr);
		if (_head == nullptr) _head = node;
		else {
			Node* last = _head;
			for (; last->next != nullptr; last = last->next);
			last->next = node;
		}
		_size++;
	}

	void remove(size_t index) {
		if (index < 0 || index >= _size) return;
		if (index == 0) {
			Node* head = _head->next;
			delete _head;
			_head = head;
		}
		else {
			Node* previous = _head, * current = _head->next;
			for (size_t i = 1; i < index; i++) {
				current = current->next;
				previous = previous->next;
			}
			previous->next = current->next;
			delete current;
		}
		_size--;
	}

	void clear() {
		while (_head != nullptr) {
			Node* next = _head->next;
			delete _head;
			_head = next;
			_size--;
		}
	}

	size_t size() {
		return _size;
	}

	bool empty() {
		return (_head == nullptr);
	}

	T& operator[](size_t index) {
		Node* current = _head;
		for (size_t i = 0; i < index; i++) current = current->next;
		return current->data;
	}

	void sort(bool (*comparator)(T& a, T& b)) {
		if (_size < 2) {
			return;
		}
		Node* sorted = nullptr;
		Node* current = _head;
		while (current != nullptr) {
			Node* next = current->next;
			if (sorted == nullptr || comparator(current->data, sorted->data)) {
				current->next = sorted;
				sorted = current;
			}
			else {
				Node* temp = sorted;
				while (temp->next != nullptr && comparator(temp->next->data, current->data)) {
					temp = temp->next;
				}
				current->next = temp->next;
				temp->next = current;
			}
			current = next;
		}
		_head = sorted;
	}
};