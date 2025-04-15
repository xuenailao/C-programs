#include <vector>
#include <iostream>

using namespace std;


int main() {
	int n = 0;
	int m = 0;
	int sum = 0;
	cin >> n >> m;

	vector<vector<int>> vec(n, vector<int>(m, 0) ) ;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> vec[i][j];
			sum += vec[i][j];

		}
	}
	
	int result = INT_MAX;
	int count = 0; // 统计遍历过的行
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			count += vec[i][j];
			if (j == m - 1) result = min(result, abs(sum - count - count));

		}
	}
	
	count = 0; // 统计遍历过的列
	for (int j = 0; j < m; j++) {
		for (int i = 0; i < n; i++) {
			count += vec[i][j];
			// 遍历到列末尾的时候开始统计
			if (i == n - 1) result = min(result, abs(sum - count - count));
		}
	}
	cout << result << endl;
	
}