#include <iostream>
#include <stack>
#include <atomic>
#include <memory>
#include <thread>

using namespace std;

template<typename T>
class ts_stack {
    struct node {
        node* next;
        shared_ptr<T> value;
        explicit node(T value) : value(make_shared<T>(value)) {}
    };
    atomic<node*> head;
    atomic<int> counter;
public:

    void push(T value) {
        node* newNode = new node(value);
        newNode->next = head.load();
        while (!head.compare_exchange_weak(newNode->next, newNode));
        counter++;
    }

    shared_ptr<T> pop() {
        auto oldHead = head.load();
        while(oldHead && !head.compare_exchange_weak(oldHead,oldHead->next));
        --counter;
        return oldHead ? oldHead->value : shared_ptr<T>();
    }

    bool empty() {
        return head.load() == nullptr;
    }

    int size() {
        return counter.load();
    }
};

ts_stack<int> my_numbers;

void fun(int start, int count) {
    for (int i = start, j = 0; j < count; ++i, ++j) {
        my_numbers.push(i);
    }
}

int main() {
    {
    jthread t1(fun,0,100'000'000);
    jthread t2(fun,100'000'000,100'000'000);
    jthread t3(fun,200'000'000,100'000'000);
    jthread t4(fun,300'000'000,100'000'000);
    }
    cout << my_numbers.size() << endl;
    return 0;
}
