#include <stack>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <condition_variable>

using namespace std;

mutex cout_mutex;

template<class T>
class ts_stack {
    stack<T> my_data;
    mutex m;
    condition_variable cv;
public:
    ts_stack() {}

    ts_stack(const ts_stack &);

    ts_stack &operator=(const ts_stack &) = delete;

    void push(T new_value);

    shared_ptr<T> pop();

    void pop(T &value);

    bool empty();
};

template<class T>
ts_stack<T>::ts_stack(const ts_stack &other) {
    lock_guard<mutex> lock(other.m);
    my_data = other.my_data;
}

template<class T>
void ts_stack<T>::push(T new_value) {
    lock_guard<mutex> lock(m);
    my_data.push(new_value);
    cv.notify_all();
}

template<class T>
shared_ptr<T> ts_stack<T>::pop() {
    unique_lock<mutex> ul(m);
    cv.wait(ul, [this] { return !this->my_data.empty(); });
    shared_ptr<T> const result(make_shared<T>(my_data.top()));
    my_data.pop();
    return result;
}

template<class T>
void ts_stack<T>::pop(T &value) {
    lock_guard<mutex> lock(m);
    cv.wait(lock, [this] { return !this->my_data.empty(); });
    value = my_data.top();
    my_data.pop();
}

template<class T>
bool ts_stack<T>::empty() {
    lock_guard<mutex> lock(m);
    return my_data.empty();
}

ts_stack<int> tss{};

void fun() { // producer
    for (auto i = 0; i < 1'000; ++i) {
        tss.push(i);
        {
            lock_guard<mutex> lg(cout_mutex);
            cerr << "Successfully pushed the integer to the stack: "
                 << i
                 << endl;
        }
        this_thread::sleep_for(10ms);
    }
}

void gun() { // consumer
    for (auto i = 0; i < 250; ++i) {
        auto read_from_stack = *tss.pop();
        {
            lock_guard<mutex> lg(cout_mutex);
            cerr << "["
                 << this_thread::get_id()
                 << "] Trying to read from the stack: "
                 << read_from_stack
                 << endl;
        }
    }
}

int main() {
    cerr << "Application is just started." << endl;
    {
    jthread producer(fun);
    jthread consumer1(gun);
    jthread consumer2(gun);
    jthread consumer3(gun);
    jthread consumer4(gun);
    }
    cerr << "Application is done: " << (tss.empty()? "empty": "not empty")<< endl;
    return 0;
}
