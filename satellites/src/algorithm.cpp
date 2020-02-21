#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <stdexcept>
#include <vector>

#include <boost/graph/graphviz.hpp>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<
        boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t,
                                        traits::edge_descriptor> > > >
    graph;
// Interior Property Maps
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;  // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

using namespace std;
void testcase() {
  int g = 0, s = 0, l = 0;
  cin >> g >> s >> l;
  // build graph
  const int N = g + s + 2;
  graph G(N);
  edge_adder adder(G);

  int src = 0;
  int sink = N - 1;

  for (int i = 0; i < g; i++) {
    adder.add_edge(src, i + 1, 1);
  }
  for (int i = 0; i < s; i++) {
    adder.add_edge(i + g + 1, sink, 1);
  }

  for (int i = 0; i < l; i++) {
    int a = 0, b = 0;
    cin >> a >> b;
    adder.add_edge(a + 1, b + g + 1, 1);
  }

  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  // Find a min cut via maxflow
  int flow = boost::push_relabel_max_flow(G, src, sink);
  // std::cout << "maximum flow = minimum cut = " << flow << "\n";

  // BFS to find vertex set S
  std::vector<int> vis(N, false);  // visited flags
  std::queue<int> Q;               // BFS queue (from std:: not boost::)
  vis[src] = true;                 // Mark the source as visited
  Q.push(src);

  int gNum = g;
  int sNum = 0;
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend;
         ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // Only follow edges with spare capacity
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      if (v >= 1 && v <= g)
        gNum--;
      else if (v >= 1 + g && v < sink)
        sNum++;
      Q.push(v);
    }
  }
  cout << gNum << " " << sNum << endl;
  if (gNum + sNum > 0) {
    // Output S
    for (int i = 1; i < g + 1; ++i) {
      if (!vis[i]) std::cout << i - 1 << " ";
    }
    for (int i = g + 1; i < N - 1; ++i) {
      if ((vis[i])) std::cout << i - 1 - g << " ";
    }
    std::cout << "\n";
  }
}
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
