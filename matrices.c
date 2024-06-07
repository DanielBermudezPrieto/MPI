#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 1024

void initialize_matrices(double (*A)[MATRIX_SIZE], double (*B)[MATRIX_SIZE]) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
        if (rank == 0) {
            for (int i = 0; i < MATRIX_SIZE; i++) {
                for (int j = 0; j < MATRIX_SIZE; j++) {
                    A[i][j] = rand() % 10 + 1;
                    B[i][j] = rand() % 10 + 1;
                }
            }
        }
        MPI_Bcast(A, MATRIX_SIZE * MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(B, MATRIX_SIZE * MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
}

void perform_matrix_multiplication(double (*A)[MATRIX_SIZE], double (*B)[MATRIX_SIZE], double (*C)[MATRIX_SIZE]) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = rank; i < MATRIX_SIZE; i += size) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            C[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void gather_results(double (*C)[MATRIX_SIZE]) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Recv(&C[i * (MATRIX_SIZE / size)], MATRIX_SIZE * MATRIX_SIZE / size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
        MPI_Send(&C[rank * (MATRIX_SIZE / size)], MATRIX_SIZE * MATRIX_SIZE / size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char** argv) {
    int rank;
    double start_time, end_time;
    double (*A)[MATRIX_SIZE], (*B)[MATRIX_SIZE], (*C)[MATRIX_SIZE];

    A = malloc(sizeof(double) * MATRIX_SIZE * MATRIX_SIZE);
    B = malloc(sizeof(double) * MATRIX_SIZE * MATRIX_SIZE);
    C = malloc(sizeof(double) * MATRIX_SIZE * MATRIX_SIZE);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    initialize_matrices(A, B);

    start_time = MPI_Wtime();
    perform_matrix_multiplication(A, B, C);
    gather_results(C);
    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Matrix multiplication completed in %.2f seconds.\n", end_time - start_time);
    }

    free(A);
    free(B);
    free(C);

    MPI_Finalize();
    return 0;
}
