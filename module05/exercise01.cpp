#include <iostream>
#include <thread>

using namespace std;

struct task {
  explicit task(int& state): state(state) {}
  void operator()(){
     for (auto i=0;i<1'000'000;++i){
         ++state;
     }
     cerr << "state: " << state << endl;
  }
private:
    int &state;
};

int main(){
   int local_state = 0;

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
