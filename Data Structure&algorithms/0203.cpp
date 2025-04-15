class Solution {
	struct ListNode {
		int val;
		ListNode* next;
		ListNode(int x) : val(x), next(nullptr){}
	};

	ListNode* removeElements(ListNode* head, int val) {
		while (head != nullptr && head->val == val) {
			ListNode* tmp = head;
			head = head->next;
			delete tmp;
		}

		ListNode* cur = head;
		while (cur != nullptr && cur->next != nullptr) {
			if (cur->next->val == val) {
				ListNode* tmp = cur->next;
				cur->next = cur->next->next;
				delete tmp;

			}
			else {
				cur = cur->next;
			}
		}

		return head;
	}

	ListNode* removeElements1(ListNode* head, int val) {
		ListNode* dummyhead = new ListNode(0);
		dummyhead->next = head;
		ListNode* cur = dummyhead;
		while (cur->next != nullptr) {
			if (cur->next->val == val) {
				ListNode* tmp = cur->next;
				cur->next = cur->next->next;
				delete tmp;

			}
			else {
				cur = cur->next;
			}

		}
		head = dummyhead->next;
		delete dummyhead;
		return head;

	}
};