#include <vector>
#include <iostream>

using namespace std;

class solution
{
public:
    int section_search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;

        while (left <= right) {
            int pos = left + (right - left) / 2; // Calculate mid-point to prevent overflow

            if (nums[pos] == target) {
                return pos; // Target found
            }
            else if (nums[pos] < target) {
                left = pos + 1; // Search in the right half
            }
            else {
                right = pos - 1; // Search in the left half
            }
        }

        // If we exit the loop, the target was not found
        cout << "Not Found" << endl;
        exit(0);
    }
};

int main() {
    vector<int> nums = { -2, 3, 5, 7, 12, 79, 123 };
    int target = 123;
    solution sol;
    int result = sol.section_search(nums, target);
    cout << "Index of target " << target << ": " << result << endl;

    return 0;
}
