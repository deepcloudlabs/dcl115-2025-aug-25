#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

bool flag = false;
mutex m;

void fun(){
    cerr << "fun() is just started..." << endl;
    this_thread::sleep_for(5s);
    flag = true;
    cerr << "fun() is just exitted..." << endl;
}

void gun(){
    unique_lock<mutex> ul{m};
    while( flag == false ){
       ul.unlock();
       this_thread::sleep_for(100ms);
       ul.lock(); 
    } 
    cerr << this_thread::get_id() << " flag is up!" << endl;
}


int main(){
    cerr << "Application is just started!" << endl;
    {
    jthread t1{fun};
    jthread t2{gun};
    jthread t3{gun};
    jthread t4{gun};
    }
    cerr << "Application is just completed!" << endl;
    return 0;
}
