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

	ListNode* reverse() {
		ListNode* curr = _dummyhead->next;
		ListNode* prev = nullptr;

		while(curr) {
			ListNode* next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;

		}

		_dummyhead->next = prev;

		return _dummyhead;
	}
}

