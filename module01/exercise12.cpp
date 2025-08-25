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

future<int> gun(int task_id){ //asynchronous function3yy 
     return async(fun,task_id);
}

future<int> async_fun(int task_id){ //asynchronous function3yy 
     return async([](int tid){
                     std::cerr << "task[" << tid << "] is just started." << std::endl; 
                     this_thread::sleep_for(10s);
                     std::cerr << "task[" << tid << "] is returning." << std::endl; 
                     return 42;
                },task_id);
}

int main() { // Main process
   std::cerr << "Application is just started." << std::endl; 
   auto result = async_fun(1);
   do{
       future_status status = result.wait_for(1s);
       if ( status == future_status::ready){
          std::cerr << "result is " << result.get() << std::endl;
          break;
       }
       std::cerr << "Main process is working hard..." << std::endl;
    } while(true);
   std::cerr << "Application is just completed." << std::endl; 
   return 0;
}

