#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <cmath>

void testcase(){
    int n = 0, maxDistance = 0, currDistance = 0, maxPar = 0, currPar = 0;
    bool isEven = false;
    std::vector<int> barLoc;
    std::cin >> n;
    std::vector<int> parasolLoc(n);

    for(int i = 0; i < n; i++){
        std::cin >>  parasolLoc[i];
    }

    //sort parasols
    sort(parasolLoc.begin(), parasolLoc.end());


    if(n==1){
        std::cout << "1 0\n" << parasolLoc[0] << std::endl;
        return;
    }
    for(auto lowerBound =  parasolLoc.begin(); lowerBound < parasolLoc.end(); lowerBound++){
        auto upperBound = std::upper_bound(lowerBound, parasolLoc.end(), *lowerBound + 200);

        if(*upperBound != *lowerBound + 200){
            upperBound--;
        }
 
        bool isEven = (*upperBound - *lowerBound) % 2 == 0;
        currDistance = std::abs(std::floor((*upperBound - *lowerBound)/ 2)); //Plus minus
        if(!isEven){
            currDistance+=1;
            currPar = std::distance(lowerBound, upperBound) + 1;
            if(maxPar < currPar || (maxPar==currPar && maxDistance > currDistance)){
                maxPar = currPar;
                maxDistance = currDistance;
                barLoc.clear();
                barLoc.push_back(*upperBound - currDistance);
            }else if(maxPar==currPar && maxDistance == currDistance){
                barLoc.push_back(*upperBound - currDistance);
            }

            
            currPar = std::distance(lowerBound, upperBound) + 1;
            if(maxPar < currPar || (maxPar==currPar && maxDistance > currDistance)){
                maxPar = currPar;
                maxDistance = currDistance;
                barLoc.clear();
                barLoc.push_back(*lowerBound + currDistance);
            }else if(maxPar==currPar && maxDistance == currDistance){
                barLoc.push_back(*lowerBound + currDistance);
            }
        }else{
        //If dist uneven decide wich one

        currPar = std::distance(lowerBound, upperBound) + 1;
        if(maxPar < currPar || (maxPar==currPar && maxDistance > currDistance)){
            maxPar = currPar;
            maxDistance = currDistance;
            barLoc.clear();
            barLoc.push_back(*lowerBound + currDistance);
        }else if(maxPar==currPar && maxDistance == currDistance){
            barLoc.push_back(*lowerBound + currDistance);
        }
    }
    }

    std::cout << maxPar << " " << maxDistance << "\n";
    auto iter = barLoc.begin();
    for(; iter!= barLoc.end(); iter++){
        std::cout << *iter << " ";
    }
    
    std::cout << std::endl;

    
}

int main(int argc, char const *argv[]) {
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for (int i = 0; i < t; i++){
        testcase();
    }
    // TODO
}
