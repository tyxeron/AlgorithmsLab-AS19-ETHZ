#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stack>
#include <stdexcept>
#include <unordered_set>
#include <vector>

using namespace std;

// std::iota(std::begin(permutation), std::end(permutation), 0);

bool isDefusable(vector<int> timings, int n) {
  vector<int> minMap(n);
  minMap[0] = timings[0];

  for (int i = 0; i <= (n - 3) / 2; i++) {
    minMap[2 * i + 1] = min(minMap[i], timings[2 * i + 1]);
    minMap[2 * i + 2] = min(minMap[i], timings[2 * i + 2]);
  }
  sort(minMap.begin(), minMap.end());

  for (int i = 0; i < n; i++) {
    if (i >= minMap[i]) {
      return false;
    }
  }
  return true;
}
void testcase() {
  int n = 0;
  cin >> n;
  vector<int> timings(n);
  for (auto& timing : timings) {
    cin >> timing;
  }

  if (isDefusable(timings, n)) {
    cout << "yes" << endl;
  } else {
    cout << "no" << endl;
  }
}

int main(int argc, char const* argv[]) {
  ios_base::sync_with_stdio(false);
  vector<int> test = {1};

  // TODO
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
