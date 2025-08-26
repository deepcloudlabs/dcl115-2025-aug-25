#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

using namespace std;

void task(int id){
    cerr << "task[" << id << "] is running..." << endl;
    this_thread::sleep_for(60s);
    cerr << "task[" << id << "] is returning..." << endl;
}

void fun(){
    vector<thread> threads{};
    cerr << "fun() is running..." << endl;
    for (auto i=0;i<128;++i){
       threads.emplace_back((thread(task,i)));
    }
    for_each(threads.begin(),threads.end(),mem_fn(&thread::join));
    cerr << "fun() is returning..." << endl;
}

int main(){ // Main Process
   cerr << "Application is just started..." << endl;
   {
      jthread t1{fun};
   }
   cerr << "Application is just completed..." << endl;
   return 0;
}
