#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

double floor_to_double(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

int main(int argc, char const* argv[]) {
  std::ios_base::sync_with_stdio(false);

  int n = 0;
  long x = 0, y = 0, a = 0, b = 0;
  long r = 0, s = 0, t = 0, u = 0;
  bool hit = false;
  R ray;
  S raySegment;

  std::cin >> n;

  while (n != 0) {
    hit = false;

    std::cin >> x;
    std::cin >> y;
    std::cin >> a;
    std::cin >> b;

    ray = R(P(x, y), P(a, b));
    std::vector<S> segments(n);

    for (auto & segment : segments) {
      std::cin >> r;
      std::cin >> s;
      std::cin >> t;
      std::cin >> u;
      segment = S(P(r, s), P(t, u));
    }

    std::random_shuffle(segments.begin(), segments.end());

    boost::optional<boost::variant<K::Point_2, K::Segment_2> > intersection;

    for (auto segment : segments) {
      if (hit) {
        if (CGAL::do_intersect(segment, raySegment))
          intersection = CGAL::intersection(segment, raySegment);
        else
          continue;

      } else {
        if (CGAL::do_intersect(segment, ray))
          intersection = CGAL::intersection(segment, ray);
        else
          continue;
      }

      if (intersection) {
        if (const P* op = boost::get<P>(&*intersection)) {
          if (hit) {
            raySegment = S(raySegment.source(), *op);
          } else {
            raySegment = S(ray.source(), *op);
          }

        } else if (const S* os = boost::get<S>(&*intersection)) {
          if (hit) {
            auto distance1 = CGAL::to_double(
                CGAL::squared_distance(os->source(), raySegment.source()));
            auto distance2 = CGAL::to_double(
                CGAL::squared_distance(os->target(), raySegment.source()));

            if (distance1 < distance2) {
              raySegment = S(raySegment.source(), os->source());
            } else {
              raySegment = S(raySegment.source(), os->target());
            }

          } else {
            // segment
            auto distance1 = CGAL::to_double(
                CGAL::squared_distance(os->source(), ray.source()));
            auto distance2 = CGAL::to_double(
                CGAL::squared_distance(os->target(), ray.source()));

            if (distance1 < distance2) {
              raySegment = S(ray.source(), os->source());
            } else {
              raySegment = S(ray.source(), os->target());
            }
          }
        } else
          throw std::runtime_error("strange segment intersection");

        hit = true;
      }
    }

    std::cin >> n;

    if (hit) {
      std::cout << std::setprecision(0) << std::fixed
                << floor_to_double(raySegment.target()[0]) << " "
                << floor_to_double(raySegment.target()[1]) << std::endl;
    } else {
      std::cout << "no" << std::endl;
    }
  }
  return 0;
}
