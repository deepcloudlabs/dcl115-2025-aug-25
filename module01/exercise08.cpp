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

int main() { // Main process
   task task1{}; // process' stack
   std::cerr << "Application is just started." << std::endl; 
   {
       std::jthread t1{task1 , 1};
   } // ~jthread() -> join
   std::cerr << "Application is just completed." << std::endl; 
   return 0;
}

