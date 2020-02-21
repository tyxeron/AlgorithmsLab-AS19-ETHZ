#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

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
using namespace std;
void testcase() {
  int n, m, s, d;
  cin >> n >> m >> s >> d;

  graph G(2 * n);
  edge_adder adder(G);
  int source = boost::add_vertex(G);
  int sink = boost::add_vertex(G);

  for (int i = 0; i < n; i++) {
    adder.add_edge(i, i + n, 1);
  }
  for (int i = 0; i < m; i++) {
    int i_, j_;
    cin >> i_ >> j_;
    adder.add_edge(i_ + n, j_, INT_MAX);
  }
  for (int i = 0; i < s; i++) {
    int s_;
    cin >> s_;
    adder.add_edge(source, s_, INT_MAX);
  }
  for (int i = 0; i < d; i++) {
    int d_;
    cin >> d_;
    adder.add_edge(d_ + n, sink, INT_MAX);
  }
  cout << boost::push_relabel_max_flow(G, source, sink) << endl;

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();
}
