#include <iostream>
#include <mpi.h>
#include <random>

#define MASTER 0
#define FROM_MASTER 1
#define MAX_NUMBER (4294967295)
#define NUMBER_OF_POSSIBLE_COMBINATIONS (MAX_NUMBER + 1)


using namespace std;
using namespace std::chrono;

void master_routine(int num_of_workers, unsigned long long guessed);

void worker_routine();

unsigned long long get_random_number(unsigned long long lower_bound, unsigned long long upper_bound);

void guess(unsigned long long from_inclusive, unsigned long long to_exclusive, unsigned long long guessed);


int main(int argc, char *argv[]) {
    int procNum, procRank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    if (procNum < 2) {
        cout << "Need at least two MPI tasks. Quitting..." << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }
    int num_of_workers = procNum - 1;

    if (procRank == MASTER) {
        unsigned long long guessed = get_random_number(0, MAX_NUMBER);
        cout << "guessed: " << guessed << endl;
        master_routine(num_of_workers, guessed);
    } else {
        worker_routine();
    }

    MPI_Finalize();
}


void master_routine(int num_of_workers, unsigned long long guessed) {
    unsigned long long lower_bound, upper_bound;
    MPI_Status status;

    auto start = steady_clock::now();
    unsigned long long segment_size = NUMBER_OF_POSSIBLE_COMBINATIONS / num_of_workers;
    lower_bound = 0;
    upper_bound = segment_size + NUMBER_OF_POSSIBLE_COMBINATIONS % num_of_workers;

    for (int dest = 1; dest <= num_of_workers; ++dest) {
        MPI_Send(&lower_bound, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MASTER, MPI_COMM_WORLD);
        MPI_Send(&upper_bound, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MASTER, MPI_COMM_WORLD);
        MPI_Send(&guessed, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MASTER, MPI_COMM_WORLD);
        lower_bound = upper_bound;
        upper_bound = upper_bound + segment_size;
    }

    int guess_status;
    MPI_Recv(&guess_status, 1, MPI_INT, MPI_ANY_SOURCE, 200, MPI_COMM_WORLD, &status);

    auto finish = steady_clock::now();
    printf("S: %7.4fs\n", (double) duration_cast<microseconds>(finish - start).count() / 1000000.0);
    MPI_Abort(MPI_COMM_WORLD, 0);
}


void worker_routine() {
    MPI_Status status;
    unsigned long long lower_bound, upper_bound, guessed_received;
    MPI_Recv(&lower_bound, 1, MPI_UNSIGNED_LONG_LONG, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
    MPI_Recv(&upper_bound, 1, MPI_UNSIGNED_LONG_LONG, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
    MPI_Recv(&guessed_received, 1, MPI_UNSIGNED_LONG_LONG, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);

    guess(lower_bound, upper_bound, guessed_received);
}

void guess(unsigned long long from_inclusive, unsigned long long to_exclusive, unsigned long long guessed) {
    for (unsigned long long i = from_inclusive; i < to_exclusive; i++) {
        if (i == guessed) {
            int guess_status = 1;
            MPI_Send(&guess_status, 1, MPI_INT, MASTER, 200, MPI_COMM_WORLD);
            return;
        }
    }
}


unsigned long long get_random_number(unsigned long long lower_bound, unsigned long long upper_bound) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<long long unsigned> distribution(lower_bound, upper_bound);
    return distribution(generator);
}
