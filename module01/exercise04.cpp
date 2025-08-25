#include <iostream>
#include <thread>

// x: data, pointer ->  heap
int* x = new int{0}; // heap -> shared memory (P, t1, t2)

void fun(){
   std::cerr << "fun() is working..." << std::hex << x << "," << std::dec << *x << std::endl;
   for (auto i=0;i<1'000'000;++i) (*x)++;
   std::cerr << "fun() is returning..." << std::hex << x << "," << std::dec << *x << std::endl;
}

void gun(){
   std::cerr << "gun() is working..." << std::hex << x << "," << std::dec << *x << std::endl;
   for (auto i=0;i<1'000'000;++i) (*x)++;
   std::cerr << "gun() is returning..." << std::hex << x << "," << std::dec << *x << std::endl;
}

int main() { // Main process
   std::thread t1(fun);
   std::thread t2(gun);
   for (auto i=0;i<1'000'000;++i) (*x)++;
   std::cerr << "Application is just started: " << std::hex << x << "," << std::dec << *x << std::endl; 
   t1.join();
   t2.join();
   std::cerr << "Application is just completed: " << std::hex << x << "," << std::dec << *x << std::endl; 
   return 0;
}

