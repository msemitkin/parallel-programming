#include <iostream>

using namespace std;

bool guess(unsigned long long from_inclusive, unsigned long long to_inclusive, unsigned long long guessed) {
    for (unsigned long long i = from_inclusive; i <= to_inclusive; i++) {
        if (i == guessed) {
            return true;
        }
    }
    return false;
}


int main() {
    unsigned long long guessed = 20;

    clock_t start = clock();
    guess(0, ULLONG_MAX, guessed);
    clock_t finish = clock();
    cout << "guessed number in: " << (finish - start) << endl;
}
