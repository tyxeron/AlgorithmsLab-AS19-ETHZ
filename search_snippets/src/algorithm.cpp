#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <map>

bool allIn(std::vector<int> & slidingWindow){
    for(auto &it : slidingWindow){
        if(it <= 0){
            return false;
        }
    }
    return true;
}

void testcase(){
    int n = 0, tmp = 0;

    std::cin >> n;
    std::vector<int> wordCounts(n);
    std::map<int,int> document; 
    std::vector<int> slidingWindow(n, 0);

    for(int i = 0; i < n; i++){
        std::cin >> tmp;
        wordCounts[i] = tmp;
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < wordCounts[i]; j++){
            std::cin >> tmp;
            document.insert(std::make_pair(tmp, i));
        }
    }

    auto i = document.begin(), j = i, next = j;
    int minLength = std::numeric_limits<int>::max();

    slidingWindow[i->second] = 1;

    while(true){    

        while(slidingWindow[i->second] > 1){
            slidingWindow[i->second]--;
            i++;
        }

        
        if(allIn(slidingWindow)){
            minLength = std::min( j==i ? 1 : j->first - i->first + 1, minLength);
        }

        next = j;
        next++;

        if(next == document.end()){
            break;
        }else{
            j++;
            slidingWindow[j->second]++;
        }
        
    }
        
    std::cout <<  minLength << std::endl;


}

int main(int argc, char const *argv[]) {
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i< t; i++){
        testcase();
    }
}
