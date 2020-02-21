#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

double computeDP(vector<double> &probs, vector<vector<double>> &memo, int day,
                 int wealth, int m, int n) {
  if (wealth >= m) {
    return 1.0;
  }

  if (day >= n) {
    return 0.0;
  }
  if (memo[day][wealth] == -1) {
    if (wealth <= 0) {
      memo[day][wealth] = 0.0;
      return 0.0;
    }
    double maxProbability = 0.0;

    for (int bet = 0; bet <= wealth; bet++) {
      // Two choices
      maxProbability =
          max(maxProbability,
              probs[day] * computeDP(probs, memo, day + 1, wealth + bet, m, n) +
                  (1.0 - probs[day]) *
                      computeDP(probs, memo, day + 1, wealth - bet, m, n));
    }

    memo[day][wealth] = maxProbability;
  }
  return memo[day][wealth];
}
void testcase() {
  int n, k, m;
  cin >> n >> k >> m;

  vector<double> probabilities(n);
  for (int i = 0; i < n; i++) {
    cin >> probabilities[i];
  }

  vector<vector<double>> memo(n, vector<double>(m, -1));
  std::cout << fixed << setprecision(5)
            << computeDP(probabilities, memo, 0, k, m, n) << endl;

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();
}
