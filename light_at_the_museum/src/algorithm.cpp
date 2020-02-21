#include <algorithm>
#include <bitset>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

struct Result {
  int nSwitches;
  int totalBrightness;
  vector<int> switches;
};

vector<Result> possibilities(vector<vector<int>> &on, vector<vector<int>> &off,
                             vector<int> targetBrightness, int nSwitches,
                             int nRooms) {
  vector<Result> result;
  for (int s = 0; s < (1 << nSwitches); ++s) {
    int number_switches = 0;
    int totalBrightness = 0;
    vector<int> brightness(nRooms, 0);
    for (int i = 0; i < nSwitches; ++i) {
      if (s & (1 << i)) {
        for (int j = 0; j < nRooms; ++j) {
          brightness[j] += off[i][j];
          totalBrightness += off[i][j];
        }
        number_switches++;
      } else {
        for (int j = 0; j < nRooms; ++j) {
          brightness[j] += on[i][j];
          totalBrightness += on[i][j];
        }
      }
    }

    bool legal = true;
    for (int i = 0; i < nRooms; ++i) {
      if (brightness[i] > targetBrightness[i]) {
        legal = false;
        break;
      }
    }
    if (legal) {
      result.push_back({number_switches, totalBrightness, brightness});
    }
  }
  return result;
}

bool cmp(const Result a, const Result b) {
  return a.totalBrightness < b.totalBrightness;
}

void testcase() {
  int nSwitches = 0, nRooms = 0;
  cin >> nSwitches >> nRooms;

  int total_brightness = 0;
  vector<int> targetBrightness(nRooms);
  for (int i = 0; i < nRooms; ++i) {
    cin >> targetBrightness[i];
    total_brightness += targetBrightness[i];
  }

  int firstHalf = nSwitches / 2;
  int secondHalf = nSwitches - firstHalf;

  vector<vector<int>> onFirstHalf(firstHalf, vector<int>(nRooms));
  vector<vector<int>> onSecondHalf(secondHalf, vector<int>(nRooms));
  vector<vector<int>> offFirstHalf(firstHalf, vector<int>(nRooms));
  vector<vector<int>> offSecondHalf(secondHalf, vector<int>(nRooms));
  
  for (int i = 0; i < firstHalf; ++i) {
    for (int j = 0; j < nRooms; ++j) {
      cin >> onFirstHalf[i][j] >> offFirstHalf[i][j];
    }
  }
  for (int i = 0; i < secondHalf; ++i) {
    for (int j = 0; j < nRooms; ++j) {
      cin >> onSecondHalf[i][j] >> offSecondHalf[i][j];
    }
  }

  vector<Result> firstHalfResults = possibilities(
      onFirstHalf, offFirstHalf, targetBrightness, firstHalf, nRooms);
  vector<Result> secondHalfResults = possibilities(
      onSecondHalf, offSecondHalf, targetBrightness, secondHalf, nRooms);

  sort(secondHalfResults.begin(), secondHalfResults.end(), cmp);

  int min_number_switches = INT_MAX;
  for (Result firstHalfResult : firstHalfResults) {
    Result target = {0, total_brightness - firstHalfResult.totalBrightness,
                     vector<int>()};
    auto range = equal_range(secondHalfResults.begin(), secondHalfResults.end(),
                             target, cmp);
    for (auto it = range.first; it != range.second; ++it) {
      // check if valid
      bool valid = true;
      for (int i = 0; i < nRooms; ++i) {
        if (firstHalfResult.switches[i] + it->switches[i] !=
            targetBrightness[i]) {
          valid = false;
        }
      }
      if (valid &&
          (firstHalfResult.nSwitches + it->nSwitches < min_number_switches)) {
        min_number_switches = firstHalfResult.nSwitches + it->nSwitches;
      }
    }
  }

  if (min_number_switches == INT_MAX) {
    cout << "impossible" << endl;
  } else {
    cout << min_number_switches << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) testcase();
}