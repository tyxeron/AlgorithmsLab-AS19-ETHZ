#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>

#include <boost/graph/strong_components.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list<
    vecS, vecS, directedS,
    property<vertex_color_t, default_color_type>,  // no vertex property
    property<edge_weight_t, int>                   // interior edge weight
                                                   // property
    >
    weighted_graph;

typedef property_map<weighted_graph, edge_weight_t>::type weight_map_;

typedef graph_traits<weighted_graph>::edge_descriptor edge_desc;

typedef graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

typedef graph_traits<weighted_graph>::edge_iterator edge_it;

int dijkstra(const weighted_graph& G, int s, int maxT) {
  int n = num_vertices(G);
  vector<int> dist_map(n);
  vector<vertex_desc> pred_map(n);

  dijkstra_shortest_paths(
      G, s,
      predecessor_map(boost::make_iterator_property_map(
                          pred_map.begin(), get(boost::vertex_index, G)))
          .distance_map(boost::make_iterator_property_map(
              dist_map.begin(), get(boost::vertex_index, G))));

  int minDist = numeric_limits<int>::max();

  for (int i = 0; i < maxT; i++) {
    minDist = min(minDist, dist_map[i]);
  }
  return minDist;
}

void getConnectedComponents(weighted_graph& G, vector<int>& teleportStations) {
  weight_map_ weights = get(edge_weight, G);
  vector<int> component(num_vertices(G)), discover_time(num_vertices(G));
  vector<vertex_desc> root(num_vertices(G));
  int num = strong_components(
      G, make_iterator_property_map(component.begin(), get(vertex_index, G)),
      root_map(make_iterator_property_map(root.begin(), get(vertex_index, G))));

  vector<vector<int>> extractedComponents(num_vertices(G));
  // stupid solution but should get me 20 points more
  for (auto stationId : teleportStations) {
    extractedComponents[component[stationId]].push_back(stationId);
  }

  int vertCount = boost::num_vertices(G);

  for (auto extractedComponent : extractedComponents) {
    int size = (int)extractedComponent.size() - 1;
    if (size % 2 == 0) {
      for (auto iter1 = extractedComponent.begin();
           iter1 != extractedComponent.end(); iter1++) {
        auto e = add_edge(*iter1, vertCount, G).first;
        weights[e] = (size) / 2;

        e = add_edge(vertCount, *iter1, G).first;
        weights[e] = (size) / 2;
      }

      vertCount++;
    } else {
      int invert = vertCount++;
      int target = vertCount++;
      int outvert = vertCount++;

      auto e = add_edge(invert, target, G).first;
      weights[e] = 1;
      e = add_edge(target, outvert, G).first;
      weights[e] = 0;

      for (auto iter1 = extractedComponent.begin();
           iter1 != extractedComponent.end(); iter1++) {
        e = add_edge(*iter1, invert, G).first;
        weights[e] = (size) / 2;

        e = add_edge(outvert, *iter1, G).first;
        weights[e] = (size) / 2;
      }
    }
  }
}

void testcase() {
  int n = 0, m = 0, k = 0, T = 0;
  int u = 0, v = 0, c = 0;
  cin >> n >> m >> k >> T;

  vector<int> teleportStations(T);

  weighted_graph G(n);
  weight_map_ weights = get(edge_weight, G);

  for (auto& station : teleportStations) {
    cin >> station;
  }
  for (int i = 0; i < m; i++) {
    cin >> u >> v >> c;
    auto e = add_edge(v, u, G).first;
    weights[e] = c;
  }

  getConnectedComponents(G, teleportStations);

  // print_graph(G);
  int minDistance = dijkstra(G, n - 1, k);

  if (minDistance > pow(10, 6)) {
    cout << "no" << endl;
  } else {
    cout << minDistance << endl;
  }
}
int main(int argc, char const* argv[]) {
  ios_base::sync_with_stdio(false);
  // TODO
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
