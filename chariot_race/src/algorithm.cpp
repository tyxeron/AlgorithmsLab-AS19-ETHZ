#include <assert.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <stdexcept>
#include <vector>

std::tuple<int, bool> traverse(std::vector<std::vector<int>> &stages,
                               std::vector<int> &memoTaken,
                               std::vector<int> &memoNotTaken,
                               std::vector<int> &memoNotTakenPredTrue,
                               std::vector<int> &cost, int currPos,
                               bool predtaken, bool mustTake) {
  int nodeTakenCost = cost[currPos];
  int nodeNotTakenCost = 0;
  std::tuple<int, bool> result;

  if (stages[currPos].size() == 0) {
    if (!predtaken || mustTake) {
      result = std::make_tuple(cost[currPos], true);
      memoTaken[currPos] = cost[currPos];
    } else {
      result = std::make_tuple(0, false);
      memoNotTaken[currPos] = 0;
    }
  } else if (mustTake) {
    if (memoTaken[currPos] == -1) {
      for (auto next : stages[currPos]) {
        nodeTakenCost += std::get<0>(traverse(stages, memoTaken, memoNotTaken,
                                              memoNotTakenPredTrue, cost, next,
                                              true, false));
      }
      memoTaken[currPos] = nodeTakenCost;
    }

    result = std::make_tuple(memoTaken[currPos], true);
  } else {
    if (memoTaken[currPos] == -1) {
      // take this node and leave the choice to other node
      for (auto next : stages[currPos]) {
        nodeTakenCost += std::get<0>(traverse(stages, memoTaken, memoNotTaken,
                                              memoNotTakenPredTrue, cost, next,
                                              true, false));
      }

      memoTaken[currPos] = nodeTakenCost;
    }

    // pred is taken can take this or next or none
    if (predtaken) {
      if (memoNotTakenPredTrue[currPos] == -1) {
        // Don't take this node and leave the choice to other node
        for (auto next : stages[currPos]) {
          nodeNotTakenCost += std::get<0>(
              traverse(stages, memoTaken, memoNotTaken, memoNotTakenPredTrue,
                       cost, next, false, false));
        }

        memoNotTakenPredTrue[currPos] = nodeNotTakenCost;
      } else {
        nodeNotTakenCost = memoNotTakenPredTrue[currPos];
      }
    } else {
      if (memoNotTaken[currPos] == -1) {
        // pred is not taken: must either take this node or successor
        bool oneSuccTaken = false;

        for (auto next : stages[currPos]) {
          auto tuple = traverse(stages, memoTaken, memoNotTaken,
                                memoNotTakenPredTrue, cost, next, false, false);
          if (std::get<1>(tuple)) {
            oneSuccTaken = true;
          }
          nodeNotTakenCost += std::get<0>(tuple);
        }

        if (!oneSuccTaken) {
          nodeNotTakenCost = 0;
          int minCost = std::numeric_limits<int>::max();
          // Don't take this node at first and leave the choice to other node
          for (int i = 0; i < stages[currPos].size(); i++) {
            for (int j = 0; j < stages[currPos].size(); j++) {
              nodeNotTakenCost += std::get<0>(traverse(
                  stages, memoTaken, memoNotTaken, memoNotTakenPredTrue, cost,
                  stages[currPos][j], false, j == i));
            }

            minCost = std::min(minCost, nodeNotTakenCost);
            nodeNotTakenCost = 0;
          }
          nodeNotTakenCost = minCost;
        }
        memoNotTaken[currPos] = nodeNotTakenCost;
      } else {
        nodeNotTakenCost = memoNotTaken[currPos];
      }
    }
    if (nodeNotTakenCost < memoTaken[currPos]) {
      result = std::make_tuple(nodeNotTakenCost, false);
    } else {
      result = std::make_tuple(memoTaken[currPos], true);
    }
  }
  return result;
}

int calculateMinCost(std::vector<std::vector<int>> &stages,
                     std::vector<int> &cost) {
  std::vector<int> memoTaken(cost.size(), -1);
  std::vector<int> memoNotTaken(cost.size(), -1);
  std::vector<int> memoNotTakenPredTrue(cost.size(), -1);

  return std::get<0>(traverse(stages, memoTaken, memoNotTaken,
                              memoNotTakenPredTrue, cost, 0, false, false));
}

void testcase() {
  int n = 0, i = 0, j = 0;
  std::cin >> n;

  std::vector<std::vector<int>> stages(n);

  std::vector<int> costs(n);

  for (int k = 0; k < n - 1; k++) {
    std::cin >> i;
    std::cin >> j;
    stages[i].push_back(j);
  }

  for (int i = 0; i < n; i++) {
    std::cin >> costs[i];
  }

  std::cout << calculateMinCost(stages, costs) << std::endl;
}
int main(int argc, char const *argv[]) {
  std::ios_base::sync_with_stdio(false);
  int t = 0;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}
