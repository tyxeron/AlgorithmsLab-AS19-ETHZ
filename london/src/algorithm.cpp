#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <boost/graph/graphviz.hpp>

#define NUM_CHARS 26

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Print Graph
#include <boost/graph/graph_utility.hpp>

#include <ctime>

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
  int h = 0, w = 0;

  cin >> h >> w;
  string crimeText;
  cin >> crimeText;

  int n = crimeText.length();

  // letter in crimeText and source plus sink
  Vertex source = 0;
  Vertex sink = 1;
  int offset = 2;

  Graph G;  // Graph, node for each word pair (back/front),
  edge_adder adder(G);

  vector<int> target(NUM_CHARS);

  vector<vector<char>> front(h, vector<char>(w));
  vector<vector<char>> back(h, vector<char>(w));

  string tmp;
  for (int i = 0; i < h; i++) {
    cin >> tmp;
    for (int j = 0; j < w; j++) {
      front[i][j] = tmp[j];
    }
  }
  for (int i = 0; i < h; i++) {
    cin >> tmp;
    for (int j = 0; j < w; j++) {
      back[i][j] = tmp[j];
    }
  }

  vector<vector<int>> pairs(NUM_CHARS, vector<int>(NUM_CHARS, 0));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      pairs[(int)front[i][j] - (int)'A'][(int)back[i][w - j - 1] - (int)'A']++;
    }
  }

  for (int i = 0; i < n; i++) {
    target[(int)crimeText[i] - (int)'A']++;
  }

  for (int i = offset; i < NUM_CHARS + offset; i++) {  // n+offset offset
    adder.add_edge(i, sink,
                   target[i - offset]);  // add capacity #bagofwords at pos i
                                         // fromtarget to sink
  }

  int nodeId = NUM_CHARS + offset;
  for (int i = 0; i < NUM_CHARS; ++i) {
    for (int j = 0; j < NUM_CHARS; ++j) {
      adder.add_edge(source, nodeId, pairs[i][j]);
      adder.add_edge(nodeId, i + offset, pairs[i][j]);
      adder.add_edge(nodeId, j + offset, pairs[i][j]);
      nodeId++;
    }
  }

  // write_graphviz(std::cout, G);
  long flow = boost::push_relabel_max_flow(G, source, sink);

  if (flow == n) {
    cout << "Yes" << endl;
  } else {
    cout << "No" << endl;
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
