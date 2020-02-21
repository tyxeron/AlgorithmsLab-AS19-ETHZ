#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

int minGame(const std::vector<std::vector<int>> & transitions, int n, int currentPos, std::vector<int> & minMemo, std::vector<int> & maxMemo);
int maxGame(const std::vector<std::vector<int>> & transitions, int n, int currentPos, std::vector<int> & minMemo, std::vector<int> & maxMemo);


int minGame(const std::vector<std::vector<int>> & transitions, int n, int currentPos, std::vector<int> & minMemo, std::vector<int> & maxMemo){
    int minMove = std::numeric_limits<int>::max();
    //We are at the end
    if(currentPos == n){
        return 0;
    }

    for(auto nextPos: transitions[currentPos]){
        if(minMemo[nextPos] == -1){
            minMemo[nextPos] = maxGame(transitions, n, nextPos, minMemo, maxMemo);
        }

        minMove = std::min(minMove, minMemo[nextPos]);
    }

    return minMove + 1;
}

int maxGame(const std::vector<std::vector<int>> & transitions, int n, int currentPos, std::vector<int> & minMemo, std::vector<int> & maxMemo){
    int maxMove = std::numeric_limits<int>::min();
    //We are at the end
    if(currentPos == n){
        return 0;
    }

    for(auto nextPos: transitions[currentPos]){
        if(maxMemo[nextPos] == -1){
            maxMemo[nextPos] = minGame(transitions, n, nextPos, minMemo, maxMemo);
        }

        maxMove = std::max(maxMove, maxMemo[nextPos]);
    }

    return maxMove + 1;
}


void testcase(){
    int n = 0, m = 0, r = 0, b = 0, u = 0, v = 0, res = 0;

    std::cin >> n;
    std::cin >> m;
    std::cin >> r;
    std::cin >> b;

    std::vector<std::vector<int>> transitions (m + 1);
    std::vector<int> minMemo(n+1, -1);
    std::vector<int> maxMemo(n+1, -1);

    for(int i = 0; i < m; i++){
        std::cin >> u;
        std::cin >> v;
        transitions[u].push_back(v);
    }

    int sherlockMoves = minGame(transitions, n, r, minMemo, maxMemo);
    int moriartyMoves = minGame(transitions, n, b, minMemo, maxMemo);

    if (sherlockMoves < moriartyMoves)
		res = 0;
	else if (sherlockMoves == moriartyMoves && (sherlockMoves % 2 == 0))
		res = 1; 
	else if (sherlockMoves == moriartyMoves)
		res = 0;
	else
        res = 1;

    std::cout << res << std::endl;
}

int main(int argc, char const *argv[]) {
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; i++){
        testcase();
    }
    // TODO
    return 0;
}
