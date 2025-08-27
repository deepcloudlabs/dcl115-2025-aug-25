#include <iostream>
#include <generator>

using namespace std;

generator<int> fun(int cnt){ // generator function -> lazy evaluation
    int a = 0;
    co_yield a;
    int b = 1;
    co_yield b;
    for (; cnt > 0; --cnt) {
        int next = a + b;
        a = b;
        b = next;
        co_yield next;
    }
}

int main(){
    for (const int next : fun(10) ){
        cout << "[for] " << next << endl;
    }
    return 0;
}