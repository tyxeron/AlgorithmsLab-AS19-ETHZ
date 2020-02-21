#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

//DP FUUUUCK
void testcase() {
  int n, i, k;
  cin >> n >> i >> k;

  vector<int> numbers;
  numbers.reserve(n);
  for (int i = 0; i < n; i++) {
    int val;
    cin >> val;
    numbers.push_back(val);
  }

  vector<vector<bool>> DP(n, vector<bool>(k, false));

  DP[0][numbers[0] % k] = true;

  for (int i = 1; i < n; i++) {
    for (int j = 0; j < k; j++) {
      DP[i][j] = DP[i - 1][j] || (numbers[i] % k) == j ||
                 DP[i - 1][abs(j - numbers[i]) % k];
    }
  }

  if (DP[n - 1][i]) {
    cout << "yes" << endl;

  } else {
    cout << "no" << endl;
  }

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();
}
