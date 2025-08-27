#include <thread>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

using namespace std;

const int cpus = thread::hardware_concurrency();

int bind_self_to_core(int core_id) {
     core_id = core_id % cpus;

     cpu_set_t cpuset;
     CPU_ZERO(&cpuset);
     CPU_SET(core_id, &cpuset);

     pthread_t current_thread = pthread_self();
     return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

void task(int cpu_id){
     bind_self_to_core(cpu_id);
     int cpu = sched_getcpu(); 
     cout << "Thread " << this_thread::get_id() << " is running on CPU " << cpu << endl;
}

int main(){
    vector<thread> threads{};
    cerr << "number of logical processors is " << thread::hardware_concurrency() << endl;
    auto cores = thread::hardware_concurrency();
    for (auto i=0;i<cores;++i){
        threads.emplace_back(thread{task,i});
    }
    for (auto& thread : threads) thread.join();
    return 0;
}
