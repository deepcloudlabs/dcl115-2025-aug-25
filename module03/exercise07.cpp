// https://en.wikipedia.org/wiki/Dining_philosophers_problem
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <semaphore>
#include <thread>

constexpr const size_t N = 5;  // number of philosophers (and forks)
enum class State 
{
    THINKING = 0,  // philosopher is THINKING
    HUNGRY = 1,    // philosopher is trying to get forks
    EATING = 2,    // philosopher is EATING
};

size_t inline left(size_t i) 
{  
    // number of the left neighbor of philosopher i, for whom both forks are available
    return (i - 1 + N) % N; // N is added for the case when  i - 1 is negative
}

size_t inline right(size_t i) 
{  
    // number of the right neighbor of the philosopher i, for whom both forks are available
    return (i + 1) % N;
}

State state[N];  // array to keep track of everyone's both_forks_available state

std::mutex critical_region_mtx;  // mutual exclusion for critical regions for 
// (picking up and putting down the forks)
std::mutex output_mtx;  // for synchronized cout (printing THINKING/HUNGRY/EATING status)

// array of binary semaphores, one semaphore per philosopher.
// Acquired semaphore means philosopher i has acquired (blocked) two forks
std::binary_semaphore both_forks_available[N]
{
    std::binary_semaphore{0}, std::binary_semaphore{0},
    std::binary_semaphore{0}, std::binary_semaphore{0},
    std::binary_semaphore{0}
};

size_t my_rand(size_t min, size_t max) 
{
    static std::mt19937 rnd(std::time(nullptr));
    return std::uniform_int_distribution<>(min, max)(rnd);
}

void test(size_t i) 
// if philosopher i is hungry and both neighbors are not eating then eat
{ 
    // i: philosopher number, from 0 to N-1
    if (state[i] == State::HUNGRY &&
        state[left(i)] != State::EATING &&
        state[right(i)] != State::EATING) 
    {
        state[i] = State::EATING;
        both_forks_available[i].release(); // forks are no longer needed for this eat session
    }
}

void think(size_t i) 
{
    size_t duration = my_rand(400, 800);
    {
        std::lock_guard<std::mutex> lk(output_mtx); // critical section for uninterrupted print
        std::cout << i << " is thinking " << duration << "ms\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

void take_forks(size_t i)
{
    {
        std::lock_guard<std::mutex> lk{critical_region_mtx};  // enter critical region
        state[i] = State::HUNGRY;  // record fact that philosopher i is State::HUNGRY
        {
            std::lock_guard<std::mutex> lk(output_mtx); // critical section for uninterrupted print
            std::cout << "\t\t" << i << " is State::HUNGRY\n";
        }
        test(i);                        // try to acquire (a permit for) 2 forks
    }                                   // exit critical region
    both_forks_available[i].acquire();  // block if forks were not acquired
}

void eat(size_t i)
{
    size_t duration = my_rand(400, 800);
    {
        std::lock_guard<std::mutex> lk(output_mtx); // critical section for uninterrupted print
        std::cout << "\t\t\t\t" << i << " is eating " << duration << "ms\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

void put_forks(size_t i) 
{ 
    
    std::lock_guard<std::mutex> lk{critical_region_mtx};    // enter critical region
    state[i] = State::THINKING;  // philosopher has finished State::EATING
    test(left(i));               // see if left neighbor can now eat
    test(right(i));              // see if right neighbor can now eat
                                 // exit critical region by exiting the function
}

void philosopher(size_t i)
{  
    while (true) 
    {                         // repeat forever
        think(i);             // philosopher is State::THINKING
        take_forks(i);        // acquire two forks or block
        eat(i);               // yum-yum, spaghetti
        put_forks(i);         // put both forks back on table and check if neighbors can eat
    }
}

int main() {
    std::cout << "dp_14\n";

    std::jthread t0([&] { philosopher(0); }); // [&] means every variable outside the ensuing lambda 
    std::jthread t1([&] { philosopher(1); }); // is captured by reference
    std::jthread t2([&] { philosopher(2); });
    std::jthread t3([&] { philosopher(3); });
    std::jthread t4([&] { philosopher(4); });
}
