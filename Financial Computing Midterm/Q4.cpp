#include <iostream>
#include <vector>
#include <algorithm>
#include <functional> 

using namespace std;

const int N = 6; // Assume there are 6 stocks
double threshold = 0.5;

double covariance[N][N] = {
    {0, 0.8, 0.1, 0.1, 0.8, 0.1}, // A row
    {0.8, 0, 0.1, 0.8, 0.8, 0.1}, // B row
    {0.1, 0.1, 0, 0.1, 0.8, 0.1}, // C row
    {0.1, 0.8, 0.1, 0, 0.8, 0.1}, // D row
    {0.8, 0.8, 0.8, 0.8, 0, 0.8}, // E row
    {0.1, 0.1, 0.1, 0.1, 0.8, 0}  // F row
};


char stockLabels[N] = { 'A', 'B', 'C', 'D', 'E', 'F' }; // Labels for stocks

// Build the adjacency matrix of the graph
vector<vector<int>> buildGraph(double threshold) {
    vector<vector<int>> adj(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (covariance[i][j] > threshold) {
                adj[i][j] = adj[j][i] = 1;
            }
        }
    }

    // Print adjacency matrix for debugging
    cout << "Adjacency Matrix:" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
    return adj;
}

// Find the maximum independent set (disparate group)
int maxIndependentSet(vector<vector<int>>& adj, vector<int>& result) {
    vector<int> temp;
    int maxSize = 0;

    function<void(int)> dfs = [&](int node) {
        bool isIndependent = true;
        for (int v : temp) {
            if (adj[node][v]) {
                isIndependent = false;
                break;
            }
        }
        if (isIndependent) {
            temp.push_back(node);
            if (temp.size() > maxSize) {
                maxSize = temp.size();
                result = temp;
            }
            for (int i = node + 1; i < N; ++i) {
                dfs(i);
            }
            temp.pop_back();
        }
        };

    for (int i = 0; i < N; ++i) {
        dfs(i);
    }
    return maxSize;
}

// Find all maximum cliques (friendly groups)
int maxClique(vector<vector<int>>& adj, vector<vector<int>>& allCliques) {
    vector<int> temp;
    int maxSize = 0;

    function<void(int)> dfs = [&](int node) {
        bool isClique = true;
        for (int v : temp) {
            if (!adj[node][v]) {
                isClique = false;
                break;
            }
        }
        if (isClique) {
            temp.push_back(node);
            if (temp.size() > maxSize) {
                maxSize = temp.size();
                allCliques.clear(); // Clear previous cliques as we found a larger one
                allCliques.push_back(temp);
            }
            else if (temp.size() == maxSize) {
                allCliques.push_back(temp); // Add clique of the same max size
            }
            for (int i = node + 1; i < N; ++i) {
                dfs(i);
            }
            temp.pop_back();
        }
        };

    for (int i = 0; i < N; ++i) {
        dfs(i);
    }
    return maxSize;
}


int main() {
    vector<vector<int>> adj = buildGraph(threshold);

    // Find the maximum independent set (disparate group)
    vector<int> independentSet;
    int maxIndependentSize = maxIndependentSet(adj, independentSet);
    cout << "Maximum Disparate Group Size: " << maxIndependentSize << endl;
    cout << "Disparate Group Stock Labels: ";
    for (int stock : independentSet) cout << stockLabels[stock] << " ";
    cout << endl;

    // Find all maximum cliques (friendly groups)
    vector<vector<int>> allCliques;
    int maxCliqueSize = maxClique(adj, allCliques);
    cout << "Maximum Friendly Group Size: " << maxCliqueSize << endl;

    cout << "All Maximum Friendly Groups (Cliques):" << endl;
    for (const auto& clique : allCliques) {
        for (int stock : clique) {
            cout << stockLabels[stock] << " ";
        }
        cout << endl;
    }


    return 0;
}
