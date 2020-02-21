#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

using namespace std;
/*

int computeDP(const vector<int>& A, const vector<int>& B,
              vector<vector<int>>& memo, const vector<int>& partialSumA,
              const vector<int>& partialSumB, int n, int aBegin, int bBegin) {
  int minVal = std::numeric_limits<int>::max();
  if (memo[aBegin][bBegin] == -1) {
    if (aBegin == n - 1 || bBegin == n - 1) {
      return (partialSumA[n] - partialSumA[aBegin] - (A.size() - aBegin)) *
             (partialSumB[n] - partialSumB[bBegin] - (B.size() - bBegin));
    }
    for (int a = aBegin + 1; a < n; a++) {
      for (int b = bBegin + 1; b < n; b++) {
        int val = computeDP(A, B, memo, partialSumA, partialSumB, n, a, b);
        if (val == std::numeric_limits<int>::max()) continue;
        minVal = min(
            minVal,
            val + (partialSumA[a] - partialSumA[aBegin] - (a - aBegin)) *
                      (partialSumB[b] - partialSumB[bBegin] - (b - bBegin)));
      }
    }
    memo[aBegin][bBegin] = minVal;
  }

  return memo[aBegin][bBegin];
}
*/
void testcase() {
  int n = 0;
  cin >> n;
  vector<int> A(n);

  vector<int> B(n);

  for (int i = 0; i < n; i++) {
    cin >> A[i];
    A[i]--;
  }
  for (int i = 0; i < n; i++) {
    cin >> B[i];
    B[i]--;
  }
  vector<int> partialSumA(n);
  vector<int> partialSumB(n);

  partialSumA[0] = A[0];
  partialSumB[0] = B[0];

  for (int i = 1; i < n; i++) {
    partialSumA[i] = partialSumA[i - 1] + A[i];
    partialSumB[i] = partialSumB[i - 1] + B[i];
  }

  vector<vector<int>> memo(n, vector<int>(n, 0));
  // cout << computeDP(A, B, memo, partialSumA, partialSumB, n, 0, 0) << endl;
  memo[0][0] = A[0] * B[0];

  for (int i = 1; i < n; i++) {
    memo[i][0] = partialSumA[i] * B[0];
    memo[0][i] = A[0] * partialSumB[i];
  }

  for (int i = 1; i < n; i++) {
    for (int j = 1; j < n; j++) {
      int prev = min(memo[i - 1][j - 1], min(memo[i][j - 1], memo[i - 1][j]));
      memo[i][j] = A[i] * B[j] + prev;
    }
  }

  cout << memo[n - 1][n - 1] << endl;
}
int main(int argc, char const* argv[]) {
  ios_base::sync_with_stdio(false);
  // TODO
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
