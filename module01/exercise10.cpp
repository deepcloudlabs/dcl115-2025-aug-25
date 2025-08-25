#include <iostream>
#include <thread>
#include <future>
using namespace std;

int fun(int task_id){ // synchronous function -> long running process
   std::cerr << "task[" << task_id << "] is just started." << std::endl; 
   this_thread::sleep_for(4s);
   std::cerr << "task[" << task_id << "] is returning." << std::endl; 
   return 42;
}

int gun(int task_id){ // 
     packaged_task<int(int)> task{fun};
     future<int> future_result = task.get_future(); 
     thread t{std::move(task),task_id};
     t.detach();
     return future_result.get();   
}

int main() { // Main process
   std::cerr << "Application is just started." << std::endl; 
   auto result = gun(1);
   std::cerr << "result is " << result << std::endl;
   std::cerr << "Application is just completed." << std::endl; 
   return 0;
}

