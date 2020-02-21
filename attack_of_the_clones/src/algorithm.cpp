#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace std;

bool cmp(const pair<int, int>& p1, const pair<int, int>& p2) {
  return (p1.second < p2.second ||
          (p1.second == p2.second && p1.first > p2.first));
}

int greedy(const vector<pair<int, int>>& pairs, int start, int end) {
  int result = 0;
  int prev_end = start;
  for (auto curr : pairs) {
    if (curr.first > prev_end && curr.second < end) {
      result++;
      prev_end = curr.second;
    }
  }
  return result;
}

void testcase() {
  int n, m;
  cin >> n >> m;

  vector<pair<int, int>> jedis(n);
  map<int, int> segments;

  int a, b;
  for (int i = 0; i < n; ++i) {
    cin >> a >> b;
    a--;
    segments[a]++;
    segments[b % m]--;
    b--;
    jedis[i] = make_pair(a, b);
    if (a > b) segments[0]++;
  }

  vector<pair<int, int>> nonOverlappingJedis;
  vector<pair<int, int>> overlappingJedis;
  int counter = 0;
  int zeroSegment = 0;
  int currMin = numeric_limits<int>::max();

  for (auto segment : segments) {
    counter += segment.second;
    if (counter < currMin) {
      currMin = counter;
      zeroSegment = segment.first;
    }
  }

  // Shift segments s.t. zero_segment is at 0
  for (int i = 0; i < n; ++i) {
    a = jedis[i].first - zeroSegment;
    if (a < 0) a += m;

    b = jedis[i].second - zeroSegment;
    if (b <= 0) b += m;

    if (a > b) {
      overlappingJedis.push_back(make_pair(a, b));
    } else {
      nonOverlappingJedis.push_back(make_pair(a, b));
    }
  }

  
  sort(nonOverlappingJedis.begin(), nonOverlappingJedis.end(), cmp);
  //Only use non overlapping jedi segments
  int maxSegments = greedy(nonOverlappingJedis, -1, m);
  int result;
  //Force selection of overlapping jedi segments one by one,
  for (auto elem : overlappingJedis) {
    result = greedy(nonOverlappingJedis, elem.second, elem.first);
    result++;
    if (result > maxSegments) {
      maxSegments = result;
    }
  }

  // Output result
  cout << maxSegments << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}