#include <memory>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>

using namespace std;
#pragma once

template<typename T>
class thread_safe_queue {
    mutex mut;
    queue<T> data_queue;
    condition_variable data_cond;
public:
    thread_safe_queue() {}

    thread_safe_queue(const thread_safe_queue &) = delete;

    thread_safe_queue &operator=(const thread_safe_queue &) = delete;

    void push(T new_value);

    bool try_pop(T &value);

    T pop();

    bool empty() {
        lock_guard<mutex> lk(mut);
        return data_queue.empty();
    }
};

template<typename T>
void thread_safe_queue<T>::push(T new_value) {
    lock_guard<mutex> lk(mut);
    data_queue.push(new_value);
    data_cond.notify_one();
}


template<typename T>
bool thread_safe_queue<T>::try_pop(T &value) {
    unique_lock<mutex> lk(mut);
    if (!data_queue.empty()) {
        value = data_queue.front();
        data_queue.pop();
        return true;
    }
    return false;
}


template<typename T>
T thread_safe_queue<T>::pop() {
    unique_lock<mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    T value = data_queue.front();
    data_queue.pop();
    return value;
}
