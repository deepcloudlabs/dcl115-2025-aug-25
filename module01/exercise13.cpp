#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <numeric>
#include <functional>
#if defined(__WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <pthread.h>
#endif
using namespace std;

void async_array_sum(vector<int>::iterator begin,vector<int>::iterator end,promise<int> promise_sum){
   // t1 -> producer: promise -> contract -> async
   #if defined(__linux__)
   pthread_setname_np(pthread_self(),"async_array_sum");
   #endif
   /*
      auto handle = GetCurrentThread();
      SetThreadDescription(handle, "async_array_sum");
   */
   std::cerr << "task is just started." << std::endl; 
   this_thread::sleep_for(1000s);
   std::cerr << "task is returning." << std::endl; 
   promise_sum.set_value(accumulate(begin,end,int{},plus<int>{}));
}

int main() { // t2 -> consumer: future -> contract 
   std::cerr << "Application is just started." << std::endl; 
   vector<int> numbers{1,2,3,4,5,6,7,8,9,10};
   promise<int> promise_sum{};
   future<int> result = promise_sum.get_future();
   thread t{async_array_sum,numbers.begin(),numbers.end(),move(promise_sum)}; 
   t.detach();
   do{
       future_status status = result.wait_for(1ms);
       if ( status == future_status::ready){
          std::cerr << "result is " << result.get() << std::endl;
          break;
       }
       std::cerr << "Main process is working hard..." << std::endl;
    } while(true);
   std::cerr << "Application is just completed." << std::endl;   
   return 0;
}

