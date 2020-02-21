#include <assert.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

#include <boost/graph/graphviz.hpp>
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
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph_dk;
typedef boost::property_map<weighted_graph_dk, boost::edge_weight_t>::type
    weight_map_dk;
typedef boost::graph_traits<weighted_graph_dk>::edge_descriptor edge_desc_dk;
typedef boost::graph_traits<weighted_graph_dk>::vertex_descriptor
    vertex_desc_dk;

using namespace std;

void dijkstra_dist(const weighted_graph_dk &G, int s, vector<int> &dist_map) {
  int n = boost::num_vertices(G);

  boost::dijkstra_shortest_paths(
      G, s,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));
}

void testcase() {
  int n = 0, m = 0, f = 0, s = 0;
  cin >> n >> m >> s >> f;
  graph G(n);
  edge_adder adder(G);
  weighted_graph_dk G_dk(n);
  weight_map_dk weights = boost::get(boost::edge_weight, G_dk);
  edge_desc_dk edge_dk;
  vector<vector<int>> cap(n, vector<int>(n));
  for (int i = 0; i < m; i++) {
    int a = 0, b = 0, c = 0, d = 0;
    // edge_desc e;
    cin >> a >> b >> c >> d;
    if (a == b) continue;

    auto edge1 = boost::edge(a, b, G_dk);
    if (edge1.second) {
      auto edge2 = boost::edge(b, a, G_dk);
      if (weights[edge1.first] > d) {
        weights[edge1.first] = d;
        weights[edge2.first] = d;
        cap[a][b] = c;
        cap[b][a] = c;
      } else if (weights[edge1.first] == d) {
        cap[a][b] += c;
        cap[b][a] += c;
      }
    } else {
      cap[a][b] = c;
      cap[b][a] = c;
      weights[boost::add_edge(a, b, G_dk).first] = d;
      weights[boost::add_edge(b, a, G_dk).first] = d;
    }
  }
  vector<int> dist_map(n);
  dijkstra_dist(G_dk, s, dist_map);
  auto edge_iters = boost::edges(G_dk);
  for (auto edge_it = edge_iters.first; edge_it != edge_iters.second;
       edge_it++) {
    edge_desc e = *edge_it;
    vertex_desc u = e.m_source;
    vertex_desc v = e.m_target;
    int weight = weights[e];
    if (dist_map[u] + weight <= dist_map[v]) {
      // add edge to graph
      adder.add_edge(u, v, cap[u][v]);
    }
  }
  int flow = boost::push_relabel_max_flow(G, s, f);
  cout << flow << endl;
}

int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
