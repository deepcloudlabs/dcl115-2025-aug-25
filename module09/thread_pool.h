#include <iostream>
#include <thread>
#include <vector>
#include <functional>

#include "ts_queue.h"

using namespace std;

//region joinable_threads
class joinable_threads {
    vector<thread> &threads;
public:
    explicit joinable_threads(vector<thread> &threads_) : threads(threads_) {}

    ~joinable_threads() {
        for (auto &thread: threads) {
            if (thread.joinable())
                thread.join();
        }
    }
};
//endregion

class thread_pool {
    atomic_bool done;
    thread_safe_queue<function<void()>> work_queue;
    vector<thread> threads;
    joinable_threads joiner;
    unsigned thread_count = thread::hardware_concurrency();

    void worker_thread() {
        while (!done || !work_queue.empty()) {
            function<void()> task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                this_thread::sleep_for(50ms);
            }
        }
    }

public:
    thread_pool() : joiner(threads), done(false) {
        try {
            for (int i = 0; i < thread_count; ++i) {
                threads.emplace_back(&thread_pool::worker_thread, this);
            }
        }
        catch (...) {
            done = true;
            throw;             // re-throw
        }
    }
    void shutdown() {
        done = true;
    }
    ~thread_pool() {
        done = true;
    }

    template<typename FunctionType>
    void submit(FunctionType f) {
        if (done) return;
        work_queue.push(function<void()>(f));
    }
};
