#include <iostream>
#include <thread>

struct task { // function object -> functor
  void operator()(int data){
     std::cerr << "task() is running..." << data << std::endl;
  }
};

int main() { // Main process
   task my_task{};
   std::thread t1{my_task, 100};
   std::thread t2{my_task, 200};
   std::thread t3{task{}, 300};
   std::cerr << "Application is just started." << std::endl; 
   t1.join();
   t2.join();
   t3.join();
   std::cerr << "Application is just completed." << std::endl; 
   return 0;
}

