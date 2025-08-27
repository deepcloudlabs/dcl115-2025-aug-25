#include <iostream>
#include <set>
#include <execution>
#include <parallel/algorithm>
#include <parallel/numeric>

using namespace std;

#include "world-util.h"
std::map<int,std::shared_ptr<world::city>> cities;
std::map<std::string,std::shared_ptr<world::country>> countries;
struct ContinentReducer {
   auto operator()(auto continents,auto& country_pair){
        continents.insert(country_pair.second->continent);
        return continents;
   }
};

int main() {
    create_world();
    set<string> continents;
    // internal loop -> stl algorithm [accumulate]
    // functional programming
    auto continent_reducer = [](auto continents,auto& country_pair) -> set<string>{
        continents.insert(country_pair.second->continent);
        return continents;
    };
    continents = reduce(execution::par,countries.begin(),countries.end(),set<string>{},continent_reducer);
    for (auto &continent: continents) {
       cout << continent << endl;
    }
    return 0;
}
