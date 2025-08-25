# DCL-115: Multi-Threaded Programming in C++23
These projects are created as part of the following training: DCL-115: Multi-Threaded Programming in C++23

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

- Creating 1024 threads:  
  ```
  1024 * 8 MB = 8 GB (virtual memory)
  Physical memory usage ≈ 1/8 → ~1 GB
  ```
