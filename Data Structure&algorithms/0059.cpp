#include <vector>
#include <iostream>

using namespace std;

//1 2 3 4 5
//16 17 18 19 6
//15 24 25 20 7
//14 23 22 21 8
//13 12 11 10 9

class solution {
public:
	vector<vector<int>> clockwise_matrix(int n) {
		int row = 0;
		int col = 0;
		int len = n;
		int num = 1;

		vector<vector<int>> nums(n, vector<int>(n, 0));
		
		while (len > 0) {
			// 向右
			while (col >= row && col < n - row -1) {
				nums[row][col] = num++;
				cout << nums[row][col] << endl;
				col++;
			}

			//向下
			while (col > row && col >= n - row - 1) {
				nums[row][col] = num++;
				cout << nums[row][col] << endl;
				row++;
			}

			//向左
			while (col <= row && col > n - row - 1) {
				nums[row][col] = num++;
				cout << nums[row][col] << endl;
				col--;
			}
			
			//向上
			while (col < row && col <= n - row - 1) {
				nums[row][col] = num++;
				cout << nums[row][col] << endl;
				row--;
			}

			if (len==1) {
				nums[row][col] = num;
			}

			//缩减方阵
			len = len-2;
			row++;
			col++;
			
		}
		return nums;
	}
};


int main() {
	solution sol;
	int n = 5;
	vector<vector<int>> vec = sol.clockwise_matrix(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << vec[i][j]<<' ';
		}
		cout<< endl;
	}
}