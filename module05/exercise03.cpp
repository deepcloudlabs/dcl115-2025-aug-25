#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
mutex m;

struct task {
  explicit task(long& state): state(state) {}
  void operator()(){
     for (auto i=0;i<1'000'000'000;++i){
         lock_guard<mutex> lg{m};
         ++state;
     }
     cerr << "state: " << state << endl;
  }
private:
    long &state;
};

int main(){
   long local_state = 0;

  task task1{local_state};
  { 
    jthread t1{task1};
    jthread t2{task1};
    jthread t3{task1};
    jthread t4{task1};
    jthread t5{task1};
  }
  cerr << "local_state: " << local_state << endl;
  return 0;
}
