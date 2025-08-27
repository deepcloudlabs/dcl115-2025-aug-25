#include <iostream>
#include <vector>
#include <generator>

using namespace std;

vector<int> legacy_get_evens(vector<int> numbers) {
    vector<int> solution{}; 
    for (const auto &number: numbers) {
        if (number%2 == 0){
           cerr << "Found an even number in legacy_get_evens(): " << number << endl << flush ;
           solution.push_back(number);
        }
    }
    return solution;
}

std::generator<int> generator_get_evens(vector<int> numbers) {
    for (const auto &number: numbers) {
        if (number%2 == 0){
           cerr << "Found an even number in get_evens(): " << number << endl << flush ;
           co_yield number;
        }
    }
}

auto
main() -> int {
    int x= 0;
    for (const auto& even : generator_get_evens(vector<int>{1,2,3,4,5,6,7,8,9,10})) {
        cout << "[for] " << even << endl;
        ++x;
        if (x==3) break;
    }
    return 0;
}
