#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

#include "ts_queue.h"

atomic<long> counter{0};
ts_queue<int> state;
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
            //lock_guard<mutex> lg{m_cerr};
            auto popped_vale = state.pop();
            counter.fetch_add(popped_vale,memory_order_acq_rel);
            //cerr << "pop[" << this_thread::get_id() << "]: " << popped_vale << endl;
        }
        // this_thread::sleep_for(1ms);
    }
}

int main() {
    {
        jthread t1{fun, 1, 200'000};
        jthread t2{fun, 200'001, 400'000};
        jthread t11{fun, 400'001, 600'000};
        jthread t12{fun, 600'001, 800'000};
        jthread t3{gun, 100'000};
        jthread t4{gun, 100'000};
        jthread t5{gun, 100'000};
        jthread t6{gun, 100'000};
        jthread t7{gun, 100'000};
        jthread t8{gun, 100'000};
        jthread t9{gun, 100'000};
        jthread t10{gun, 100'000};
    }
    cerr << "queue size is " << state.size() << endl;
    cerr << "queue status is " << (state.empty() ? "empty" : "not empty") << endl;
    cerr << "counter is " << counter.load(memory_order_acquire) << endl;

    return 0;
}
