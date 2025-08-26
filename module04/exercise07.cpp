#include <vector>
#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#include <numeric>
#include <future>

using namespace std;

binary_semaphore signaling{0};

vector<int> nums{};

void prepare_data(){
    cout << "preparing data..." << flush;
    nums.insert(nums.end(),{4,8,15,16,23,42});
    this_thread::sleep_for(5s);
    cout << "done" << endl << flush;
    signaling.release();
}

int process_data(){
    cout << "waiting for data..." << flush;
    signaling.acquire(); // blocked
    cout << "done" << endl << flush;
    return accumulate(nums.begin(),nums.end(),int());
}

int main(){
    cout << "Application is just started..." << endl << flush;
    thread t1(prepare_data);
    auto total = async(process_data);
    cout << "Sum is " << total.get() << endl << flush;
    t1.join();
    return 0;
}
