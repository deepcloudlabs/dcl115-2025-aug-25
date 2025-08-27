#include <atomic>
#include <thread>
#include <iostream>

using namespace std;

atomic<int> global_data{-1};
atomic<bool> data_is_ready{false};


int main(){
   cout << "Application is just started..." << endl << flush;
   auto anomally = 0;
   for (auto i=0;i<1'000'000;++i)
   {  
       auto observed_value = 0;
       {
       global_data.store(-1, memory_order_relaxed);
       data_is_ready.store(false, memory_order_relaxed);
       jthread producer{ 
          [](){
            global_data.store(42, memory_order_relaxed);
            data_is_ready.store(true, memory_order_relaxed);
          }
       };
       jthread consumer{ 
          [&](){
            while(!data_is_ready.load(memory_order_relaxed));
            observed_value = global_data.load(memory_order_relaxed);
          }
       };
       }
       if (observed_value != 42) anomally++;
   }
   cout << "Anomally is " << anomally << flush;
   cout << "Application is just completed..." << endl << flush;
   return 0;
}
