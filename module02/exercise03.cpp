#include <iostream>
#include <thread>
#include <functional>
#include <numeric>

using namespace std;

void gun(){
    cerr << "gun() is running..." << endl;
    this_thread::sleep_for(500s);
}

void fun(thread t){
    cerr << "fun() is running..." << endl;
    if(t.joinable()) t.join();
    cerr << "fun() is returning..." << endl;
}

int main(){ // Main Process
   cerr << "Application is just started..." << endl;
   thread t1{gun};
   //t1.detach();
   fun(move(t1));
   cerr << "Application is just completed..." << endl;
   return 0;
}
