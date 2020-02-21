#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>
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
// using namespace boost;

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

using namespace std;
void testcase() {
  int n = 0, m = 0, s = 0, i1 = 0, i2 = 0, tmp = 0;
  cin >> n >> m >> s;
  unordered_map<int, int> storeLocation(s);
  Graph G(n + 1);
  edge_adder adder(G);
  int source = 0;
  int sink = n;

  for (int i = 0; i < s; i++) {
    cin >> tmp;
    auto it = storeLocation.find(tmp);
    if (it != storeLocation.end()) {
      it->second++;
    } else {
      storeLocation.emplace(tmp, 1);
    }
  }

  for (auto it = storeLocation.begin(); it != storeLocation.end(); it++) {
    adder.add_edge(it->first, sink, it->second);
  }

  for (int i = 0; i < m; i++) {
    cin >> i1 >> i2;
    adder.add_edge(i1, i2, 1);
    if (i1 != source) {
      adder.add_edge(i2, i1, 1);  // only backedgesfor normal intersections
    }
  }
  long flow = boost::push_relabel_max_flow(G, source, sink);
  if (flow == s) {
    cout << "yes" << endl;
  } else {
    cout << "no" << endl;
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
