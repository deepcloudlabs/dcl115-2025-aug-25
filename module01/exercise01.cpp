//text: fun, gun, main
//      A::method1, A::method2

//data: x,k,j

//stack: local/automatic variables -> z, p

//heap: new int{108} --> delete p

class A {
   public:
      void method1();
      void method2();
};

void fun(){
    static int k=0;
}

void gun(){
   double arr[100'000];
}

int x = 42;

int main(){
   int z = 100;
   int *p = new int{108};
   static double j = 3.141516;

   delete p;
   return 0;
}
