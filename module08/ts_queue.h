#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class ts_queue {
    queue<T> que;
    mutex m; // for thread-safety
    condition_variable cv; // synchronization?
public:
    void push(T& item);
    T pop();
    int size();
    bool empty();
};

template<typename T>
void ts_queue<T>::push(T &item) {
    unique_lock<mutex> lock{m};
    que.push(item);
    cv.notify_one();
}

template<typename T>
T ts_queue<T>::pop() {
    unique_lock<mutex> lock{m};
    cv.wait(lock,[this](){return !this->que.empty();});
    auto item = que.front();
    que.pop();
    return item;
}

template<typename T>
int ts_queue<T>::size() {
    unique_lock<mutex> lock{m};
    return que.size();
}

template<typename T>
bool ts_queue<T>::empty() {
    unique_lock<mutex> lock{m};
    return que.empty();
}

