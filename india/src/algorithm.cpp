#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
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
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
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
  edge_desc add_edge_edge_desc(int from, int to, long capacity, long cost) {
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
    return e;
  }
};

using namespace std;
void testcase() {
  int c, g, b, k, a;
  cin >> c >> g >> b >> k >> a;

  graph G(c);
  edge_adder adder(G);

  int source = boost::add_vertex(G);
  auto c_map = boost::get(boost::edge_capacity, G);

  int s_flow = 0;
  out_edge_it e, eend;
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  auto initEdge = adder.add_edge_edge_desc(source, k, 0, 0);

  long maxCap = 0;
  for (int i = 0; i < g; i++) {
    int x, y, d, e;
    cin >> x >> y >> d >> e;
    adder.add_edge(x, y, e, d);
    maxCap += e;
  }

  int maxSuitcase = 0;

  boost::successive_shortest_path_nonnegative_weights(G, k, a);
  int cost = boost::find_flow_cost(G);

  int l = 0;
  int r = maxCap;
  while (l < r) {
    int m = l + (r - l) / 2;
    c_map[initEdge] = m;
    boost::successive_shortest_path_nonnegative_weights(G, source, a);
    cost = boost::find_flow_cost(G);

    s_flow = 0;
    for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G);
         e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];
    }

    if (cost <= b) {
      maxSuitcase = max(maxSuitcase, s_flow);
      l = m + 1;
    } else if (cost > b) {
      r = m;
    }
  }

  cout << maxSuitcase << endl;
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();
}
