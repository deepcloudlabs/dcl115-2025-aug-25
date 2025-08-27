#include <iostream>
#include <stack>
#include <atomic>
#include <memory>
#include <thread>

using namespace std;

template<typename T>
class ts_atomic_queue {
    struct node {
        node *next;
        node *previous;
        shared_ptr<T> value;
        explicit node(T value) : value(make_shared<T>(value)), next{nullptr}, previous{nullptr} {}
    };

    atomic<node *> head;
    atomic<node *> tail;
    atomic<int> counter;
public:
    explicit ts_atomic_queue() : head{nullptr}, tail{nullptr}, counter{0} {
    }

    void push(const T value) { // tail
        auto new_node = new node(value);

        node *tmp = nullptr;
        if (tail.compare_exchange_strong(tmp, new_node)) {
            head.store(new_node, std::memory_order_relaxed);
            return;
        }

        node *old_tail;
        do {
            old_tail = tail;
            new_node->previous = old_tail;
        } while (!tail.compare_exchange_strong(old_tail, new_node));
        new_node->previous->next = new_node;
    }

    shared_ptr<T> pop() { // head
        if (head.load(std::memory_order_relaxed) == nullptr) {
            return nullptr;
        }
        auto value = head.load()->value;
        node *tmp = nullptr;
        node *new_head = head;
        if (tail.compare_exchange_strong(new_head, tmp)) {
            head.store(tmp, std::memory_order_relaxed);
            return value;
        }

        node *old_head;
        do {
            old_head = head;
        } while (head && !head.compare_exchange_strong(old_head, old_head->next));
        delete old_head;
        return value;
    }

    bool empty() {
        return head.load() == nullptr && tail.load() == nullptr;
    }

    int size() {
        return counter.load();
    }
};
