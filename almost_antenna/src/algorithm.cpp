#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt KR;
typedef CGAL::Min_circle_2_traits_2<KR> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

// TODO only 99

double ceil_to_double(const KR::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}
double calculateMinRadius(std::vector<KR::Point_2>& P, int indexToIgnore) {
  Min_circle mc;
  for (int i = 0; i < P.size(); i++) {
    if (i == indexToIgnore) continue;
    mc.insert(P[i]);
  }

  return ceil_to_double(CGAL::sqrt(mc.circle().squared_radius()));
}

int main(int argc, char const* argv[]) {
  std::ios_base::sync_with_stdio(false);
  int n = 0;
  long x = 0, y = 0;
  std::cin >> n;

  while (n != 0) {
    std::vector<KR::Point_2> P(n);
    double minRadius = std::numeric_limits<double>::max();

    for (auto& point : P) {
      std::cin >> x;
      std::cin >> y;
      point = KR::Point_2(x, y);
    }
    std::random_shuffle(P.begin(), P.end());

    Min_circle mc(P.begin(), P.begin() + n, false);

    Traits::Circle c = mc.circle();

    // calulate boundary points
    for (int i = 0; i < n; i++) {
      if (mc.has_on_boundary(P[i])) {
        minRadius = std::min(minRadius, calculateMinRadius(P, i));
      }
    }

    std::cout << std::setprecision(0) << std::fixed << minRadius << "\n";
    std::cin >> n;
  }
  std::cout << std::flush;
  return 0;
}
