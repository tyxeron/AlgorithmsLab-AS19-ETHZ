// ALGOLAB BGL Tutorial 3
// Code demonstrating
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using
// successive_shortest_path_nonnegative_weights

// Includes
// ========
#include <iostream>
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
};

using namespace std;

void testcase() {
  int n = 0;
  cin >> n;

  const int N = n + 2;
  const int source = 0;
  const int sink = N - 1;

  // Create graph, edge adder class and propery maps
  graph G(N);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  int totalStudentAmount = 0;

  for (int day = 0; day < n; day++) {
    int amount, cost;
    cin >> amount >> cost;
    adder.add_edge(source, day + 1, amount, cost);
  }

  for (int day = 0; day < n; day++) {
    int amount, price;
    cin >> amount >> price;
    totalStudentAmount += amount;
    adder.add_edge(day + 1, sink, amount, 20 - price);
  }

  for (int day = 0; day < n - 1; day++) {
    int amount, cost;
    cin >> amount >> cost;
    adder.add_edge(day + 1, day + 2, amount, cost);
  }

  // Run the algorithm

  // Option 1: Min Cost Max Flow with cycle_canceling O(n^3)
  /*
  int flow = boost::push_relabel_max_flow(G, source, sink);
  boost::cycle_canceling(G);
  int cost = boost::find_flow_cost(G);*/

  // Option 2: Min Cost Max Flow with
  // successive_shortest_path_nonnegative_weights
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = boost::find_flow_cost(G);
  // Iterate over all edges leaving the source to sum up the flow values.
  int flow = 0;
  out_edge_it e, eend;
  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G);
       e != eend; ++e) {
    flow += c_map[*e] - rc_map[*e];
  }

  if (flow == totalStudentAmount) {
    cout << "possible ";
  } else {
    cout << "impossible ";
  }

  cout << flow << " " << 20 * flow - cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}