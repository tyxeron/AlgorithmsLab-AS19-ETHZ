#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <unordered_set>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_edges_iterator Edge_iterator;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Vertex_circulator Vertex_ciruclator;
typedef Triangulation::Edge_circulator Edge_circulator;
typedef Triangulation::Point Point;
using namespace std;

double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

int time(double sqDist) { return max(0.0, ceil(sqrt(sqrt(sqDist) - 0.5))); }
int main() {
  // read number of points
  size_t n = 0;
  cin >> n;
  while (n != 0) {
    int l, b, r, s;
    cin >> l >> b >> r >> s;

    K::Point_2 bottomLeft(l, b);
    K::Point_2 bottomRight(r, b);
    K::Point_2 topLeft(l, s);

    K::Point_2 topRight(r, s);

    vector<K::Segment_2> dishBounds;
    dishBounds.push_back(K::Segment_2(topLeft, topRight));
    dishBounds.push_back(K::Segment_2(bottomRight, topRight));
    dishBounds.push_back(K::Segment_2(bottomLeft, bottomRight));
    dishBounds.push_back(K::Segment_2(bottomLeft, topLeft));

    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
      int x, y;
      std::cin >> x >> y;
      pts.push_back(K::Point_2(x, y));
    }
    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    vector<int> deathTimes;
    deathTimes.reserve(n);

    for (Vertex_iterator v = t.finite_vertices_begin();
         v != t.finite_vertices_end(); ++v) {
      double minUpper = CGAL::squared_distance(v->point(), dishBounds[0]);
      double minRight = CGAL::squared_distance(v->point(), dishBounds[1]);
      double minBottom = CGAL::squared_distance(v->point(), dishBounds[2]);
      double minLeft = CGAL::squared_distance(v->point(), dishBounds[3]);

      double minTime = min(minUpper, min(minBottom, min(minLeft, minRight)));

      Edge_circulator e = v->incident_edges();
      do {
        if (!t.is_infinite(e)) {
          auto sqDist =
              t.segment(e).squared_length() /
              4;  // REMBER to divide by 4 as we are interested in half distance
          minTime = min(minTime, sqDist);
        }
      } while (++e != v->incident_edges());

      deathTimes.push_back(time(minTime));
    }

    sort(deathTimes.begin(), deathTimes.end());

    int first = deathTimes[0];
    int last = deathTimes[n - 1];
    int mid = deathTimes[n / 2];
    cout << first << " " << mid << " " << last << endl;
    cin >> n;
  }
}
