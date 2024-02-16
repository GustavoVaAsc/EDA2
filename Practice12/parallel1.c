#include <stdio.h>
#include <omp.h>

static long num_steps = 10000000;
double step;

int main() {
    int i;
    double pi = 0, startTime;
    startTime = omp_get_wtime();

    step = 1.0 / (double)num_steps;

    #pragma omp parallel for
    for (int i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;

        #pragma omp atomic
        pi += 4.0 / (1.0 + x * x);
    }

    pi *= step; 

    printf("Pi = %f\n", pi);
    printf("Time = %f\n", omp_get_wtime() - startTime);

    return 0;
}