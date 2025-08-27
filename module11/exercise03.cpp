#include <coroutine>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

template<typename T>
struct Generator {
    struct promise_type {
        std::optional<T> current_value;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        std::suspend_always yield_value(T value) {
            current_value = std::move(value);
            return {};
        }

        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;

    explicit Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    Generator(Generator&& other) noexcept : handle(other.handle) { other.handle = {}; }
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = {};
        }
        return *this;
    }
    ~Generator() { if (handle) handle.destroy(); }

    bool next() {
        if (!handle || handle.done()) return false;
        handle.resume();
        return !handle.done();
    }

    T value() {
        return *handle.promise().current_value;
    }
};

Generator<int> get_evens(std::vector<int> numbers) {
    for (const auto &number: numbers) {
        if (number%2 == 0)
           co_yield number;
    }
}

int main() {
    auto evens = get_evens(std::vector<int>{1,2,3,4,5,6,7,8,9,10}) ;

    while (evens.next()) {
        std::cout << evens.value() << " ";
    }
    std::cout << "\n";
    return 0;
}
