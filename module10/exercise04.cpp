#include <iostream>
#include <string>
#include <ranges>
#include <thread>
#include <mutex>

#include "world-util.h"
#include "city.h"
#include "country.h"

using namespace std;
using namespace world;

map<int , shared_ptr<city>> cities;
map<string , shared_ptr<country>> countries;

int main() { 
    create_world();
    auto is_asian = [](shared_ptr<country> & a_country) -> bool {
        cout << "[is_asian] " << a_country->name << endl;
        return a_country->continent.compare("Asia") == 0;
    };
    ranges::filter_view<ranges::elements_view<ranges::ref_view<decltype(countries)>,1>,decltype(is_asian)> asian_countries =
     countries | std::ranges::views::values | std::ranges::views::filter(is_asian) ;
    // in the future: | ranges::parallel;
    for (auto& a_country : asian_countries){
        cout << *a_country << endl;
    }
    /*
    for (auto& a_country : countries | std::ranges::views::values | std::ranges::views::filter(is_asian) ){
        cout << "[for] " << a_country->name << endl;
        if(true) break;
    }
    */
    return 0;
}
