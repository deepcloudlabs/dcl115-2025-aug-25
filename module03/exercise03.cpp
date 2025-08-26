#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
mutex global_mutex;

class vehicle { // thread-safe data structure
   const double capacity;
   double current_load;
   recursive_mutex m;
public:
   explicit vehicle(const double capacity=1'000) : capacity(capacity) {}
   double get_capacity() const { return this->capacity; }
   double get_current_load() const { return this->current_load; }
   double load(double weight) {
      lock_guard<recursive_mutex> lock{m};
      if (weight <= 0) return this->current_load;
      if ((weight+this->current_load) > this->capacity) return this->current_load;
      this->current_load += weight;
      return this->current_load;
   }

   double unload(double weight) {
      lock_guard<recursive_mutex> lock{m};
      if (weight <= 0) return this->current_load;
      if (weight > this->current_load) return this->current_load;
      this->current_load -= weight;
      return this->current_load;
   }
   friend void transfer_load(vehicle& from,vehicle& to,double weight);
};

void transfer_load(vehicle& from,vehicle& to,double weight){
      { // atomic 
        unique_lock<recursive_mutex> lock1{from.m,defer_lock};
        unique_lock<recursive_mutex> lock2{to.m,defer_lock};
        lock(lock1,lock2);
//        lock_guard<mutex> lock{global_mutex};
        from.unload(weight);
        to.load(weight);
      }
} 

void transfer_task(vehicle& from,vehicle& to){
     for (auto i=0;i<10'000'000;++i){
        transfer_load(from,to,1);
     }
} 
  
vehicle vehicle1{100'000'000}; 
vehicle vehicle2{200'000'000}; 

int main(){
    vehicle1.load(100'000'000);
    {
       jthread t1{transfer_task,ref(vehicle1),ref(vehicle2)};
       jthread t2{transfer_task,ref(vehicle1),ref(vehicle2)};
       jthread t3{transfer_task,ref(vehicle1),ref(vehicle2)};
       jthread t4{transfer_task,ref(vehicle1),ref(vehicle2)};
       jthread t5{transfer_task,ref(vehicle1),ref(vehicle2)};
    }
    cerr << "vehicle1's current load: " << vehicle1.get_current_load() << endl;
    cerr << "vehicle2's current load: " << vehicle2.get_current_load() << endl;
    return 0;
}
