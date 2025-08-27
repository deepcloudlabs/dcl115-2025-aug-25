#include <iostream>
#include <numeric>
#include <vector>
#include <chrono>

using namespace std;
const long SIZE = 16'000'000;

vector<int> numbers{};

int main(){
    ::numbers.reserve(SIZE);
    for (auto i=0;i<SIZE;i++){
       ::numbers.push_back(i);
    }
    auto start = chrono::high_resolution_clock::now();
    auto sum = accumulate(::numbers.begin(), ::numbers.end(),long{},plus<long>{});
    cerr << "Done. sum is " << sum << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cerr << "Time spent: " << duration.count() << " microseconds" << endl << flush;
    return 0;
}
