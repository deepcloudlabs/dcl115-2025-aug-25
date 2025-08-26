#include <iostream>
#include <thread>
#include <functional>
#include <numeric>

using namespace std;

void fun1(int x,int y){
  cerr << "fun1(): x: " << x << ",y: " << y << endl;
}

void fun2(int *array,int size){
  cerr << "fun2(): array: " << hex << array << ",size: " << size << endl;
}

void fun3(int &sum,const int *array,int size){
  cerr << "fun3(): sum: " << sum << ",array: " << hex << array  << ", size: " << dec << size << endl;
}

void fun4(const int &sum,const int *array,int size){
  cerr << "fun4(): sum: " << sum << ",array: " << hex << array  << ", size: " << dec << size << endl;
}


int main(){ // Main Process
  int numbers[]{1,2,3,4,5,6,7,8,9,10}; // stack array
  int result;
  jthread t1{fun1,3,5};
  jthread t2{fun2,numbers,10};
  jthread t3{fun3,ref(result),numbers,10};
  jthread t4{fun4,cref(result),numbers,10};
  return 0;
}
