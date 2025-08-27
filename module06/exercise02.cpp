#include <atomic>
#include <thread>
#include <iostream>

using namespace std;

int global_data = -1;
atomic<bool> data_is_ready{false};

void producer(){
    while(data_is_ready.load()) {} // load, spin
    global_data = 42; // store
    data_is_ready.store(true,memory_order_release); //store   
}

void consumer(){
   while(!data_is_ready.load(memory_order_acquire)) {} // load, spin
   //cout << global_data << endl; // load
   if (global_data == -1){
      cerr << "There is an anomaly in instruction ordering!" << endl ;
   }
}

int main(){
   cout << "Application is just started..." << endl << flush;
   for (auto i=0;i<1'000'000;++i)
   {
     global_data = -1;
     data_is_ready.store(false);
     jthread t1{producer};
     jthread t2{consumer};
   }
   cout << "Application is just completed..." << endl << flush;
   return 0;
}
