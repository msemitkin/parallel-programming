#include <omp.h>
#include <printf.h>
//#define MAX_NUMBER (ULLONG_MAX - 1)
#define MAX_NUMBER (4294967295)

void guess(unsigned long long from_inclusive, unsigned long long to_inclusive, unsigned long long guessed) {
    for (unsigned long long i = from_inclusive; i <= to_inclusive; i++) {
        if (i == guessed) {
            break;
        }
    }
}


int main() {
//    unsigned long long guessed = MAX_NUMBER / 3;

    unsigned long long upper_bound_exclusive = MAX_NUMBER + 1;
    unsigned long long lower_bound_inclusive = 0;
    unsigned long long guessed = MAX_NUMBER - 5;

    omp_set_num_threads(4);
    #pragma omp parallel for
    for (unsigned long long i = lower_bound_inclusive; i < upper_bound_exclusive; i++) {
        if (i == guessed) {
            printf("found");
        }
    }
    printf("S: %7.4fs\n", 1.0);
}
