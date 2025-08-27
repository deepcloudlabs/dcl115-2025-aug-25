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
       global_data.store(-1, memory_order_release); // store -> store
       data_is_ready.store(false, memory_order_release);
       jthread producer{ 
          [](){
            global_data.store(42, memory_order_release);
            data_is_ready.store(true, memory_order_release);
          }
       };
       jthread consumer{ 
          [&](){
            while(!data_is_ready.load(memory_order_acquire)); // load -> load
            observed_value = global_data.load(memory_order_acquire);
          }
       };
       }
       if (observed_value != 42) anomally++;
   }
   cout << "Anomally is " << anomally << flush;
   cout << "Application is just completed..." << endl << flush;
   return 0;
}
