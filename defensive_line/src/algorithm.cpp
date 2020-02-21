#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

using namespace std;
int computeDP(const vector<int> &intervals, vector<vector<int>> &memo,
              int start, int mLeft, int n) {
  if (start == n) {
    return -1;  // at end
  }
  // Recall from memo
  if (memo[start][mLeft] == -2) {
    if (mLeft == 1) {
      if (start == n - 1) {
        return intervals[start] == -1 ? -1 : 1;
      } else {
        int maxAchievableWith = intervals[start] - start + 1;
        int maxAchievableWithout =
            computeDP(intervals, memo, start + 1, mLeft, n);
        memo[start][mLeft] = max(maxAchievableWith, maxAchievableWithout);
      }
    } else {
      int best = -1;
      for (int i = start; i < n; i++) {
        if (intervals[i] == -1) {
          continue;
        }

        int intervalLen = intervals[i] - i + 1;
        int maxValueAchievable =
            computeDP(intervals, memo, intervals[i] + 1, mLeft - 1, n);

        if (maxValueAchievable !=
            -1) {  // Otherwise, we can't attack this interval
          best = max(best, intervalLen + maxValueAchievable);
        }
      }

      memo[start][mLeft] = best;
    }
  }
  return memo[start][mLeft];
}
void testcase() {
  int n = 0, m = 0, k = 0;
  cin >> n >> m >> k;
  vector<int> defensiveValue(n);

  for (auto &defVal : defensiveValue) {
    cin >> defVal;
  }
  vector<int> intervals(n, -1);
  // Get all intervals with a sliding window
  int startI = 0, endI = 0;
  int slidingWindow = defensiveValue[0];
  int numInterval = 0;
  while (endI < n) {
    if (slidingWindow == k) {
      intervals[startI] = endI;
      numInterval++;
    }
    if (slidingWindow <= k) {
      slidingWindow += defensiveValue[++endI];

    } else {
      slidingWindow -= defensiveValue[startI++];
    }
  }
  if (numInterval < m) {
    cout << "fail" << endl;
    return;
  }

  vector<vector<int>> memo(n, vector<int>(m + 1, -2));
  int output = computeDP(intervals, memo, 0, m, n);
  if (output == -1) {
    cout << "fail" << endl;
  } else {
    cout << output << endl;
  }
  // Top down DP from here
}

int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
