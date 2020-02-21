#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <set>
#include <stdexcept>
#include <vector>

using namespace std;
void testcase() {
  int n = 0, m = 0;
  cin >> n >> m;
  vector<int> friends;
  friends.reserve(n);
  vector<int> boxes;
  boxes.reserve(m);

  for (int i = 0; i < n; i++) {
    int val = 0;
    cin >> val;
    friends.push_back(val);
  }

  sort(friends.begin(), friends.end());
  reverse(friends.begin(), friends.end());

  multiset<int, greater<int>> w;
  for (int i = 0; i < m; i++) {
    int wi;
    cin >> wi;
    w.insert(wi);
  }

  if (*w.begin() > friends[0]) {
    cout << "impossible" << endl;
    return;
  }

  int trips = 0;
  while (!w.empty()) {
    trips++;
    for (int i = 0; i < n; i++) {
      auto box = w.lower_bound(friends[i]);
      if (box != w.end()) {
        w.erase(box);
      } else {
        break;
      }
    }
  }
  cout << 2 * trips + (trips - 1) << endl;
}
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
