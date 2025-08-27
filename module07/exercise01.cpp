#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

#include "ts_atomic_queue.h"

atomic<int> counter{0};
ts_atomic_queue<int> state;
mutex m_cerr;

void fun(int start, int stop) {
    for (auto i = start; i <= stop; ++i) {
        state.push(i);
        // this_thread::sleep_for(2ms);
    }
}

void gun(int fetch_size) {
    for (auto i = 0; i < fetch_size; ++i) {
        {
            lock_guard<mutex> lg{m_cerr};
            auto popped_vale = state.pop();
            if (popped_vale == nullptr) continue;
            counter.fetch_add(*popped_vale,memory_order_acq_rel);
            cerr << "pop[" << this_thread::get_id() << "]: " << *popped_vale << endl;
        }
        // this_thread::sleep_for(1ms);
    }
}

int main() {
    {
        jthread t1{fun, 1, 1'000};
        jthread t2{fun, 1'001, 2'000};
        jthread t3{gun, 500};
        jthread t4{gun, 500};
        jthread t5{gun, 500};
        jthread t6{gun, 500};
    }
    cerr << "queue size is " << state.size() << endl;
    cerr << "queue status is " << (state.empty() ? "empty" : "not empty") << endl;
    cerr << "counter is " << counter.load(memory_order_acquire) << endl;

    return 0;
}
