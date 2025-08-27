#include <coroutine>
#include <iostream>

struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;

    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(Task&& other) noexcept : handle(other.handle) { other.handle = {}; }
    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = {};
        }
        return *this;
    }
    ~Task() { if (handle) handle.destroy(); }

    void resume() {
        if (handle && !handle.done()) {
            handle.resume();
        }
    }
};

Task hello_coroutine() {
    std::cout << "Hello ";
    co_await std::suspend_always{}; // first suspension
    std::cout << "World!\n";
    co_await std::suspend_always{}; // first suspension
    std::cout << "Mars!\n";
}

int main() {
    auto task = hello_coroutine();
    std::cout << "Resuming...\n";
    task.resume();
    task.resume();
    task.resume();
}

       
