#include<stdio.h>
#include<omp.h>

static long num_steps = 100000000;
double step;

// Parallel paradigm: Loop parallelism

int main(){
    double x, pi, sum = 0.0, it = 0;
    it = omp_get_wtime();
    int i;
    step = 1.0 / (double)num_steps;

    #pragma omp parallel for reduction(+:sum) private(x)
    for (i=0; i<num_steps; i++){
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
    }
    pi = step * sum;
    printf("PI = %f\n", pi);
    return 0;
}