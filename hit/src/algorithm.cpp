#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

int main(int argc, char const *argv[]) {
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;
  while (n != 0) {
    long x, y, a, b;
    std::cin >> x >> y >> a >> b;
    R ray(P(x, y), P(a, b));

    std::vector<S> segments;
    segments.reserve(n);
    for (int i = 0; i < n; i++) {
      long r, s, t, u;
      std::cin >> r >> s >> t >> u;
      segments.push_back(S(P(r, s), P(t, u)));
    }

    bool doIntersect = false;
    for (const auto &segment : segments) {
      if (CGAL::do_intersect(segment, ray)) {
        doIntersect = true;
        break;
      }
    }
    std::cout << (doIntersect ? "yes" : "no") << "\n";

    std::cin >> n;
  }
}
