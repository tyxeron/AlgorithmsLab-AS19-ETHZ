#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <cassert>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor
    Edge;  // Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor
    Vertex;  // Vertex Descriptor: with vecS vertex list, this is really just an
             // int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator
    EdgeIt;  // to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator
    OutEdgeIt;  // to iterate over all outgoing edges of a vertex

using namespace std;
using namespace boost;

typedef vector<vector<pair<int, int> > > Queries;

int binary(const int maxAge, const vector<int> &path, const vector<int> &age) {
  int l = 0;
  int r = path.size() - 1;
  while (l != r) {
    int p = (l + r) / 2;
    if (age[path[p]] > maxAge) {
      l = p + 1;
    } else {
      r = p;
    }
  }
  return path[l];
}

void dfs(const int u, const Graph &G, vector<int> &path, const Queries &queries,
         vector<int> &result, const vector<int> &ageMap) {
  // handle queries
  for (auto query : queries[u]) {
    result[query.second] = binary(query.first, path, ageMap);
  }

  OutEdgeIt ebeg, eend;
  for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
    int v = boost::target(*ebeg, G);
    path.push_back(v);
    dfs(v, G, path, queries, result, ageMap);
  }

  path.pop_back();
}

void testcase() {
  int n, q;
  cin >> n >> q;
  Graph G(n);
  std::unordered_map<string, int> nameMap;
  vector<string> indexMap(n);
  vector<int> ageMap(n);

  for (int i = 0; i < n; ++i) {
    string name;
    int age;
    cin >> name >> age;
    nameMap[name] = i;
    indexMap[i] = name;
    ageMap[i] = age;
  }

  // Read graph
  for (int i = 0; i + 1 < n; ++i) {
    Edge e;
    bool success;
    string s, p;
    cin >> s >> p;
    tie(e, success) = boost::add_edge(nameMap[p], nameMap[s], G);
  }

  // Read queries
  vector<vector<pair<int, int> > > queries(n);
  for (int i = 0; i < q; ++i) {
    string name;
    int maxAge;
    cin >> name >> maxAge;
    queries[nameMap[name]].push_back(make_pair(maxAge, i));
  }
  vector<int> result(q);

  int rootIndex = max_element(ageMap.begin(), ageMap.end()) - ageMap.begin();

  // DFS
  vector<int> path;
  path.push_back(rootIndex);
  dfs(rootIndex, G, path, queries, result, ageMap);
  // Print result in correct order
  for (int i = 0; i < q; ++i) {
    cout << indexMap[result[i]];
    if (i != q - 1) cout << " ";
  }
  cout << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) testcase();
  return 0;
}