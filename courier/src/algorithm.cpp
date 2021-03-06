#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

#include <boost/graph/graphviz.hpp>
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

#define MAXCAP numeric_limits<long>::max()
using namespace std;

void testcase() {
  int Z = 0, J = 0;
  cin >> Z >> J;
  graph G(Z + J + 2);
  int source = boost::num_vertices(G) - 1;
  int sink = source - 1;
  edge_adder adder(G);

  for (int i = 0; i < Z; i++) {
    int cost = 0;
    cin >> cost;
    adder.add_edge(i, sink, cost);
  }

  int totalReward = 0;
  for (int i = 0; i < J; i++) {
    int reward = 0;
    cin >> reward;
    totalReward += reward;
    adder.add_edge(source, i + Z, reward);
  }

  for (int i = 0; i < J; i++) {
    // Job i
    int nI = 0;
    int c = 0;
    cin >> nI;
    for (int j = 0; j < nI; j++) {
      cin >> c;
      adder.add_edge(i + Z, c, MAXCAP);
    }
  }

  int flow = boost::push_relabel_max_flow(G, source, sink);
  cout << totalReward - flow << endl;
}
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
