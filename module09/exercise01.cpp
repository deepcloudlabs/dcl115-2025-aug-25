#include <thread>
#include <iostream>
#include <vector>
#include <pthread.h>

using namespace std;
void task(){
      int cpu = sched_getcpu(); 
      cout << "Thread " << this_thread::get_id() << " is running on CPU " << cpu << endl;
}

int main(){
    vector<thread> threads{};
    cerr << "number of logical processors is " << thread::hardware_concurrency() << endl;
    auto cores = thread::hardware_concurrency();
    for (auto i=0;i<cores;++i){
        threads.emplace_back(thread{task});
    }
    for (auto& thread : threads) thread.join();
    return 0;
}
