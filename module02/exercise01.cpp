#include <iostream>
#include <thread>
#include <pthread.h>

using namespace std;

void fun(){
   cerr << "thread id " << this_thread::get_id() << endl;
   pthread_t current_thread = pthread_self();
   cerr << "pthread id " << current_thread << endl;
}

int main(){ // Main Process
   {
      jthread t1{fun};
      jthread t2{fun};
      jthread t3{fun};
      jthread t4{fun};
   }
   cerr << "main thread id " << this_thread::get_id() << endl;
   return 0;
}
