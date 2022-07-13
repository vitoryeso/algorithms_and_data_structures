#include <iostream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

unsigned fib(unsigned n) {
    if (n == 1 || n == 2) {
        return 1;
    }
    else {
        return fib(n - 1) + fib(n - 2);
    }
}

unsigned fib_memo(unsigned n, unsigned *memo) {
    if (memo[n] != 0) {
        return memo[n];
    }
    unsigned result;

    if (n == 1 || n == 2) {
        result = 1;
    }
    else {
        result = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
    }
    memo[n] = result;
    return result;
}

unsigned fib_bottom_up(unsigned n) {
    unsigned prov;
    unsigned *fib = new unsigned[n + 1];

    for (int i=1; i<n+1; i++) {
        if (i <= 2) {
            prov = 1;
        }
        else {
            prov = fib[i - 1] + fib[i - 2];
        }
        fib[i] = prov;
    }
    return fib[n];
}
int main(int argc, char **argv) {
    cout << "oi kkkk" << endl;

    unsigned n = stoi(argv[1]);

    /*
    auto start = high_resolution_clock::now();

    cout << "fib(" << n << "): " << fib(n) << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    */

    unsigned *memo = new unsigned[n];
    for (unsigned i=0; i<n; i++) memo[n] = 0;

    auto start = high_resolution_clock::now();

    cout << "fib(" << n << "): " << fib_memo(n, memo) << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now();
    cout << "fib(" << n << "): " << fib_bottom_up(n) << endl;
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
}