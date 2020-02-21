#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <boost/graph/kruskal_min_spanning_tree.hpp>

#include <boost/graph/connected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef weighted_graph::edge_iterator edge_iter;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void kruskal(const weighted_graph &G, std::vector<edge_desc> &mst) {
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
}

using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS,
                       property<vertex_distance_t, int>,
                       property<edge_weight_t, int>>
    Graph;
typedef std::pair<int, int> E;

using namespace std;
void testcase() {
  int n, i_;
  cin >> n >> i_;
  i_ -= 1;  // 0 based indices

  // graph related
  weighted_graph G(n);
  auto weightmap = get(edge_weight, G);

  // build graph
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < n - (i + 1) + 1; j++) {
      cin >> weightmap[add_edge(i, i + j, G).first];
    }
  }
  std::pair<edge_iter, edge_iter> es = boost::edges(G);

  vector<pair<int, edge_desc>> edgesSorted(boost::num_edges(G));
  int i = 0;
  for (auto iter = es.first; iter != es.second; iter++, i++) {
    edgesSorted[i] = make_pair(weightmap[*iter], *iter);
  }

  // Sort edges O(nlog(n))
  sort(edgesSorted.begin(), edgesSorted.end());

  // Create MST with kruskal O(n^2)
  vector<edge_desc> mstEdgesExt;
  kruskal(G, mstEdgesExt);

  weighted_graph MST(n);
  auto weightmapMST = get(edge_weight, MST);

  // build graph
  int MSTCost = 0;
  vector<edge_desc> mstEdges;
  for (const auto &edge : mstEdgesExt) {
    auto e = boost::add_edge(edge.m_source, edge.m_target, MST).first;
    mstEdges.push_back(e);
    weightmapMST[e] = weightmap[edge];
    MSTCost += weightmap[edge];
  }

  int minCost = numeric_limits<int>::max();

  for (const auto &edge : mstEdges) {
    int tmpCost = MSTCost;
    // remove weight from total cost
    auto weight = weightmapMST[edge];
    tmpCost -= weight;
    // Temporarely remove edge from MST
    boost::remove_edge(edge.m_source, edge.m_target, MST);

    std::vector<int> component(num_vertices(MST));

    int num = connected_components(MST, &component[0]);

    for (auto iter = edgesSorted.begin(); iter != edgesSorted.end(); iter++) {
      if (component[iter->second.m_source] !=
              component[iter->second.m_target] &&
          (edge.m_source != iter->second.m_source ||
           edge.m_target != iter->second.m_target)) {
        tmpCost += iter->first;
        break;
      }
    }
    minCost = min(minCost, tmpCost);

    // add edge back in
    weightmapMST[boost::add_edge(edge.m_source, edge.m_target, MST).first] =
        weight;
  }
  cout << minCost << endl;
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();
}
