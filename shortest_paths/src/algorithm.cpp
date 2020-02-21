#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

struct vertex_info {
  int lon;
  int lat;
};

// BGL include
#include <boost/graph/adjacency_list.hpp>
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS,
    vertex_info,                                // no vertex property
    boost::property<boost::edge_weight_t, int>  // interior edge weight property
    >
    weighted_graph;

typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;

typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

#include <boost/graph/dijkstra_shortest_paths.hpp>
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

typedef std::vector<edge_desc> path_t;

int dijkstra_path(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);
  std::vector<vertex_desc> pred_map(n);

  boost::dijkstra_shortest_paths(
      G, s,
      boost::distance_map(
          boost::make_iterator_property_map(dist_map.begin(),
                                            boost::get(boost::vertex_index, G)))
          .predecessor_map(boost::make_iterator_property_map(
              pred_map.begin(), boost::get(boost::vertex_index, G))));

  vertex_desc v = 0;
  path_t path;

  if (dist_map[t] == std::numeric_limits<int>::max()) {
    std::cout << "unreachable\n";
    return -1;
  } else {
    for (vertex_desc u = pred_map[v]; u != v; v = u, u = pred_map[v]) {
      std::pair<edge_desc, bool> edge_pair = boost::edge(u, v, G);
      path.push_back(edge_pair.first);
    }
    std::cout << dist_map[t] << "\n" << path.size();
  }

  for (path_t::reverse_iterator riter = path.rbegin(); riter != path.rend();
       ++riter) {
    vertex_desc u_tmp = boost::source(*riter, G);
    vertex_desc v_tmp = boost::target(*riter, G);
    edge_desc e_tmp = boost::edge(u_tmp, v_tmp, G).first;

    std::cout << "  " << u_tmp << " " << v_tmp;
  }
  std::cout << "\n";
  return dist_map[t];
}

void testcase() {
  int n = 0, m = 0, q = 0;
  int lon = 0, lat = 0;
  int a = 0, b = 0, c = 0;
  int s = 0, t = 0;

  int distance = 0;

  std::cin >> n;
  std::cin >> m;
  std::cin >> q;

  std::vector<int> queryResults(q);
  weighted_graph G(n);
  std::vector<vertex_desc> shortestPath;

  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;

  // Read in vertex
  for (int i = 0; i < n; i++) {
    std::cin >> lon;
    std::cin >> lat;
    G[i] = {lat, lon};
    // add this info? why
  }

  // Read in edges
  for (int i = 0; i < m; i++) {
    std::cin >> a;
    std::cin >> b;
    std::cin >> c;
    e = boost::add_edge(a, b, G).first;
    weights[e] = c;
  }

  // Read in queries
  for (auto query : queryResults) {
    std::cin >> s;
    std::cin >> t;
    dijkstra_path(G, s, t);
  }

  std::cout << std::flush;
}

int main(int argc, char const *argv[]) {
  std::ios_base::sync_with_stdio(false);
  testcase();
}
