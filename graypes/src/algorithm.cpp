#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

/// BGL
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/enum.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_edges_iterator Edge_iterator;
/// BGL end

double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

using namespace std;
int main(int argc, char const* argv[]) {
  ios_base::sync_with_stdio(false);
  int n = 0;
  cin >> n;
  while (n != 0) {
    Triangulation t;

    vector<K::Point_2> pts;
    pts.reserve(n);
    for (size_t i = 0; i < n; ++i) {
      int x = 0, y = 0;
      cin >> x >> y;
      pts.push_back(K::Point_2(x, y));
    }

    t.insert(pts.begin(), pts.end());
    K::FT minLength;
    bool first = true;
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end();
         ++e) {
      K::FT l = t.segment(e).squared_length();
      if (first || minLength > l) {
        minLength = l;
        first = false;
      }
    }
    cout << ceil_to_double(50 * CGAL::sqrt(minLength)) << endl;
    cin >> n;
  }
}
