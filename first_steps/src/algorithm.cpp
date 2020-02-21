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
    boost::no_property,                         // no vertex property
    boost::property<boost::edge_weight_t, int>  // interior edge weight property
    >
    weighted_graph;

typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;

#include <boost/graph/kruskal_min_spanning_tree.hpp>
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

#include <boost/graph/dijkstra_shortest_paths.hpp>
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

int kruskal(weighted_graph &G) {
  long totalWeight = 0;
  weight_map weightMap = boost::get(boost::edge_weight, G);

  // Create Minimum Spanning Tree with kruskal algorithm
  std::vector<edge_desc> mst;

  // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, back_inserter(mst));

  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end();
       ++it) {
    totalWeight += weightMap[*it];
  }

  return totalWeight;
}

int dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<long> dist_map(n);

  boost::dijkstra_shortest_paths(
      G, s,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), get(boost::vertex_index, G))));

  long maxDist = 0;
  for (auto dist : dist_map) {
    if (dist < INT_MAX) {  // if i is reachable
      maxDist = std::max(dist, maxDist);
    }
  }
  return maxDist;
}

void testcase() {
  int n = 0, m = 0;
  int u = 0, v = 0, w = 0;

  long totalWeight = 0;
  long maxDistance = 0;
  std::cin >> n;
  std::cin >> m;

  weighted_graph G(n);

  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;

  // Read in edges
  for (int i = 0; i < m; i++) {
    std::cin >> u;
    std::cin >> v;
    std::cin >> w;
    e = boost::add_edge(u, v, G).first;
    weights[e] = w;
  }

  totalWeight = kruskal(G);

  maxDistance = dijkstra_dist(G, 0);

  std::cout << totalWeight << " " << maxDistance << std::endl;
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
