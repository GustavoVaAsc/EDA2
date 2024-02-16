#include <stdio.h>
#include <omp.h>

// Parallel paradigm: Single Program - Multiple Data

int counter = 0;
#pragma omp threadprivate(counter)
int increment_counter(){
    return ++counter + omp_get_thread_num();
}

int main() {
    int num_threads = 4;
    //omp_set_num_threads(num_threads);
    printf("Counter before parallel: %d\n\n", counter);
    #pragma omp parallel
    {
        printf("Thread %d: Counter: %d\n", omp_get_thread_num(), increment_counter());
    }

    printf("\nCounter after parallel: %d\n", counter);
    return 0;
}