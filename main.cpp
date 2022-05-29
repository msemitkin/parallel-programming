#include <iostream>
#include <omp.h>

#define MAX_NUMBER (4294967295)

static const int NUMBER_OF_THREADS = 4;
using namespace std;
using namespace std::chrono;

int main() {
    unsigned long long guessed = MAX_NUMBER / 10;

    auto start = steady_clock::now();
    omp_set_num_threads(NUMBER_OF_THREADS);
    volatile bool found = false;
#pragma omp parallel for shared(found)
    for (unsigned long long i = 0; i <= MAX_NUMBER; i++) {
        if (!found) {
            if (i == guessed) {
                found = true;
            }
        }
    }
    auto finish = steady_clock::now();
    if (found) {
        printf("S: %7.4fs\n", (double) duration_cast<microseconds>(finish - start).count() / 1000000.0);
    }
}
