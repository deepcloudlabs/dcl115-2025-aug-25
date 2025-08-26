#include <iostream>
#include <thread>
#include <barrier>

using namespace std;

barrier go{5};
mutex m;
struct Worker {
    explicit Worker(const string &label) : label(label) {}
    void operator()(){
        cerr << label << " is just arrived..." << endl;
        go.arrive_and_wait();
        {
            lock_guard<mutex> lg{m};
            cerr << label << " is just started..." << endl;
        }
    }
private:
   string label;
};

int main() {
    cerr << "Application is just started!" << endl;
    Worker worker1{"worker #1"};
    Worker worker2{"worker #2"};
    Worker worker3{"worker #3"};
    Worker worker4{"worker #4"};
    Worker worker5{"worker #5"};
    Worker worker6{"worker #6"};
    Worker worker7{"worker #7"};
    Worker worker8{"worker #8"};
    Worker worker9{"worker #9"};
    Worker worker10{"worker #10"};
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
