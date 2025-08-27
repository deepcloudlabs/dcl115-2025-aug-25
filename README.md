# DCL-115: Multithreaded Programming in C++23
These projects are created as part of the following training: DCL-115: Multithreaded Programming in C++23

Please follow the link for the complete training catalog: https://www.deepcloudlabs.com/resources

# C++ Processes and Threads

## Application Model
```
c++ -> app -> process (OS)
```

### Types
1. **Multi-process**
2. **Single Process → Multi-Thread**
   - High throughput, low latency
3. **Multi-process → Multi-Thread**
   - IPC (Inter-Process Communication)

---

## Process

- **System Resources**: CPU, Memory  
- **Relationships**:  
  - Parent Process ⟷ Child Process  

### Memory Layout
- Virtual Memory ⟷ Physical Memory  
- Segments:
  - **Stack**
  - **Heap**
  - **Text**
  - **Data**

> `fork()`: parent process’ memory → duplicated into child process  

---

## Thread

- **System Resources**: CPU, Memory  
- **Mapping**:  
  - Process → Threads: `t1`, `t2`, `t3`

### Memory Sharing
- **Shared Across Threads**: Text, Data, Heap  
- **Private per Thread**: Stack  

Example:
```
t1 -> fun()
t2 -> gun()
t3 -> sun()
```

Each thread has its **own Stack**, but shares the process’s **Text, Data, and Heap** with others.  
This aligns with the "function" execution model.  

---

## Memory Considerations

- Example:  
  ```
  256 MB process -> 1024 children -> 256 GB
  ```

- `ulimit -a` (example output):
  ```
  stack size                  (kbytes, -s) 8192
  ```

# Multitasking, Concurrency, and Parallel Programming

## Multitasking → Concurrency
- Condition: **CPU ≥ 1**

---

## Parallel Programming → Parallelism
- Condition: **CPU > 1**

---

## Combined Model
**Multitasking + Parallel Programming → Multi-thread Programming**
  

- Creating 1024 threads:  
  ```
  1024 * 8 MB = 8 GB (virtual memory)
  Physical memory usage ≈ 1/8 → ~1 GB
  ```

  # C++ Thread Creation Examples

```cpp
#include <thread>
using namespace std;

void fun(int data);

struct task { // function object -> functor
  void operator()(int data){
     std::cerr << "task() is running..." << data << std::endl;
  }
};

int main(){
    thread t1{fun,100};     // (1) using a regular function
    thread t2{task{},100};  // (2) using a functor (function object)

    function<void(int)> lambda1 = [](int data){
       std::cerr << "lambda expression 1 is running..." << data << std::endl;
    };

    auto lambda2 = [](int data){
       std::cerr << "lambda expression 2 is running..." << data << std::endl;
    };

    std::thread t1{lambda1 , 100}; 
    // (3) similar to (2), using a std::function with a lambda

    std::thread t2{
        [](int data){ 
            std::cerr << "lambda expression 3 is running..." << data << std::endl; 
        }, 
        200
    }; 
    // (3) direct inline lambda expression
}
```

---

## Key Points

1. **Regular Function**: `thread t1{fun,100};`  
   - Creates a thread executing a plain function.

2. **Functor (Function Object)**: `thread t2{task{},100};`  
   - A struct/class with `operator()` can be passed to a thread.

3. **Lambda Expressions**:  
   - Lambdas can be wrapped in `std::function` or passed directly.  
   - Provides inline, lightweight function definitions.

