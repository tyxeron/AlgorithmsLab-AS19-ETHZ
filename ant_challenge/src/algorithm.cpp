#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS,
    boost::property<boost::vertex_distance_t, int>,  // no vertex property
    boost::property<boost::edge_weight_t, int>  // interior edge weight property
    >
    weighted_graph;

typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;

#include <boost/graph/dijkstra_shortest_paths.hpp>
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

#include <boost/graph/prim_minimum_spanning_tree.hpp>

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(
      G, s,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), get(boost::vertex_index, G))));

  return dist_map[t];
}

void dijkstra_spanning_tree(weighted_graph &G, int s, weighted_graph &ST) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> pred_map(n);

  weight_map weightMapST = boost::get(boost::edge_weight, ST);
  weight_map weightMapG = boost::get(boost::edge_weight, G);

  boost::property_map<weighted_graph, boost::vertex_distance_t>::type distance =
      boost::get(boost::vertex_distance, G);
  boost::property_map<weighted_graph, boost::vertex_index_t>::type indexmap =
      boost::get(boost::vertex_index, G);

  boost::prim_minimum_spanning_tree(G, s, &pred_map[0], distance, weightMapG,
                                    indexmap,
                                    boost::default_dijkstra_visitor());

  for (int i = 0; i < n; i++) {
    int edgeSource = pred_map[i];
    if (i == edgeSource) continue;
    auto edgeST = boost::edge(edgeSource, i, ST);
    auto edgeG = boost::edge(edgeSource, i, G);
    int weightG = weightMapG[edgeG.first];
    if (!edgeST.second) {
      edgeST = boost::add_edge(edgeSource, i, ST);
      weightMapST[edgeST.first] = weightG;
    } else {
      weightMapST[edgeST.first] = std::min(weightMapST[edgeST.first], weightG);
    }
  }
}

void testcase() {
  int n = 0, e = 0, s = 0, a = 0, b = 0;

  int t1 = 0, t2 = 0;
  std::cin >> n >> e >> s >> a >> b;
  std::vector<weighted_graph> G(s, weighted_graph(n));
  std::vector<weight_map> weights(s);
  weighted_graph ST(n);
  std::vector<int> hives(s);

  for (int i = 0; i < s; i++) {
    weights[i] = boost::get(boost::edge_weight, G[i]);
  }

  for (int i = 0; i < e; i++) {
    std::cin >> t1 >> t2;
    for (int j = 0; j < s; j++) {
      // add edge
      auto e = boost::add_edge(t1, t2, G[j]);
      std::cin >> weights[j][e.first];
    }
  }

  for (int i = 0; i < s; i++) {
    std::cin >> hives[i];
    dijkstra_spanning_tree(G[i], hives[i], ST);
  }

  std::cout << dijkstra_dist(ST, a, b) << std::endl;
}
int main(int argc, char const *argv[]) {
  std::ios_base::sync_with_stdio(false);
  int t = 0;
  std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }

  return EXIT_SUCCESS;
}
