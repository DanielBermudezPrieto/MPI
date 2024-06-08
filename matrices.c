#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 1024

void initialize_matrices(double (*A)[MATRIX_SIZE], double (*B)[MATRIX_SIZE], double (*C)[MATRIX_SIZE], int rank) {
    if (rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                A[i][j] = rand() % 10 + 1;
                B[i][j] = rand() % 10 + 1;
                C[i][j] = 0.0; // Initialize C with zeros
            }
        }
    } else {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                C[i][j] = 0.0; // Initialize C with zeros
            }
        }
    }
    MPI_Bcast(A[0], MATRIX_SIZE * MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B[0], MATRIX_SIZE * MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

void perform_matrix_multiplication(double (*A)[MATRIX_SIZE], double (*B)[MATRIX_SIZE], double (*C)[MATRIX_SIZE], int rank, int size) {
    int rows_per_process = MATRIX_SIZE / size;
    int start_row = rank * rows_per_process;
    int end_row = (rank == size - 1) ? MATRIX_SIZE : start_row + rows_per_process;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            for (int k = 0; k < MATRIX_SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void gather_results(double (*C)[MATRIX_SIZE], double (*C_local)[MATRIX_SIZE], int rank, int size) {
    int rows_per_process = MATRIX_SIZE / size;
    MPI_Gather(rank == 0 ? MPI_IN_PLACE : C_local[rank * rows_per_process], rows_per_process * MATRIX_SIZE, MPI_DOUBLE,
               C[0], rows_per_process * MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    int rank, size;
    double start_time, end_time;
    double (*A)[MATRIX_SIZE], (*B)[MATRIX_SIZE], (*C)[MATRIX_SIZE];

    A = (double (*)[MATRIX_SIZE])malloc(sizeof(double) * MATRIX_SIZE * MATRIX_SIZE);
    B = (double (*)[MATRIX_SIZE])malloc(sizeof(double) * MATRIX_SIZE * MATRIX_SIZE);
    C = (double (*)[MATRIX_SIZE])malloc(sizeof(double) * MATRIX_SIZE * MATRIX_SIZE);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    initialize_matrices(A, B, C, rank);

    start_time = MPI_Wtime();
    perform_matrix_multiplication(A, B, C, rank, size);
    gather_results(C, C, rank, size);
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
