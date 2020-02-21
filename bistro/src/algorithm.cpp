#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

using namespace std;

double floor_to_double(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}
int main() {
  ios_base::sync_with_stdio(false);

  // read number of points
  size_t n;
  cin >> n;
  while (n != 0) {
    // read points
    vector<K::Point_2> existingLoc;
    existingLoc.reserve(n);
    for (size_t i = 0; i < n; ++i) {
      int x, y;
      cin >> x >> y;
      existingLoc.push_back(K::Point_2(x, y));
    }

    // construct triangulation
    Triangulation t;
    t.insert(existingLoc.begin(), existingLoc.end());
    size_t m;
    cin >> m;
    /*
      vector<K::Point_2> newLoc;
      newLoc.reserve(m);*/
    for (size_t i = 0; i < m; i++) {
      int x = 0, y = 0;
      cin >> x >> y;
      K::Point_2 newLoc(x, y);
      auto nearestVert = t.nearest_vertex(newLoc);
      cout << fixed << setprecision(0)
           << floor_to_double(
                  CGAL::squared_distance(nearestVert->point(), newLoc))
           << endl;
    }
    cin >> n;
  }
}
