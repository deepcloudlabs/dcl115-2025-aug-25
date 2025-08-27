#include <vector>
#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#include <mutex>

using namespace std;

counting_semaphore<1> vehicle{20};
mutex m;

void moving(int no) {
    vehicle.acquire();
    {
        lock_guard<mutex> lg(m);
        cerr << "The passenger (" << no << ") gets on the vehicle." << endl;
    }
    this_thread::sleep_for(3s);
    vehicle.release();
    {
        lock_guard<mutex> lg(m);
        cerr << "The passenger (" << no << ") leaves the vehicle." << endl;
    }
}

int main() {
    cerr << "Application is just started..." << endl;
    vector<thread> passengers{};
    for (int i = 1; i <= 100; ++i) {
        passengers.push_back(thread(moving, i));
    }
    {
        lock_guard<mutex> lg(m);
        cerr << "All threads are created!" << endl;
    }
    for (auto &t: passengers) {
        t.join();
    }
    cerr << "Application is done." << endl;
    return 0;
}
