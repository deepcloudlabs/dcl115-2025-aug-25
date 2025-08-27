#include <iostream>
#include <set>

using namespace std;

#include "world-util.h"
std::map<int,std::shared_ptr<world::city>> cities;
std::map<std::string,std::shared_ptr<world::country>> countries;

int main() {
    create_world();
    set<string> continents;
    // external loop -> not easy to solve in parallel
    // imperative
    for (auto &country: countries) {
        continents.insert(country.second->continent);
    }
    for (auto &continent: continents) {
       cout << continent << endl;
    }
    return 0;
}
