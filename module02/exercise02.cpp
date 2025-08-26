#include <iostream>
#include <thread>
#include <functional>
#include <numeric>

using namespace std;

void fun(int &result,int *begin,int* end){
    result = accumulate(begin,end,int{},plus<int>{});
}

int main(){ // Main Process
   int sum=0; // Stack
   { 
      int numbers[]{1,2,3,4,5,6,7,8,9,10}; // stack array
      int *p = numbers; // stack pointer
      int *q = numbers+10;
      jthread t{fun,ref(sum),p,q};
      cerr << "sum: " << sum << endl;
   }
   
   cerr << "sum: " << sum << endl;
   return 0;
}
