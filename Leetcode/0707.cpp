#include<iostream>
using namespace std;


class MyLinkedList {
private:
	struct ListNode {
		int val;
		ListNode* next;
		ListNode(int x) : val(x), next(nullptr) {}
	};


	ListNode* _dummyhead;
	int _size;

public:
	MyLinkedList() {
		_dummyhead = new ListNode(0);
		_size = 0;
	}

	int get(int index) {
		int val = 0;
		ListNode* cur = _dummyhead;

		if (index >= _size) {
			return -1;
		}

		for (int i = 0; i < index; i++) {
			cur = cur->next;
		}

		val = cur->val;
		return val;
	}

	void addAtHead(int val) {
		ListNode* newnode = new ListNode(val);
		newnode->next = _dummyhead->next;
		_dummyhead->next = newnode;
		_size++;
	}

	void addAtTail(int val) {
		ListNode* newnode = new ListNode(val);
		ListNode* tail = _dummyhead;

		//for (int i = 0; i < _size; i++) {
			//tail = tail->next;
		//}

		while (tail->next != nullptr) {
			tail = tail->next;

		}
		tail->next = newnode;
		_size++;
	}

	void addAtIndex(int index, int val) {
		ListNode* newnode = new ListNode(val);
		ListNode* cur = _dummyhead;

		if (index > _size || index < 0) {
			return;
		}
		
		for (int i = 0; i < index-1; i++) {
			cur = cur->next;
		}
		
		newnode->next = cur->next;
		cur->next = newnode;
		

		_size++;
	}

	void deleteAtIndex(int index) {
		ListNode* cur = _dummyhead;
		if (index > _size || index < 0) {
			return;
		}

		for (int i = 0; i < index - 1; i++) {
			cur = cur->next;
		}

		ListNode* tmp = cur->next;
		cur->next = tmp->next; 
		delete tmp;
		tmp = nullptr;
		_size--;

	}

	void printLinkedList() {
		ListNode* cur = _dummyhead;
		while (cur->next != nullptr) {
			cout << cur->next->val << " ";
			cur = cur->next;
		}
		cout << endl;
	}
};
