#include<vector>
using std::vector;


class solution {
public:
	int climb_ladder(int N){
		vector<int> dp(N+1);
		dp[0] = 0;
		dp[1] = 1;

		for (int i = 2; i < N + 1; i++) {
			dp[i] = dp[i - 1] + dp[i - 2];
		}
		


	}
}