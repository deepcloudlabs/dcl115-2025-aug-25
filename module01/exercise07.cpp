#include <iostream>
#include <thread>
#include <functional>
using namespace std;

struct task { // class -> state 
public:
  explicit task(const int state=0) : state() {}
  void operator()(int task_id){
     std::cerr << "task[" << task_id << "] is running..." << std::endl;
     for (auto i=0;i<1'000'000;++i) this->state++;
     std::cerr << "task[" << task_id << "] is returning..." << state << std::endl;
  }
  int get_state() const { return this->state; }
private:
     int state;
};

int state = 0; // global state

void fun(int task_id){
   std::cerr << "task[" << task_id << "] is running..." << std::endl;
   for (auto i=0;i<1'000'000;++i) ::state++;
   std::cerr << "task[" << task_id << "] is returning..." << state << std::endl;
}

int main() { // Main process
   task task1{}; // process' stack
   //task task2{}; // process' stack
   //task task3{}; // process' stack
   std::thread t1{task1 , 1};
   std::thread t2{fun , 2};
   std::thread t3{fun , 3};
   std::cerr << "Application is just started." << std::endl; 
   t1.join();
   t2.join();
   t3.join();
   std::cerr << "task1->state:  " << task1.get_state() << endl;
   std::cerr << "::state:  " << ::state << endl;
   //std::cerr << "task3->state:  " << task3.get_state() << endl;
   std::cerr << "Application is just completed." << std::endl; 
   return 0;
}

