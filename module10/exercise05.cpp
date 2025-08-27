#include <vector>
#include <iostream>

using namespace std;

vector<int> fun(int seed){
    cout << "fun() has just started!" << endl;
    vector<int> result{};
    result.push_back(seed);
    while (seed > 1){
        seed = seed%2 == 0 ? seed/2 : 3*seed+1;
        result.push_back(seed);
    }
    cout << "fun() has just finished!" << endl;
    return result;
}

int main(){
    for (auto& number : fun(1'000'000)){
        cout << "[for] " << number << endl;
        if (number < 100) break;
    }
    return 0;
}