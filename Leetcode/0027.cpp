#include <vector>
#include <iostream>

using namespace std;


class solution
{
public:
	int remove(vector<int>& nums, int val) {
		int len = nums.size();
		int fast = 0;
		int slow = 0;
		int k = 0;

		while (fast<len) {
			if (nums[fast] != val) {
				nums[slow++] = nums[fast];
				k++;
				
			}
			fast++;
		}

		return k;

	}

};


int main() {
	vector<int> nums = { 1,10,4,6,7,10,10, 20 };
	int val = 10;
	solution sol;

	int len = sol.remove(nums, val);
	for (int i = 0; i < len; i++) {
		cout << nums[i] << ' ' << endl;

	}


}