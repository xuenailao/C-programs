#include <vector>
#include <iostream>

using namespace std;


class solution {
public:
	int minlen_array(vector<int>& nums, int s) {
		int sum = 0;
		int result = nums.size();
		int begin = 0;

		for (int end = 0; end < nums.size(); end++) {
			sum += nums[end];
			while (sum >= s) {
				if (result > end - begin + 1) {
					result = end - begin + 1;
				}
				sum -= nums[begin++];
			}
		}
		

		return result;
	}
};


int main() {
	solution sol;
	vector<int> nums = { 2,4,5,5,12,2,4 };

	int len = sol.minlen_array(nums, 22);
	cout << len << endl;

}