#include <CGAL/Direction_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <quadmath.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
#include <set>
#include <stack>
#include <stdexcept>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Direction_2 D;

//#define
using namespace std;
using namespace CGAL;

vector<int> indices;
vector<R> trajectories;
vector<K::FT> slopes;
vector<double> startY;

bool sortAsc(int i, int j) { return startY[i] < startY[j]; }

void testcase() {
  // Read input and create set of motos
  int n;
  cin >> n;
  indices = vector<int>(n);
  trajectories = vector<R>(n);
  slopes = vector<K::FT>(n);
  startY = vector<double>(n);
  double y0 = 0, x1 = 0, y1 = 0;
  for (int i = 0; i < n; i++) {
    std::cin >> y0 >> x1 >> y1;
    indices[i] = i;
    startY[i] = y0;
    R ray = R(P(0.0, y0), P(x1, y1));
    trajectories[i] = ray;
    slopes[i] = ray.direction().dy() / ray.direction().dx();
  }

  sort(indices.begin(), indices.end(), sortAsc);
  vector<int> stillRiding;
  stillRiding.push_back(indices[0]);

  for (int i = 1; i < n; i++) {
    int bi = indices[i];

    bool stillR = true;
    while (!stillRiding.empty() && stillR) {
      int bj = stillRiding.back();
      if (do_intersect(trajectories[bi], trajectories[bj])) {
        if (slopes[bi] + slopes[bj] > 0) {
          // smaller slope of bi means rider bi came before: bj stops
          stillRiding.pop_back();
        } else {
          // Same slope or bi slope is bigger than bj: bi stops
          stillR = false;
        }
      } else {
        break;
      }
    }

    if (stillR) {
      stillRiding.push_back(bi);
    }
  }
  sort(stillRiding.begin(), stillRiding.end());
  for (auto bike : stillRiding) {
    cout << bike << " ";
  }
  cout << endl;
}

int main(int argc, char const* argv[]) {
  std::ios_base::sync_with_stdio(false);
  int t = 0;
  std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
  // TODO
  return 0;
}
