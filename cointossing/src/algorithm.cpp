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

typedef std::unordered_multimap<char, int>::iterator MMAPIterator;

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

void testcase() {
  int n = 0, m = 0, countC = 0;
  int a = 0, b = 0, c = 0;

  cin >> n >> m;

  vector<pair<int, int>> gameUndecided;
  vector<int> earningsDecided(n, 0);
  vector<int> earningsDiff(n, 0);

  for (int i = 0; i < m; i++) {
    cin >> a >> b >> c;

    switch (c) {
      case 0:
        gameUndecided.emplace_back(a, b);

        break;
      case 1:
        earningsDecided[a] += 1;

        break;
      case 2:
        earningsDecided[b] += 1;

        break;
      default:
        exit(-1);
    }
  }
  int cCount = gameUndecided.size();

  Graph G(n + cCount + 2);
  edge_adder adder(G);
  int source = 0;
  int sink = 1;
  int targetEarning = 0;
  int nodeOffset = 2 + cCount;
  int expectedFlow = 0;
  bool earlyTermination = false;
  for (int i = 0; i < n; i++) {
    cin >> targetEarning;
    int diff = targetEarning - earningsDecided[i];
    if (diff < 0) {
      earlyTermination = true;
    }

    expectedFlow += diff;
    adder.add_edge(nodeOffset + i, sink, diff);
  }
  if (earlyTermination) {
    cout << "no" << endl;
    return;
  }

  for (int i = 0; i < cCount; i++) {
    adder.add_edge(source, i + 2, 1);
    adder.add_edge(i + 2, nodeOffset + gameUndecided[i].first, 1);
    adder.add_edge(i + 2, nodeOffset + gameUndecided[i].second, 1);
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);
  const auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const auto c_map = boost::get(boost::edge_capacity, G);
  // boost::print_graph(G, boost::get(boost::vertex_index, G));
  // cout << expectedFlow << endl;
  if (flow == expectedFlow) {
    // check back edges of source or games?
    // for now return yes
    // works NOW check back edges

    for (int i = 0; i < cCount; i++) {
      if (c_map[boost::edge(source, i + 2, G).first] -
              rc_map[boost::edge(source, i + 2, G).first] ==
          0) {
        cout << "no" << endl;
        return;
      }
    }

    cout << "yes" << endl;
  } else {
    cout << "no" << endl;
  }
}
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  // TODO
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
