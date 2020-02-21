#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <cassert>
#include <iostream>
#include <vector>

// choose exact integral type
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;
struct Cell {
  int x, y, z, i;
};

bool calculateFeasability(int maxDeg, vector<Cell> &goodCells,
                          vector<Cell> &badCells) {
  Program lp(CGAL::SMALLER, false, 0, false, 0);

  int epsilon = 0;
  int coeffIndex = 1;
  for (int xDeg = 0; xDeg <= maxDeg; ++xDeg) {
    for (int yDeg = 0; yDeg <= maxDeg - xDeg; ++yDeg) {
      for (int zDeg = 0; zDeg <= maxDeg - xDeg - yDeg; ++zDeg) {
        for (Cell good : goodCells) {
          double value =
              pow(good.x, xDeg) * pow(good.y, yDeg) * pow(good.z, zDeg);
          lp.set_a(coeffIndex, good.i, value);
        }

        for (Cell bad : badCells) {
          double value = pow(bad.x, xDeg) * pow(bad.y, yDeg) * pow(bad.z, zDeg);
          lp.set_a(coeffIndex, bad.i, value);
        }
        coeffIndex++;
      }
    }
  }

  // Add marin term epsilon
  for (Cell good : goodCells) {
    lp.set_a(epsilon, good.i, 1.0);
    lp.set_b(good.i, 0.0);
  }

  for (Cell bad : badCells) {
    lp.set_a(epsilon, bad.i, -1.0);
    lp.set_b(bad.i, 0.0);
    lp.set_r(bad.i, CGAL::LARGER);
  }

  // maximize margin parameter epsilon
  lp.set_c(epsilon, -1.0);
  lp.set_l(epsilon, true, 0.0);
  lp.set_u(epsilon, true, 1.0);

  CGAL::Quadratic_program_options options;
  options.set_pricing_strategy(CGAL::QP_BLAND);
  Solution s = CGAL::solve_linear_program(lp, ET(), options);
  assert(s.solves_linear_program(lp));

  return !s.is_infeasible() && (s.objective_value() != 0.0);
}
void testcase() {
  int t = 0, h = 0;
  cin >> h;
  cin >> t;
  int x = 0, y = 0, z = 0;

  vector<Cell> goodCells;
  goodCells.reserve(h);
  vector<Cell> badCells;
  badCells.reserve(h);

  for (int i = 0; i < h; i++) {
    cin >> x >> y >> z;
    goodCells.push_back({x, y, z, i});
  }

  for (int i = 0; i < t; i++) {
    cin >> x >> y >> z;
    badCells.push_back({x, y, z, i + h});
  }

  int l = 0;
  int r = 30;
  while (l < r) {
    int m = (l + r) / 2;
    if (calculateFeasability(m, goodCells, badCells)) {
      r = m;
    } else {
      l = m + 1;
    }
  }
  assert(l == r);

  if (l != 30) {
    cout << l << endl;
  } else {
    cout << "Impossible!" << endl;
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
