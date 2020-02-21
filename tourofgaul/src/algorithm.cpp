#include <algorithm>
#include <cassert>
// #include <chrono>  // for high_resolution_clock
#include <climits>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<
        boost::edge_capacity_t, long,
        boost::property<
            boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                            boost::property<boost::edge_weight_t, long> > > > >
    graph;  // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;  // Iterator

// Custom edge adder class
class edge_adder {
  graph& G;

 public:
  explicit edge_adder(graph& G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);  // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;  // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;       // new assign cost
    w_map[rev_e] = -cost;  // new negative cost
  }
};
void testcase() {
  int n, m;
  cin >> n >> m;

  graph g(n + 2);
  edge_adder adder(g);

  auto c_map = boost::get(boost::edge_capacity, g);
  auto r_map = boost::get(boost::edge_reverse, g);
  auto rc_map = boost::get(boost::edge_residual_capacity, g);
  int sink = boost::num_vertices(g) - 1;
  int source = sink - 1;
  // for the first three testcases these values are all the same

  for (int i = 0; i < n - 1; i++) {
    int stop;
    cin >> stop;
    adder.add_edge(i, i + 1, stop, 128);
    adder.add_edge(i + 1, sink, stop, 0);

    adder.add_edge(source, i, stop, 0);
  }

  // read in food routes
  for (int i = 0; i < m; i++) {
    int a, b, d;
    cin >> a >> b >> d;
    adder.add_edge(a, b, 1, -d + (b - a) * 128);
  }
  
  int s_flow = 0;
  out_edge_it e, eend;
  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, g), g);
       e != eend; ++e) {
    s_flow += c_map[*e] - rc_map[*e];
  }
  boost::successive_shortest_path_nonnegative_weights(g, source, sink);
  int cost = boost::find_flow_cost(g);

  cout << 128 * s_flow - cost << endl;
  //   ofstream file("graph.dot");
  //   write_graphviz(file, g);
  return;
}
int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();
}
