#include <iostream>
#include <omp.h>

#define MAX_NUMBER (4294967295)

static const int NUMBER_OF_THREADS = 4;
using namespace std;
using namespace std::chrono;

bool guess(unsigned long long from_inclusive, unsigned long long to_inclusive, unsigned long long guessed) {
    volatile bool found = false;
#pragma omp parallel for
    for (unsigned long long i = from_inclusive; i <= to_inclusive; i++) {
        if (i == guessed) {
            found = true;
        }
    }
    return found;
}

int main() {
    unsigned long long guessed = MAX_NUMBER / 3;

    auto start = steady_clock::now();
    omp_set_num_threads(NUMBER_OF_THREADS);
    bool found = guess(0, MAX_NUMBER, guessed);
    auto finish = steady_clock::now();
    if (found) {
        printf("S: %7.4fs\n", (double) duration_cast<microseconds>(finish - start).count() / 1000000.0);
    } else {
        printf("error");
    }
}
