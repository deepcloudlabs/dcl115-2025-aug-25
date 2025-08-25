#include <iostream>
#include <thread>
void hello(){
   std::cerr << "Hello Concurrent World!\n";
}

int main() {
   std::cerr << "Application is just started\n";
   std::thread t(hello);
   t.join();
   std::cerr << "Application is just completed\n";
   return 0;
}

