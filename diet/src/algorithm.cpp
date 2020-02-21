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
  int n = 0, m = 0;  // m : #products, n: #nutritients
  cin >> n >> m;
  while (n != 0 && m != 0) {
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    int price = 0;

    vector<pair<int, int>> limits(n);
    vector<vector<int>> amounts(n, vector<int>(m));

    for (auto &limit : limits) {
      cin >> limit.first >> limit.second;  // <min,max>
    }

    for (int i = 0; i < m; i++) {
      cin >> price;
      for (int j = 0; j < n; j++) {
        cin >> amounts[j][i];
      }
      lp.set_c(i, price);
    }

    int counter = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        lp.set_a(j, 2 * i, amounts[i][j]);
        lp.set_a(j, 2 * i + 1, -amounts[i][j]);
      }

      lp.set_b(2 * i, limits[i].second);
      lp.set_b(2 * i + 1, -limits[i].first);
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    // cout << s << endl;
    if (s.is_optimal()) {
      auto sol = std::floor(CGAL::to_double(s.objective_value().numerator() /
                                            s.objective_value().denominator()));
      cout << sol << endl;

    } else {
      cout << "No such diet." << endl;
    }

    cin >> n >> m;
  }
}
