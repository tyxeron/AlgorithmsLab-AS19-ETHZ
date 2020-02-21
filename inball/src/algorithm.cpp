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
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);

  int n = 0, d = 0, val = 0;
  cin >> n;
  while (n != 0) {
    Program lp(CGAL::SMALLER, false, 0, false, 0);

    cin >> d;
    for (int i = 0; i < n; i++) {
      long ai = 0;
      for (int j = 0; j < d; j++) {
        cin >> val;
        ai += pow(val, 2);
        lp.set_a(j, i, val);
      }

      cin >> val;

      lp.set_a(d, i, sqrt(ai));
      lp.set_b(i, val);
    }

    lp.set_c(d, -1);  // Maximize objective R
    lp.set_l(d, true, 0);

    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.is_optimal()) {
      auto sol = CGAL::to_double(-s.objective_value().numerator() /
                                 s.objective_value().denominator());
      cout << sol << endl;

    } else if (s.is_unbounded()) {
      cout << "inf" << endl;
    } else {
      cout << "none" << endl;
    }

    cin >> n;
  }
  return EXIT_SUCCESS;
}
