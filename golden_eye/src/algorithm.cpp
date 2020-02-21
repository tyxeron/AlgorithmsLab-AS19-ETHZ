#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_vertices_iterator Vertices_iterator;
typedef Triangulation::Finite_edges_iterator Edge_Iterator;
typedef Triangulation::Vertex_handle Vert;

typedef std::tuple<int, int, K::FT> Edge;
typedef std::vector<Edge> EdgeV;

using namespace std;

void testcase() {
  int n, m;
  long p;
  cin >> n >> m >> p;
  Triangulation t;
  vector<pair<K::Point_2, int>> pts;
  pts.reserve(n);

  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    pts.emplace_back(K::Point_2(x, y), i);
  }

  t.insert(pts.begin(), pts.end());

  EdgeV edges;
  edges.reserve(3 * n);  // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Vert vs = e->first->vertex((e->second + 1) % 3);
    Vert vt = e->first->vertex((e->second + 2) % 3);
    int i1 = vs->info();
    int i2 = vt->info();
    if (i1 > i2) std::swap(i1, i2);
    auto segLen = t.segment(e).squared_length();
    edges.emplace_back(i1, i2, segLen);
  }
  std::sort(edges.begin(), edges.end(),
            [](const Edge& e1, const Edge& e2) -> bool {
              return std::get<2>(e1) < std::get<2>(e2);
            });

  // setup and initialize union-find data structure

  K::FT b = 0, a = 0;
  boost::disjoint_sets_with_storage<> ufa(n);
  boost::disjoint_sets_with_storage<> ufb(n);
  auto ai = edges.begin();
  auto bi = edges.begin();

  boost::disjoint_sets_with_storage<> ufp(n);
  typedef std::vector<Edge>::const_iterator ECI;
  for (ECI e = edges.begin(); e != edges.end() && std::get<2>(*e) <= p; ++e)
    ufp.union_set(std::get<0>(*e), std::get<1>(*e));

  for (std::size_t i = 0; i < m; ++i) {
    int x0, y0, x1, y1;
    std::cin >> x0 >> y0 >> x1 >> y1;
    K::Point_2 p0(x0, y0), p1(x1, y1);
    Vert v0 = t.nearest_vertex(p0);
    Vert v1 = t.nearest_vertex(p1);
    K::FT d = 4 * std::max(CGAL::squared_distance(p0, v0->point()),
                           CGAL::squared_distance(p1, v1->point()));
    if (d <= p && ufp.find_set(v0->info()) == ufp.find_set(v1->info())) {
      // mission possible with power p -> also with b
      std::cout << "y";
      if (d > b) b = d;
      for (; bi != edges.end() &&
             ufb.find_set(v0->info()) != ufb.find_set(v1->info());
           ++bi)
        ufb.union_set(std::get<0>(*bi), std::get<1>(*bi));
    } else
      std::cout << "n";
    // ensure it is possible at power a
    if (d > a) a = d;
    for (; ai != edges.end() &&
           ufa.find_set(v0->info()) != ufa.find_set(v1->info());
         ++ai)
      ufa.union_set(std::get<0>(*ai), std::get<1>(*ai));
  }
  if (ai != edges.begin() && std::get<2>(*(ai - 1)) > a)
    a = std::get<2>(*(ai - 1));
  if (bi != edges.begin() && std::get<2>(*(bi - 1)) > b)
    b = std::get<2>(*(bi - 1));
  std::cout << std::fixed << std::setprecision(0) << "\n"
            << a << "\n"
            << b << "\n";
}

int main(int argc, char const* argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
