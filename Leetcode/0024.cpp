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

	ListNode* exchange() {

	}
	
}

