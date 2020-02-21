#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <unordered_set>
#include <vector>

using namespace std;

int nextBoat(vector<pair<int, int>> &boatDesc) {
  int minPos = numeric_limits<int>::min();
  int minPosBefore = numeric_limits<int>::min();

  int boatCount = 0;
  for (auto boat : boatDesc) {
    if (minPos > boat.first) {
      minPos = min(minPos, max(boat.first, minPosBefore + boat.second));
    } else {
      boatCount++;
      minPosBefore = minPos;
      minPos = max(boat.first, minPos + boat.second);
    }
  }
  return boatCount;
}
void testcase() {
  int n = 0;
  cin >> n;
  vector<pair<int, int>> boatDesc(n);

  for (int i = 0; i < n; i++) {
    cin >> boatDesc[i].second >> boatDesc[i].first;
  }
  sort(boatDesc.begin(), boatDesc.end());

  cout << nextBoat(boatDesc) << endl;
}
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  // TODO
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
