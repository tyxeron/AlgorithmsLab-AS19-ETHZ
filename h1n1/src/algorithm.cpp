#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Face_handle Face_handle;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<
    boost::setS, boost::vecS,
    boost::undirectedS,  // Use vecS for the VertexList! Choosing setS for the
                         // OutEdgeList disallows parallel edges.
    boost::no_property,  // interior properties of vertices
    boost::property<boost::edge_weight_t, long>  // interior properties of edges
    >
    Graph;
typedef boost::graph_traits<Graph>::edge_descriptor
    Edge;  // Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor
    Vertex;  // Vertex Descriptor: with vecS vertex list, this is really just an
             // int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator
    EdgeIt;  // to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator
    OutEdgeIt;  // to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type
    WeightMap;  // property map to access the interior property edge_weight_t
using namespace std;
long getMinDist(const int i, const vector<Vertex> &predmap,
                vector<long> &minDist, const WeightMap &w, const Graph &G) {
  if (minDist[i] == -1) {
    Edge e = boost::edge(i, predmap[i], G).first;
    minDist[i] =
        min(LONG_MAX - w[e], getMinDist(predmap[i], predmap, minDist, w, G));
  }
  return minDist[i];
}

void testcase() {
  while (true) {
    int n;
    cin >> n;
    if (n == 0) {
      break;
    }

    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for (int i = 0; i < n; i++) {
      long x, y;
      cin >> x >> y;
      pts.emplace_back(x, y);
    }
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    int V = 1;
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end();
         f++) {
      f->info() = V++;
    }

    Graph G(V);
    WeightMap w = boost::get(boost::edge_weight, G);
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end();
         f++) {
      int v = f->info();
      for (int i = 0; i < 3; i++) {
        int u;
        if (t.is_infinite(f->neighbor(i))) {
          u = 0;
        } else {
          u = f->neighbor(i)->info();
        }

        w[boost::add_edge(v, u, G).first] =
            LONG_MAX -
            (long)CGAL::squared_distance(f->vertex((i + 1) % 3)->point(),
                                         f->vertex((i + 2) % 3)->point());
      }
    }

    std::vector<Vertex> primpredmap(V);
    Vertex start = 0;
    boost::prim_minimum_spanning_tree(
        G,
        boost::make_iterator_property_map(primpredmap.begin(),
                                          boost::get(boost::vertex_index, G)),
        boost::root_vertex(start));

    vector<long> minDist(V, -1);
    minDist[0] = LONG_MAX;
    for (int i = 1; i < V; i++) {
      minDist[i] = getMinDist(i, primpredmap, minDist, w, G);
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
      long x, y, d;
      cin >> x >> y >> d;
      K::Point_2 point(x, y);
      if (CGAL::squared_distance(t.nearest_vertex(point)->point(), point) <
          K::FT(d)) {
        cout << "n";
      } else {
        Face_handle f = t.locate(point);
        if (t.is_infinite(f)) {
          cout << "y";
        } else {
          int start = f->info();
          if (minDist[start] < 4 * d) {
            cout << "n";
          } else {
            cout << "y";
          }
        }
      }
    }
    cout << endl;
  }
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  testcase();
}
