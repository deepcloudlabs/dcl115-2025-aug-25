#include <iostream>
#include <mutex>
#include <thread>
#include <memory>

using namespace std;

class singleton { // thread-safe data structure
    int state;
    mutex m;
public:
    void fun(){
      lock_guard<mutex> lk{m};
      state++;
    }
    int get_state() {
      lock_guard<mutex> lk{m};
      return state;
    }
};

// lazy initializtion
shared_ptr<singleton> singleton_ptr{};
//mutex singleton_mutex;
once_flag singleton_flag;

void instantiate_singleton(){
   singleton_ptr.reset(new singleton);
}

void task(){
   call_once(singleton_flag,instantiate_singleton);
   for (auto i=0;i<100'000'000;++i) singleton_ptr->fun();
}

int main(){
    {
       jthread t1{task};
       jthread t2{task};
       jthread t3{task};
       jthread t4{task};
       jthread t5{task};
    }
    cerr << "state: " << singleton_ptr->get_state() << endl;
    return 0;
}
