#include <iostream>
#include <set>
#include <execution>
#include <parallel/algorithm>
#include <parallel/numeric>

using namespace std;

#include "country.h"
#include "world-util.h"
std::map<int,std::shared_ptr<world::city>> cities;
std::map<std::string,std::shared_ptr<world::country>> countries;

int main() {
    create_world();
    set<string> continents;
    // internal loop -> stl algorithm [accumulate]
    // functional programming
    auto continent_reducer = [](auto continents,auto& country_pair) -> set<string>{
        continents.insert(country_pair.second->continent);
        return continents;
    };
    auto asian_filter = [](auto& country){
        return country.second->continent == "Asia";
    };
    vector<pair<std::string,std::shared_ptr<world::country>>> asian_countries;
    vector<long> populations;
    auto to_population = [](auto& country_pair){
        return country_pair.second->population;
    };
    // pipeline: HoF -> function parameter, function chain
    // filter/map/reduce -> copy_if/transform/accumulate
    // countries -> filter -> asian countries -> transform -> population -> accumulate -> solution
    // parallel pipeline
    copy_if(execution::par,countries.begin(),countries.end(), back_inserter(asian_countries),asian_filter);
    transform(asian_countries.begin(),asian_countries.end(),back_inserter(populations),to_population);
    auto total_population = reduce(execution::par,populations.begin(),populations.end(),long{},plus<long>{});
    cout << "total population is " << total_population << endl;
    return 0;
}
