#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
/*
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
*/
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
                            boost::property<boost::edge_weight_t, long>>>>>
    graph;  // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;  // Iterator
/*
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
};*/

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

int dijkstra_dist(const weighted_graph& G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(
      G, s,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}
using namespace std;
void testcase() {
  int numLayers;
  cin >> numLayers;

  int N = numLayers * (numLayers + 1) / 2;

  weighted_graph G(2 * N);  // In and Out nodes
  weight_map weights = boost::get(boost::edge_weight, G);
  vector<int> nodeWeights(N);

  int count = N - 1;

  edge_desc e;
  // In and Out nodes
  for (int i = 1; i <= numLayers; i++) {
    for (int j = 0; j < i; j++) {
      int w;
      cin >> w;
      e = boost::add_edge(count, count + N, G).first;
      weights[e] = w;
      nodeWeights[count--] = w;
    }
  }

  int nodeNum = 0;
  for (int i = numLayers, layer = 0; i > 0; i--, layer++) {
    for (int j = 0; j < i; j++) {
      // left and topleft
      if (j != 0) {
        e = boost::add_edge(nodeNum + N, nodeNum - 1, G).first;
        weights[e] = 0;

        e = boost::add_edge(nodeNum + N, nodeNum + numLayers - layer - 1, G)
                .first;
        weights[e] = 0;
      }
      // right and topright
      if (j != i - 1) {
        e = boost::add_edge(nodeNum + N, nodeNum + 1, G).first;
        weights[e] = 0;

        e = boost::add_edge(nodeNum + N, nodeNum + numLayers - layer, G).first;
        weights[e] = 0;
      }

      // below
      if (i != numLayers) {
        // bottomleft
        e = boost::add_edge(nodeNum + N, nodeNum - numLayers + layer - 1, G)
                .first;
        weights[e] = 0;
        // bottomright
        e = boost::add_edge(nodeNum + N, nodeNum - numLayers + layer, G).first;
        weights[e] = 0;
      }

      nodeNum++;
    }
  }

  // Calculate the three paths
  std::vector<vertex_desc> predmap1(2 * N);
  std::vector<int> distmap1(2 * N);
  vertex_desc start = 0;
  boost::dijkstra_shortest_paths(
      G, start,
      boost::predecessor_map(
          boost::make_iterator_property_map(predmap1.begin(),
                                            boost::get(boost::vertex_index, G)))
          .distance_map(boost::make_iterator_property_map(
              distmap1.begin(), boost::get(boost::vertex_index, G))));

  std::vector<vertex_desc> predmap2(2 * N);
  std::vector<int> distmap2(2 * N);
  start = numLayers - 1;
  boost::dijkstra_shortest_paths(
      G, start,
      boost::predecessor_map(
          boost::make_iterator_property_map(predmap2.begin(),
                                            boost::get(boost::vertex_index, G)))
          .distance_map(boost::make_iterator_property_map(
              distmap2.begin(), boost::get(boost::vertex_index, G))));

  std::vector<vertex_desc> predmap3(2 * N);
  std::vector<int> distmap3(2 * N);
  start = N - 1;
  boost::dijkstra_shortest_paths(
      G, start,
      boost::predecessor_map(
          boost::make_iterator_property_map(predmap3.begin(),
                                            boost::get(boost::vertex_index, G)))
          .distance_map(boost::make_iterator_property_map(
              distmap3.begin(), boost::get(boost::vertex_index, G))));
  int min_dist = numeric_limits<int>::max();
  for (int i = N; i < 2 * N; i++) {
    min_dist = min(min_dist, distmap1[i] + distmap2[i] + distmap3[i] -
                                 2 * nodeWeights[i - N]);
  }

  cout << min_dist << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();
}
