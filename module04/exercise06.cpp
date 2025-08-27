#include <iostream>
#include <thread>
#include <latch>

using namespace std;

latch ready{5};
mutex m;
struct Worker {
    explicit Worker(const int &id) : id(id) {}
    void operator()(){
        cerr << "Worker #"  << id << " is just arrived..." << endl;
        if (id < 6) ready.count_down();
        ready.wait();
        {
            lock_guard<mutex> lg{m};
            cerr << "Worker #" << id << " is just started..." << endl;
        }
    }
private:
   int id;
};

int main() {
    cerr << "Application is just started!" << endl;
    Worker worker1{1};
    Worker worker2{2};
    Worker worker3{3};
    Worker worker4{4};
    Worker worker5{5};
    Worker worker6{6};
    Worker worker7{7};
    Worker worker8{8};
    Worker worker9{9};
    Worker worker10{10};
    {
        jthread t1 {worker1};
        this_thread::sleep_for(1s);
        jthread t2 {worker2};
        this_thread::sleep_for(1s);
        jthread t3 {worker3};
        this_thread::sleep_for(1s);
        jthread t4 {worker4};
        this_thread::sleep_for(1s);
        jthread t5 {worker5};
        this_thread::sleep_for(1s);
        jthread t6 {worker6};
        this_thread::sleep_for(1s);
        jthread t7 {worker7};
        this_thread::sleep_for(1s);
        jthread t8 {worker8};
        this_thread::sleep_for(1s);
        jthread t9 {worker9};
        this_thread::sleep_for(1s);
        jthread t10 {worker10};
        this_thread::sleep_for(1s);
    }
    cerr << "Application is just stopped!" << endl;
    return 0;
}
