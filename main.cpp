#include <iostream>
#include <mpi.h>

#define MASTER 0
#define FROM_MASTER 1
//#define MAX_NUMBER (ULLONG_MAX - 1)
//#define NUMBER_OF_POSSIBLE_COMBINATIONS (MAX_NUMBER + 1)
#define MAX_NUMBER (4294967295)
#define NUMBER_OF_POSSIBLE_COMBINATIONS (MAX_NUMBER + 1)


using namespace std;
using namespace std::chrono;


void guess(unsigned long long from_inclusive, unsigned long long to_inclusive, unsigned long long guessed) {
    for (unsigned long long i = from_inclusive; i <= to_inclusive; i++) {
        if (i == guessed) {
            int guess_status = 1;
            MPI_Send(&guess_status, 1, MPI_INT, MASTER, 200, MPI_COMM_WORLD);
            return;
        }
    }
}


void worker_routine(int procRank) {
    MPI_Status status;
    unsigned long long lower_bound, upper_bound, guessed_received;
    MPI_Recv(&lower_bound, 1, MPI_UNSIGNED_LONG_LONG, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
    MPI_Recv(&upper_bound, 1, MPI_UNSIGNED_LONG_LONG, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
    MPI_Recv(&guessed_received, 1, MPI_UNSIGNED_LONG_LONG, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
    cout << "From:" << procRank
         << "\nlowerBound: " << lower_bound
         << "\nupperBound: " << upper_bound
         << endl;

    guess(lower_bound, upper_bound, guessed_received);
}


void master_routine(int num_of_workers, unsigned long long guessed) {
    unsigned long long lower_bound, upper_bound;
    MPI_Status status;
//    cout << "number of workers: " << num_of_workers << endl;

    auto start = steady_clock::now();
    unsigned long long segment_size = NUMBER_OF_POSSIBLE_COMBINATIONS / num_of_workers;
    lower_bound = 0;
    upper_bound = segment_size + NUMBER_OF_POSSIBLE_COMBINATIONS % num_of_workers;
    cout << "segment size: " << segment_size << endl;
    cout << "NUMBER_OF_POSSIBLE_COMBINATIONS: " << NUMBER_OF_POSSIBLE_COMBINATIONS << endl;
    cout << "NUMBER_OF_POSSIBLE_COMBINATIONS % num_of_workers: " << NUMBER_OF_POSSIBLE_COMBINATIONS % num_of_workers
         << endl;

    for (int dest = 1; dest <= num_of_workers; ++dest) {
        MPI_Send(&lower_bound, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MASTER, MPI_COMM_WORLD);
        MPI_Send(&upper_bound, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MASTER, MPI_COMM_WORLD);
        MPI_Send(&guessed, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MASTER, MPI_COMM_WORLD);
        lower_bound = upper_bound + 1;
        upper_bound = upper_bound + segment_size;

        //        cout << "sent to " << dest << endl;
    }

    int guess_status;
    MPI_Recv(&guess_status, 1, MPI_INT, MPI_ANY_SOURCE, 200, MPI_COMM_WORLD, &status);

    auto finish = steady_clock::now();
    printf("S: %7.4fs\n", (double) duration_cast<microseconds>(finish - start).count() / 1000000.0);
    MPI_Abort(MPI_COMM_WORLD, 0);
}

int main(int argc, char *argv[]) {
    unsigned long long guessed = MAX_NUMBER / 3;
    int procNum;
    int procRank;
    int num_of_workers;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    if (procNum < 2) {
        cout << "Need at least two MPI tasks. Quitting..." << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }
    num_of_workers = procNum - 1;

    if (procRank == MASTER) {
        master_routine(num_of_workers, guessed);
    } else {
        worker_routine(procRank);
    }

    MPI_Finalize();
}
