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

void solveMax(int a, int b) {
  Program lp(CGAL::SMALLER, true, 0, false, 0);
  const int X = 0;
  const int Y = 1;

  lp.set_a(X, 0, 1);
  lp.set_a(Y, 0, 1);
  lp.set_b(0, 4);  //  x + y  <= 4

  lp.set_a(X, 1, 4);
  lp.set_a(Y, 1, 2);
  lp.set_b(1, a * b);  // 4x + 2y <= ab

  lp.set_a(X, 2, -1);
  lp.set_a(Y, 2, 1);
  lp.set_b(2, 1);  // -x + y <= 1

  // objective function
  lp.set_c(X, a);  // -ax
  lp.set_c(Y, -b);   // by

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_optimal()) {
    auto sol = std::floo  r(CGAL::to_double(-1 * s.objective_value().numerator() /
                          s.objective_value().denominator()));
    cout << sol << endl;

  } else if (s.is_unbounded()) {
    cout << "unbounded" << endl;
  } else {
    cout << "no" << endl;
  }
}
void solveMin(int a, int b) {
  Program lp(CGAL::LARGER, false, 0, true, 0);
  // Ax <= b, lower bound 0 and no upper bounds
  // set the coefficients of A and b
  const int X = 0;
  const int Y = 1;
  const int Z = 2;
  lp.set_a(X, 0, 1);
  lp.set_a(Y, 0, 1);
  lp.set_b(0, -4);  //  x + y  >= -4

  lp.set_a(X, 1, 4);
  lp.set_a(Y, 1, 2);
  lp.set_a(Z, 1, 1);
  lp.set_b(1, -a * b);  // 4x + 2y + z >= -ab

  lp.set_a(X, 2, -1);
  lp.set_a(Y, 2, 1);
  lp.set_b(2, -1);  // -x + y >= -1

  // objective function
  lp.set_c(X, a);  // ax
  lp.set_c(Y, b);  // by
  lp.set_c(Z, 1);  // z
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal()) {
    auto sol = std::ceil(CGAL::to_double(s.objective_value().numerator() /
                         s.objective_value().denominator()));
    cout << sol << endl;

  } else if (s.is_unbounded()) {
    cout << "unbounded" << endl;
  } else {
    cout << "no" << endl;
  }
}

int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);

  while (true) {
    int a = 0, b = 0;
    int p = 0;

    cin >> p;
    switch (p) {
      case 0: {
        return EXIT_SUCCESS;
      }
      case 1: {
        cin >> a >> b;
        solveMax(a, b);
        break;
      }
      case 2: {
        cin >> a >> b;
        solveMin(a, b);
        break;
      }
      default:
        cout << "Wrong parameters" << endl;
        return EXIT_SUCCESS;
    }
  }
  return EXIT_SUCCESS;
}
