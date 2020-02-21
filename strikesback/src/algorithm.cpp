#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

#include <CGAL/Gmpq.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;
struct Asteroid {
  K::Point_2 position;
  int density;
  Asteroid(int x, int y, int dens)
      : density(dens), position(K::Point_2(x, y)) {}
};
struct ShootingPoint {
  K::Point_2 position;
  double radius = numeric_limits<double>::max();
  ShootingPoint(int x, int y, double rad)
      : radius(rad), position(K::Point_2(x, y)) {}
  ShootingPoint(int x, int y) : position(K::Point_2(x, y)) {}
};
void testcase() {
  // read number of points
  int a, s, b;
  cin >> a >> s >> b;
  int e;
  cin >> e;
  vector<Asteroid> asteroids;
  asteroids.reserve(a);
  for (int i = 0; i < a; i++) {
    int x, y, d;
    cin >> x >> y >> d;

    asteroids.emplace_back(x, y, d);
  }

  Program lp(CGAL::LARGER, true, 0, false, 0);

  vector<ShootingPoint> shootingP;
  shootingP.reserve(s);

  for (int i = 0; i < s; i++) {
    int x, y;
    cin >> x >> y;
    shootingP.emplace_back(x, y);
    lp.set_a(i, a, 1);
  }

  lp.set_b(a, e);
  lp.set_r(a, CGAL::SMALLER);

  if (b > 0) {
    std::vector<K::Point_2> pts;
    pts.reserve(b);
    for (int i = 0; i < b; ++i) {
      int x, y;
      std::cin >> x >> y;
      pts.push_back(K::Point_2(x, y));
    }
    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    for (auto& shootingPoint : shootingP) {
      shootingPoint.radius =
          CGAL::to_double(CGAL::squared_distance(
              shootingPoint.position,
              t.nearest_vertex(shootingPoint.position)->point())) -
          DBL_EPSILON;
    }
  }

  // Construct Linear program

  for (int j = 0; j < a; j++) {
    auto asteroid = asteroids[j];
    lp.set_b(j, asteroid.density);
    for (int i = 0; i < s; i++) {
      auto shootingPoint = shootingP[i];
      double distance =
          CGAL::squared_distance(asteroid.position, shootingPoint.position);
      if (distance <= shootingPoint.radius) {
        // Add value since the distance is smaller than the radius
        lp.set_a(i, j, 1.0f / max((double)(1.0), distance));
      }
    }
  }
  // solve the program, using ET as the exact type
  Solution sol = CGAL::solve_linear_program(lp, ET());
  assert(sol.solves_linear_program(lp));

  // output solution
  if (sol.is_infeasible()) {
    cout << "n" << endl;
  } else {
    cout << "y" << endl;
  }
  // For each asteroid add the constraint
}
int main(int argc, char const* argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
