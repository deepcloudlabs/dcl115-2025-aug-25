#include <iostream>
#include <thread>
#include <functional>
using namespace std;

struct task { // function object -> functor
  void operator()(int data){
     std::cerr << "task() is running..." << data << std::endl;
  }
};

int main() { // Main process
   function<void(int)> lambda1 =[](int data){
       std::cerr << "lambda expression 1 is running..." << data << std::endl;
   };
   auto lambda2 =[](int data){
       std::cerr << "lambda expression 2 is running..." << data << std::endl;
   };
   std::thread t1{lambda1 , 100};
   std::thread t2{[](int data){ std::cerr << "lambda expression 3 is running..." << data << std::endl; }, 200};
   std::thread t3{lambda2 , 300};
   std::cerr << "Application is just started." << std::endl; 
   t1.join();
   t2.join();
   t3.join();
   std::cerr << "Application is just completed." << std::endl; 
   return 0;
}

