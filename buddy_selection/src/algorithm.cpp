#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int> >
    Graph;

typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

typedef boost::graph_traits<Graph>::edge_iterator EdgeIter;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIter;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

void testcase() {
  int n = 0, c = 0, f = 0;
  std::cin >> n >> c >> f;

  Graph graph(n);
  std::vector<std::vector<std::string> > characteristics(n);

  std::string characteristic;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
      std::cin >> characteristic;
      characteristics[i].push_back(characteristic);
    }
    std::sort(characteristics[i].begin(), characteristics[i].end());
  }

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      std::vector<std::string> commonCharacteristics(c);
      auto it = std::set_intersection(
          characteristics[i].begin(), characteristics[i].end(),
          characteristics[j].begin(), characteristics[j].end(),
          commonCharacteristics.begin());
      int commonCCount = it - commonCharacteristics.begin();
      if (commonCCount > f) {
        boost::add_edge(i, j, graph);
      }
    }
  }
  std::vector<Vertex> people(n);
  boost::edmonds_maximum_cardinality_matching(graph, &people[0]);
  std::cout << (boost::matching_size(graph, &people[0]) * 2 == n ? "not optimal"
                                                               : "optimal")
            << std::endl;
}

int main(int argc, char const* argv[]) {
  std::ios_base::sync_with_stdio(false);
  int t = 0;
  std::cin >> t;
  while (t--) {
    testcase();
  }
}
