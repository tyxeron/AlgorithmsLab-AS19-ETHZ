#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <stdexcept>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    Traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<
        boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t,
                                        Traits::edge_descriptor> > > >
    Graph;
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type
    EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type
    ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
  Graph &G;
  EdgeCapacityMap &capacitymap;
  ReverseEdgeMap &revedgemap;

 public:
  // to initialize the Object
  EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap)
      : G(G), capacitymap(capacitymap), revedgemap(revedgemap) {}

  // to use the Function (add an edge)
  void addEdge(int from, int to, long capacity) {
    Edge e, rev_e;
    bool success;
    boost::tie(e, success) = boost::add_edge(from, to, G);
    boost::tie(rev_e, success) = boost::add_edge(to, from, G);
    capacitymap[e] = capacity;
    capacitymap[rev_e] = 0;
    revedgemap[e] = rev_e;
    revedgemap[rev_e] = e;
  }
};

using namespace std;
void testcase() {
  int n = 0, m = 0;
  cin >> n >> m;
  Graph G(n);
  EdgeCapacityMap capacity = boost::get(boost::edge_capacity, G);
  ReverseEdgeMap rev_edge = boost::get(boost::edge_reverse, G);
  EdgeAdder adder(G, capacity, rev_edge);

  for (int i = 0; i < m; i++) {
    int a = 0, b = 0, c = 0;
    cin >> a >> b >> c;
    adder.addEdge(a, b, c);
  }

  int maxFlow = numeric_limits<int>::max();
  int bestSource = -1;
  int bestSink = -1;
  for (int i = 1; i < n; i++) {
    int tmpFlow1 = boost::push_relabel_max_flow(G, 0, i);
    int tmpFlow2 = boost::push_relabel_max_flow(G, i, 0);
    if (tmpFlow1 < maxFlow) {
      bestSource = 0;
      bestSink = i;
      maxFlow = tmpFlow1;
    }
    if (tmpFlow2 < maxFlow) {
      bestSource = i;
      bestSink = 0;
      maxFlow = tmpFlow2;
    }
  }

  int flow = boost::push_relabel_max_flow(G, bestSource, bestSink);

  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  // BFS to find vertex set S
  std::vector<int> vis(n, false);  // visited flags
  std::queue<int> Q;               // BFS queue (from std:: not boost::)
  int numFigurines = 1;
  vis[bestSource] = true;  // Mark the source as visited
  Q.push(bestSource);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    OutEdgeIt ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend;
         ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // Only follow edges with spare capacity
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      numFigurines++;
      Q.push(v);
    }
  }

  cout << flow << endl;
  cout << numFigurines;
  for (int i = 0; i < n; i++) {
    if (vis[i]) cout << " " << i;
  }

  cout << endl;
}
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  //
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
