#include <iostream>
#include <thread>

using namespace std;

struct A {
    int state;
    explicit A(const int data=0) : state(data){
       cerr << "A::A()" << endl;
    }
    ~A(){
       cerr << "A::~A()" << endl;
    }
    void fun(){
      for (auto i=0;i<1'000'000;++i)
          ++state;
    }
};

thread_local A a{};
thread_local int i;

void task(){
  int k=0;
  ++i;
  a.fun();
  cerr << "a.state: " << hex << (&a.state) << ", " << dec << a.state << endl;
}

int main(){
    cerr << "Application is just starteed: a.state: " << hex << (&a.state) << ", " << dec << a.state << endl;
     {
       jthread t1{task};
       jthread t2{task};
       jthread t3{task};
       jthread t4{task};
       jthread t5{task};
     }
    cerr << "Application is just completed: a.state: " << hex << (&a.state) << ", " << dec << a.state << endl;
    return 0;
}
