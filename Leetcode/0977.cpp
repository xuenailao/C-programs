#include <vector>
#include <iostream>
#include <cmath>

using namespace std;


class solution {
public:
    vector<int> square_sort(vector<int> nums) {
        int n = nums.size();
        vector<int> result(n);
        int left = 0, right = n - 1;
        int pos = n - 1; // Position to insert the largest square

        while (left <= right) {
            int left_square = nums[left] * nums[left];
            int right_square = nums[right] * nums[right];

            if (left_square > right_square) {
                result[pos] = left_square;
                left++;
            }
            else {
                result[pos] = right_square;
                right--;
            }
            pos--;
        }

        return result;
    }
};


int main() {
	vector<int> nums = { -2, -3, 5, -7, 12, 12, -23 };
	solution sol;
	vector<int> nums2 = sol.square_sort(nums);

	for (int element : nums2) {
		cout << element << endl;

	}
	return 0;
}
