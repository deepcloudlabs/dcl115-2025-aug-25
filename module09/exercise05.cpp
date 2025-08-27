#include <iostream>
#include <future>
#include <algorithm>
#include <vector>
#include <list>
#include <thread>

using namespace std;
class joinable_threads {
    vector<thread>& threads;
public:
    explicit joinable_threads(vector<thread>& threads_)
            : threads(threads_)
    {}
    ~joinable_threads(){
        for (auto& t : threads)
        {
            if (t.joinable())
                t.join();
        }
    }
} ;

template <typename Iterator,typename T>
typename iterator_traits<Iterator>::difference_type
parallel_count(Iterator first, Iterator last,const T& value)
{
    int length= distance(first,last);

    if (length==0) return 0;

    unsigned long const min_per_thread=25;
    unsigned long const max_threads= (length+min_per_thread-1)/min_per_thread;
    unsigned long const harware_threads= thread::hardware_concurrency();
    unsigned long const num_threads=
            min(
                    harware_threads!=0?harware_threads:2,
                    max_threads
            );
    unsigned long const block_size= length / num_threads;
    vector<future<int>> futures(num_threads-1);
    vector<thread> threads(num_threads-1);
    joinable_threads thread_joiner(threads);

    Iterator block_start= first;
    for (unsigned long int i=0;i<(num_threads-1);++i)
    {
        Iterator block_end= block_start;
        advance(block_end,block_size);
        packaged_task<int(void)> task(
                [=](){
                    return count(block_start,block_end,value);
                }
        );
        futures[i]= task.get_future();
        threads[i]= thread(move(task));
        block_start= block_end;
    }

    typename iterator_traits<Iterator>::difference_type counter= count(block_start,last,value);
    for (unsigned long int i=0;i<(num_threads-1);++i)
    {
        counter += futures[i].get();
    }
    return counter;
}

// Linear Partition
int main(){
    vector<int> numbers;
    numbers.resize(640'000'000);
    int f= 5;
    for (int i=1;i<=640'000'000;++i)
    {
        numbers.push_back(i%10);
    }
    auto start = std::chrono::high_resolution_clock::now();
    cout << parallel_count(
            numbers.begin(),
            numbers.end(),
            f
    ) << endl ;
    auto stop = std::chrono::high_resolution_clock::now();
    cout << "duration for parallel: " << chrono::duration_cast<chrono::microseconds>(stop - start).count() << " microsec." << endl << flush;
    start = std::chrono::high_resolution_clock::now();
    cout << count(
            numbers.begin(),
            numbers.end(),
            f
    ) << endl ;
    stop = std::chrono::high_resolution_clock::now();
    cout << "duration for serial: " << chrono::duration_cast<chrono::microseconds>(stop - start).count() << " microsec." << endl << flush;
    return 0;
}
