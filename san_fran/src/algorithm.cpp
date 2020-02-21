#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

using namespace std;
typedef vector<vector<pair<long, long>>> AdjList;
typedef vector<vector<long>> Memo;  //[pos,k] = maxVal achievable

long recurse(const AdjList &adjList, Memo &memo, long pos, long k) {
  if (k == 0) return 0;

  long maxVal = numeric_limits<long>::min();

  if (memo[pos][k] == -1) {
    if (adjList[pos].size() == 0) {
      // free action
      if (pos == 0) return 0;
      maxVal = max(maxVal, recurse(adjList, memo, 0, k));
    } else {
      for (auto edge : adjList[pos]) {
        maxVal = max(maxVal,
                     recurse(adjList, memo, edge.first, k - 1) + edge.second);
      }
    }
    memo[pos][k] = maxVal;
  }
  return memo[pos][k];
}

void testcase() {
  long n, m, x, k;
  cin >> n >> m >> x >> k;

  AdjList edges(n);  // <pos,value>

  Memo memo(n, vector<long>(k + 1, -1));
  for (int i = 0; i < m; i++) {
    long u, v, p;
    cin >> u >> v >> p;
    if ((u == v) && p == 0) continue;
    edges[u].emplace_back(v, p);
  }
  for (int i = 1; i <= k; i++) {
    recurse(edges, memo, 0, i);
  }

  for (int i = 0; i <= k; i++) {
    if (memo[0][i] >= x) {
      cout << i << endl;
      return;
    }
  }
  cout << "Impossible" << endl;
}
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
