#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Circle_2 C;

using namespace std;

struct Warehouse {
  int s, a;
  P p;
  Warehouse(int x_, int y_, int s_, int a_) {
    s = s_;
    a = a_;
    p = P(x_, y_);
  }
};

struct Stadium {
  int d, u;
  P p;
  Stadium(int x_, int y_, int d_, int u_) {
    d = d_;
    u = u_;
    p = P(x_, y_);
  }
};

struct Contour {
  C circle;
  Contour(int x_, int y_, int r_) { circle = C(P(x_, y_), pow(r_, 2)); }
};

double floor_to_double(const Solution& s) {
  double a = std::floor(CGAL::to_double(-s.objective_value_numerator() /
                                        s.objective_value_denominator()));
  while (a > -s.objective_value_numerator() / s.objective_value_denominator())
    a -= 1;
  while (a + 1 <=
         -s.objective_value_numerator() / s.objective_value_denominator())
    a += 1;
  return a;
}

void testcase() {
  int n, m, c;
  cin >> n >> m >> c;

  vector<Stadium> stadiums;
  vector<Warehouse> warehouses;
  vector<vector<float>> revenues(n, vector<float>(m));
  vector<Contour> contoursNonEmpty;
  vector<Contour> contours;

  for (int i = 0; i < n; i++) {
    int x, y, s, a;
    cin >> x >> y >> s >> a;
    warehouses.emplace_back(x, y, s, a);
  }

  for (int i = 0; i < m; i++) {
    int x, y, d, u;
    cin >> x >> y >> d >> u;
    stadiums.emplace_back(x, y, d, u);
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> revenues[i][j];
    }
  }

  for (int i = 0; i < c; i++) {
    int x, y, r;
    cin >> x >> y >> r;
    contours.emplace_back(x, y, r);
  }

  for (int k = 0; k < c && contoursNonEmpty.size() < 100; k++) {
    const K::Point_2& center = contours[k].circle.center();
    K::FT sq_rad = contours[k].circle.squared_radius();
    for (int i = 0; i < n + m; i++) {
      P p;
      if (i >= n) {
        p = stadiums[i - n].p;
      } else {
        p = warehouses[i].p;
      }
      if (CGAL::compare(CGAL::square(p.x() - center.x()) - sq_rad,
                        -CGAL::square(p.y() - center.y())) == CGAL::SMALLER) {
        contoursNonEmpty.push_back(contours[k]);
        break;
      }
    }
  }

  // Calculate contour line deficits and deduce from revenue
  if (contoursNonEmpty.size() > 0) {
    for (int i = 0; i < n; i++) {
      P q = warehouses[i].p;
      for (int j = 0; j < m; j++) {
        int count = 0;
        for (int k = 0; k < contoursNonEmpty.size(); k++) {
          const K::Point_2& center = contoursNonEmpty[k].circle.center();
          K::FT sq_rad = contoursNonEmpty[k].circle.squared_radius();
          P p = stadiums[j].p;
          if (CGAL::compare(CGAL::square(p.x() - center.x()) - sq_rad,
                            -CGAL::square(p.y() - center.y())) !=
              CGAL::compare(CGAL::square(q.x() - center.x()) - sq_rad,
                            -CGAL::square(q.y() - center.y()))) {
            count++;
          }
        }

        revenues[i][j] -= count / 100.f;
      }
    }
  }

  // Without contour lines:
  Program lp(CGAL::SMALLER, true, 0, false, 0);

  // warehouse constraints
  int varNum = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      lp.set_a(varNum++, i, 1);
    }
    lp.set_b(i, warehouses[i].s);
  }
  varNum = 0;

  // stadiums constraints
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      lp.set_a(j * m + i, (3 * i) + n, -1);
      lp.set_a(j * m + i, (3 * i + 1) + n, 1);
      lp.set_a(j * m + i, (3 * i + 2) + n, warehouses[j].a);
    }
    lp.set_b((3 * i) + n, -stadiums[i].d);
    lp.set_b((3 * i + 1) + n, stadiums[i].d);
    lp.set_b((3 * i + 2) + n, 100 * stadiums[i].u);
  }

  varNum = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      lp.set_c(varNum++, -revenues[i][j]);
    }
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));

  // output solution
  if (s.is_infeasible()) {
    cout << "RIOT!" << endl;
  } else {
    std::cout << fixed << setprecision(0) << floor_to_double(s) << endl;
  }
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) testcase();

  return 0;
}
