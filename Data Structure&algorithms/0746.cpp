#include<vector>
using namespace std;
using std::vector;

class solution {
public:
	int climb_ladder_wise(vector<int> cost) {
		int N = cost.size();
		vector<int> dp(N); 

		dp[0] = 0;
		dp[1] = 0;
		for (int i = 0; i < N; i++) {
			dp[i] = min(dp[i - 1] + cost[i - 1], dp[i - 2] + cost[i - 2]);
		}

		return dp[N];

	}
};