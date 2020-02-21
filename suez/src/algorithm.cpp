#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

using namespace std;
int w = 0;
int h = 0;

double ceil_to_double(const SolT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

void testcase() {
  int n = 0, m = 0;
  cin >> n >> m >> h >> w;

  Program lp(CGAL::SMALLER, true, 1, false, 0);

  // First points
  vector<pair<long, long>> freeNails;
  vector<pair<long, long>> occupiedNails;
  freeNails.reserve(n);
  occupiedNails.reserve(m);

  for (int i = 0; i < n; i++) {
    long x = 0, y = 0;
    cin >> x >> y;
    freeNails.emplace_back(x, y);
  }
  for (int i = 0; i < m; i++) {
    long x = 0, y = 0;
    cin >> x >> y;
    occupiedNails.emplace_back(x, y);
  }

  int constraintNum = 0;
  for (int i = 0; i < n; i++) {
    lp.set_c(i, -(2 * (h + w)));
    // Compare free with free posters
    for (int j = i + 1; j < n; j++) {
      long d_width = 2 * abs(freeNails[i].first - freeNails[j].first);
      long d_height = 2 * abs(freeNails[i].second - freeNails[j].second);
      // Only one condition must be fulfilled, take less strict.
      if (d_width * h > d_height * w) {
        lp.set_a(i, constraintNum, w);
        lp.set_a(j, constraintNum, w);
        lp.set_b(constraintNum, d_width);
      } else {
        lp.set_a(i, constraintNum, h);
        lp.set_a(j, constraintNum, h);
        lp.set_b(constraintNum, d_height);
      }
      constraintNum++;
    }
  }

  // Compare free with fixed posters
  for (int i = 0; i < n; i++) {
    long min_width = LONG_MAX;
    long min_height = LONG_MAX;

    for (int j = 0; j < m; j++) {
      long d_width = 2 * abs(freeNails[i].first - occupiedNails[j].first) - w;
      long d_height =
          2 * abs(freeNails[i].second - occupiedNails[j].second) - h;

      // Only one condition must be fulfilled, take less strict.
      if (d_width / w > d_height / h) {
        min_width = min(min_width, d_width);
      } else {
        min_height = min(min_height, d_height);
      }
    }

    if (min_width / w < min_height / h) {
      lp.set_a(i, constraintNum, w);
      lp.set_b(constraintNum, min_width);
    } else {
      lp.set_a(i, constraintNum, h);
      lp.set_b(constraintNum, min_height);
    }
    constraintNum++;
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));

  cout << fixed << setprecision(0) << ceil_to_double(-s.objective_value())
       << endl;
}
int main(int argc, char const* argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
