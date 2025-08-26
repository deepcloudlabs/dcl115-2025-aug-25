#include <iostream>
#include <thread>

/*
   threads -> resource (file/memory/socket/connection/...) 
   mutator thread writes to state
   thread safety
   i) application
  ii) data structures




*/
using namespace std;

int global_state = 0; // data
mutex m;

void fun(){
   for(int i=0;i<100'000'000;++i){
      lock_guard<mutex> lock(m);
      ::global_state++; // critical section -> race condition
   }
}

int main(){
    cerr << "Application is just started..." << endl;
    {
       jthread t1{fun};
       jthread t2{fun};
       jthread t3{fun};
       jthread t4{fun};
       jthread t5{fun};
       jthread t6{fun};
       jthread t7{fun};
       jthread t8{fun};
    }
    cerr << "Application is just completed...global_state: " << ::global_state << endl;
    return 0;
}
