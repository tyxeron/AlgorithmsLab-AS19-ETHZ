#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

int playRound(std::vector<std::vector<int>> &memo,
              const std::vector<int> &coins, const int begin, const int end,
              int round, int m, int k) {
  if (begin == end) {
    return round % m == k ? coins[begin] : 0;
  }

  /*
   * Pseudo code for memo:
   * If have seen coin begin to end save or retrieve solution
   */

  if (memo[begin][end] == -1) {
    memo[begin][end] =
        round % m == k
            ? std::max(coins[end] + playRound(memo, coins, begin, end - 1,
                                              round + 1, m, k),
                       coins[begin] + playRound(memo, coins, begin + 1, end,
                                                round + 1, m, k))
            : std::min(playRound(memo, coins, begin, end - 1, round + 1, m, k),
                       playRound(memo, coins, begin + 1, end, round + 1, m, k));
  }
  return memo[begin][end];
}

void testcase() {
  int n = 0, m = 0, k = 0;
  std::cin >> n;
  std::cin >> m;
  std::cin >> k;
  std::vector<int> sovereigns(n);
  std::vector<std::vector<int>> memo(n, std::vector<int>(n, -1));

  for (int i = 0; i < n; i++) {
    std::cin >> sovereigns[i];
  }

  std::cout << playRound(memo, sovereigns, 0, n - 1, 0, m, k) << std::endl;
}

int main(int argc, char const *argv[]) {
  std::ios_base::sync_with_stdio(false);
  int t = 0;
  std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}