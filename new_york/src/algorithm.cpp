#include <assert.h>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

// Use global variables to have less on the stack and not run into a stack
// overflow due to the recursive calls
vector<vector<int>> children;
vector<int> path;
multiset<int> temps;
vector<int> temperature;
int difference;
int trip_length;
set<int> result;

// DFS from position pos
void DFS(const int &pos) {
  // Push pos onto stack
  path.push_back(pos);

  temps.insert(temperature[pos]);

  // Trip too long
  if (temps.size() > trip_length) {
    auto rm = path.rbegin()[trip_length];
    auto it = temps.find(temperature[rm]);
    assert(it != temps.end());
    temps.erase(it);
  }

  if (temps.size() == trip_length) {
    auto minElement = *temps.begin();
    auto maxElement = *temps.rbegin();
    if (abs(maxElement - minElement) <= difference) {
      result.insert(path.rbegin()[trip_length - 1]);
    }
  }

  for (auto &child : children[pos]) {
    DFS(child);
  }

  // Pop pos from stack
  path.pop_back();

  // Remove current temperature
  auto it = temps.find(temperature[pos]);
  assert(it != temps.end());
  temps.erase(it);

  // Check if necessary to add temperature that is trip_length away
  if (path.size() >= trip_length) {
    int i = path.rbegin()[trip_length - 1];
    temps.insert(temperature[i]);
  }
}

void testcase() {
  int n;
  cin >> n >> trip_length >> difference;
  temperature = vector<int>(n);

  for (int i = 0; i < n; i++) {
    cin >> temperature[i];
  }

  children = vector<vector<int>>(n);
  for (int i = 0; i < n - 1; i++) {
    int u, v;
    cin >> u >> v;
    children[u].push_back(v);
  }

  path.clear();
  path.reserve(n);
  temps.clear();
  result.clear();
  DFS(0);
  if (result.size() == 0) {
    cout << "Abort mission" << endl;
  } else {
    for (auto t : result) {
      cout << t << " ";
    }
    cout << endl;
  }
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();
}
