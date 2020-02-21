#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

int playRound( std::vector<std::vector<int>> & memo, const std::vector<int> & coins, const int begin,  const int end, int  i ){
    if(begin==end){
        return i%2 == 0 ? coins[begin] : 0;
    }
    
    /*
    * Pseudo code for memo:
    * If have seen coin v_i to v_j save or retrieve solution
    */

    if(memo[begin][end] == -1){
        memo[begin][end] = i%2==0?
        std::max(coins[end] + playRound(memo,coins,begin, end -1, i + 1), coins[begin] + playRound(memo,coins,begin + 1, end, i + 1)):
        std::min(playRound(memo,coins,begin, end -1, i + 1), playRound(memo,coins,begin + 1, end, i + 1));
    }
    return memo[begin][end];
    

}

void testcase(){
    int n  = 0;
    std::cin >> n;
    std::vector<int> coins(n);
    std::vector<std::vector<int>> memo(n,std::vector<int>(n, -1));

    for(int i = 0; i < n ; i ++){
        std::cin >> coins[i];
    }

    std::cout << playRound(memo, coins, 0, n - 1 , 0) << std::endl;
}

int main(int argc, char const *argv[]) {
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; i++){
        testcase();
    }
}
