#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

using namespace std;
void testcase() {
  // TODO:
  int m, n;
  cin >> m >> n;
  vector<pair<K::Point_2, int>> participants;
  participants.reserve(m);
  for (int i = 0; i < m; i++) {
    int x, y;
    int r;
    cin >> x >> y >> r;
    participants.push_back(make_pair(K::Point_2(x, y), r));
  }
  int h;
  cin >> h;

  vector<pair<K::Point_2, int>> lamps;
  lamps.reserve(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    lamps.emplace_back(K::Point_2(x, y), i);
  }
  Triangulation t;
  t.insert(lamps.begin(), lamps.end());

  vector<int> winners;

  int maxHit = numeric_limits<int>::min();
  vector<int> firsthit(m, -1);
  for (int i = 0; i < m; i++) {
    auto participant = participants[i];

    if (CGAL::squared_distance(participant.first,
                               t.nearest_vertex(participant.first)->point()) >=
        pow(participant.second + h, 2)) {
      winners.push_back(i);
    } else {
      for (int j = 0; j < n; j++) {
        if (CGAL::squared_distance(participant.first, lamps[j].first) <
            pow(participant.second + h, 2)) {
          firsthit[i] = j;
          maxHit = max(maxHit, j);
          break;
        }
      }
    }
  }
  if (winners.size() == 0) {
    for (int i = 0; i < m; i++) {
      if (firsthit[i] == maxHit) {
        winners.push_back(i);
      }
    }
  }
  sort(winners.begin(), winners.end());

  for (int i = 0; i < winners.size(); i++) {
    cout << winners[i];
    if (i < winners.size()) {
      cout << " ";
    }
  }

  cout << endl;
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();
}
