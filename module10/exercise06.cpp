#include <iostream>
#include <generator>
#include <ranges>

using namespace std;

generator<int> fun(int seed){ // generator function -> lazy evaluation
    cout << "fun() has just started!" << endl;
    cout << "[fun] yielding " << seed << endl;
    co_yield seed;
    while (seed > 1){
        seed = seed%2 == 0 ? seed/2 : 3*seed+1;
        cout << "[fun] yielding " << seed << endl;
        co_yield seed;
    }
    cout << "fun() has just finished!" << endl;
}

int main(){
    for (const int next : fun(1'000'000) | std::views::take_while([](int u){
        cout << "[take_while] " << u << endl;
        return u >100;
    })){
        cout << "[for] " << next << endl;
    }
    return 0;
}