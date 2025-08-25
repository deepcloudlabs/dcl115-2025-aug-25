#include <thread>
#include <iostream>
using namespace std;

int main(){
   cerr << thread::hardware_concurrency() << endl;
   return 0;
}
