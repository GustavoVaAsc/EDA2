#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define NUM_THREADS 4

double funcBig(int m, int n){
    if (m == 1 && n == 1) {
        return 1;
    }
    if (m == 0 || n == 0) {
        return 0;
    }

    return funcBig(m-1,n) + funcBig(m,n-1);
}

int main(){
    omp_set_num_threads(NUM_THREADS);

    double A = 0;
    double rtime = omp_get_wtime();

    #pragma omp parallel
    {
        double tmp, B;
        tmp = funcBig(5,5);
        printf("Thread #%d before atomic, in time %f with result: %f\n", omp_get_thread_num(), omp_get_wtime()-rtime,A );
        #pragma omp atomic
        A += tmp;
        printf("Thread #%d after atomic, in time %f with result: %f \n", omp_get_thread_num(), omp_get_wtime()-rtime, A );
    }

    printf("A: \n",A);

    return 0;
}
