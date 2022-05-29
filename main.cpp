#include <iostream>

using namespace std;
using namespace std::chrono;

#define MAX_NUMBER (4294967295)

bool guess(unsigned long long from_inclusive, unsigned long long to_inclusive, unsigned long long guessed) {
    for (unsigned long long i = from_inclusive; i <= to_inclusive; i++) {
        if (i == guessed) {
            return true;
        }
    }
    return false;
}


int main() {
    unsigned long long guessed = MAX_NUMBER / 3;

    auto start = steady_clock::now();
    guess(0, MAX_NUMBER, guessed);
    auto finish = steady_clock::now();
    printf("S: %7.4fs\n", (double) duration_cast<microseconds>(finish - start).count() / 1000000.0);
}
