#include <thread>
#include <future>
#include <iostream>
#include <numeric>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

using namespace std;
const long SIZE = 16'000'000;

vector<int> numbers{};

const int cpus = thread::hardware_concurrency();

int bind_self_to_core(int core_id) {
     core_id = core_id % cpus;

     cpu_set_t cpuset;
     CPU_ZERO(&cpuset);
     CPU_SET(core_id, &cpuset);

     pthread_t current_thread = pthread_self();
     return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

long task(int cpu_id,vector<int>& array,int start,int size){
     //bind_self_to_core(cpu_id);
     int cpu = sched_getcpu(); 
     cerr << "Thread - start: " << start  
                   << ",size: " << size << ", id: " << this_thread::get_id() << " is running on CPU " << cpu << endl << flush;
     // parallel task
     return accumulate(next(array.begin(), start),next(array.begin(), start+size),long{},plus<long>{});
}


int main(){
    ::numbers.reserve(SIZE);
    for (auto i=0;i<SIZE;i++){
       ::numbers.push_back(i);
    }
    vector<future<long>> futures{};
    cerr << "Number of logical processors is " << thread::hardware_concurrency() << endl << flush;
    auto cores = thread::hardware_concurrency();
    auto size = ::numbers.size() / cores;
    cerr << "starting the threads..." << endl << flush;
    auto start = chrono::high_resolution_clock::now();
    for (auto i=0,start=0;i<cores;++i,start+=size){
        futures.emplace_back(async(task,i,ref(::numbers),start,size));
    }
    long sum = 0;
    for (auto& partial_sum : futures) sum += partial_sum.get();
    cerr << "Done. sum is " << sum << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cerr << "Time spent: " << duration.count() << " microseconds" << endl << flush;
    return 0;
}
