#include <thread>
#include <future>
#include <iostream>
#include <vector>

using namespace std;

vector<long> fun(long n) {
    cerr << "thread[" << this_thread::get_id() << "] is running fun(" << n << ") now..." << endl;
    vector<long> result;
    result.push_back(n);
    while (n > 1) {
        n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
        result.push_back(n);
    }
    this_thread::sleep_for(3s);
    return result;
}

int main() {
   std::cout << "Application is just started!" << std::endl;
    cerr << "thread[" << this_thread::get_id() << "] is calling async(...) now..." << endl;
    future<vector<long>> list_of_numbers = async(std::launch::async, fun, 17);

    for (auto &number: list_of_numbers.get())
        cout << number << " ";
    cout << endl;
    return 0;
}
