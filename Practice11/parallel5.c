#include<stdio.h>
#include<omp.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define MAT_SIZE 384


double big_call1(int id){
    double A[MAT_SIZE][MAT_SIZE];

    for (int i = 0; i < MAT_SIZE; i++) {
        for (int j = 0; j < MAT_SIZE; j++) {
            A[i][j] = rand() % 100;
        }
    }

    int i, j, k;
    double determinant = 1.0;

    for (i = 0; i < MAT_SIZE - 1; i++) {
        for (j = i + 1; j < MAT_SIZE; j++) {
            double factor = A[j][i] / A[i][i];

            for (k = i; k < MAT_SIZE; k++) {
                A[j][k] -= factor * A[i][k];
            }
        }
    }

    for (i = 0; i < MAT_SIZE; i++) {
        determinant *= A[i][i];
    }

    return determinant;
}

double big_call2(int id, int m, int n){

    if (m == 1 && n == 1) {
        return 1;
    }
    if (m == 0 || n == 0) {
        return 0;
    }

    return big_call2(id,m-1,n) + big_call2(id,m,n-1);
}

int main(){
    omp_set_num_threads(NUM_THREADS);

    double A[NUM_THREADS], B[NUM_THREADS];

    int id;
    double rtime = omp_get_wtime();

    #pragma omp parallel private (id)
    {
        id = omp_get_thread_num();

        printf("Gaussian elimination on thread %d in time %f\n", id, omp_get_wtime()-rtime );
        A[id] = big_call1(id);
        printf("Gaussian elimination end in thread %d at %f time\n", id, omp_get_wtime()-rtime);

        #pragma omp barrier

        printf("Recursive Grid Traveler on thread #%d in time %f\n", id, omp_get_wtime()-rtime);
        B[id] = big_call2(id,5,5);
        printf("Grid Traveler end in thread %d at %f time\n", id, omp_get_wtime()-rtime);
    }

    return 0;
}
