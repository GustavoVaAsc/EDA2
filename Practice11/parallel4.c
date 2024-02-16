#include<stdio.h>
#include<omp.h>

static long num_steps = 100000;
double step;

int main(){
    int i;
    int nThreads = omp_get_max_threads();
    printf("Current maximum threads available: %d\n\n",nThreads);

    double pi = 0, rtime = 0; // rtime is the running time variable
    double a[nThreads];

    rtime = omp_get_wtime();
    step = 1.0 / (double)num_steps;

    #pragma omp parallel
    {
        double x, sum = 0;
        for(int i = 0; i < num_steps; i += nThreads) {
            x = (i + 0.5 + omp_get_thread_num()) * step;
            sum = sum + 4.0/(1.0 + x*x);
        }
        a[omp_get_thread_num()] = step * sum;
    }
    for (i=0; i < nThreads; i++){
        pi += a[i];
    }

    printf("PI = %f\n", pi);
    printf("Time elapsed: %f\n",omp_get_wtime() - rtime);
}