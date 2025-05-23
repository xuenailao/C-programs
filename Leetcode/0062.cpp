#include<vector>
using std::vector;


class solution {
public:
	int botpath(int m, int n) {
		// m down, n right
		vector<vector<int>> dp(m, vector<int>(n, 0));
		for (int i = 0; i < m; i++) dp[i][0] = 1;
		for (int j = 0; j < n; j++) dp[0][j] = 1;

		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				dp[i][j] = dp[i][j - 1] + dp[i - 1][j];
			}
		}

		return dp[m - 1][n - 1];

	}
};