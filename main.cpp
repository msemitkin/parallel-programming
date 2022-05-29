#include <iostream>
#include <omp.h>
#include <random>

#define MAX_NUMBER (4294967295)

static const int NUMBER_OF_THREADS = 5;
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
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<long long unsigned> distribution(0, MAX_NUMBER);
    unsigned long long guessed = distribution(generator);
    cout << "guessed: " << guessed << endl;

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
