#include <assert.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <unordered_set>
#include <vector>

using namespace std;

pair<int, int> min(const pair<int, int> &p1, const pair<int, int> &p2) {
  return (p1.first < p2.first ||
          (p1.first == p2.first && p1.second > p2.second))
             ? p1
             : p2;
}

// We start by adding the current Beveragse and recursively add more of it, add
// from the next beverage or ignore the current beverage
pair<int, int> calculateOptimumPunch(
    vector<vector<pair<int, int>>>
        &memo,  //[index][volume]<cost,variety> of best punch so far
    const vector<pair<int, int>> &beverages,  // <cost,volume>
    int i, int v) {                           // i:index, v:volume
  if (memo[i][v].first != -1) {
    return memo[i][v];
  }
  auto thisBeverage = beverages[i];

  if (i == 0) {
    int currAmount = (v + thisBeverage.second - 1) / thisBeverage.second;
    memo[i][v] = make_pair(thisBeverage.first * currAmount, currAmount > 0);
  } else if (v == 0) {
    memo[i][v] = make_pair(0, 0);
  } else {
    // Either add charge of this beverage, from the next one or igrnore this
    // beverage
    int newVolume = max(v - thisBeverage.second, 0);
    auto sameVarietyPunch =
        calculateOptimumPunch(memo, beverages, i, newVolume);
    sameVarietyPunch.first += thisBeverage.first;

    auto improvedVarietyPunch =
        calculateOptimumPunch(memo, beverages, i - 1, newVolume);
    improvedVarietyPunch.second++;
    improvedVarietyPunch.first += thisBeverage.first;

    auto leaveOutPunch = calculateOptimumPunch(memo, beverages, i - 1, v);

    memo[i][v] =
        min(improvedVarietyPunch, min(leaveOutPunch, sameVarietyPunch));
  }

  return memo[i][v];
}

void testcase() {
  int n = 0, k = 0;
  std::cin >> n >> k;
  vector<pair<int, int>> beverages(n);
  for (auto &beverage : beverages) {
    std::cin >> beverage.first >> beverage.second;
  }

  vector<vector<pair<int, int>>> memo(
      n, vector<pair<int, int>>(k + 1, pair<int, int>(-1, 0)));
  auto result = calculateOptimumPunch(memo, beverages, n - 1, k);
  cout << result.first << " " << result.second << endl;
}

int main(int argc, char const *argv[]) {
  std::ios_base::sync_with_stdio(false);
  int t = 0;
  std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
  return 0;
}
