#include <assert.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <stdexcept>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Print Graph
#include <boost/graph/graph_utility.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    Traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    Traits::edge_descriptor>>>>
    Graph;

typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;

using namespace std;

class edge_adder {
  Graph &G;

 public:
  explicit edge_adder(Graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;  // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int n = 0, end1 = 0, end2 = 0, w = 0, offset = 0;
  cin >> w >> n;

  set<pair<int, int>> bricks;
  for (int i = 0; i < n; i++) {
    cin >> end1 >> end2;
    // cout << "Pairs: " << end1 << " " << end2 << endl;
    if (end1 == end2) continue;
    if (end2 < end1) swap(end1, end2);
    if (end1 == 0 && end2 == w) {
      offset++;
    } else {
      bricks.emplace(end1, end2);
    }
  }

  Graph G(w * 2);
  edge_adder adder(G);
  int source = 0;
  int sink = 2 * w - 1;
  
  int valBefore = source;
  int src = source;
  for (auto it = bricks.begin(); it != bricks.end() && it->first != w; it++) {
    if (valBefore != it->first) {
      valBefore = it->first;
      src = it->first * 2;
      adder.add_edge(src - 1, src, 1);
    }

    int target = it->second * 2 - 1;
    adder.add_edge(src, target, 1);
  }

  // boost::print_graph(G, boost::get(boost::vertex_index, G));
  long flow = boost::push_relabel_max_flow(G, source, sink);
  cout << flow + offset << endl;
}
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
